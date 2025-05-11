#include "pms7003.h"

__attribute__((unused)) static const char *TAG = "PMS7003";

esp_err_t pms7003_initUart(uart_config_t *uart_config)
{
    // Cấu hình UART
    uart_config->baud_rate = 9600;
    uart_config->data_bits = UART_DATA_8_BITS;
    uart_config->parity = UART_PARITY_DISABLE;
    uart_config->stop_bits = UART_STOP_BITS_1;
    uart_config->flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config->source_clk = UART_SCLK_APB;

    // Cài đặt driver UART
    esp_err_t error_1 = uart_driver_install(CONFIG_PMS_UART_PORT, (RX_BUFFER_SIZE * 2), 0, 0, NULL, 0);
    if (error_1 != ESP_OK) {
        ESP_LOGE(__func__, "Failed to install UART driver");
        return ESP_ERROR_PMS7003_INIT_UART_FAILED;
    }

    // Cấu hình tham số UART
    esp_err_t error_2 = uart_param_config(CONFIG_PMS_UART_PORT, uart_config);
    if (error_2 != ESP_OK) {
        ESP_LOGE(__func__, "Failed to configure UART parameters");
        uart_driver_delete(CONFIG_PMS_UART_PORT);
        return ESP_ERROR_PMS7003_INIT_UART_FAILED;
    }

    // Cấu hình chân UART
    esp_err_t error_3 = uart_set_pin(CONFIG_PMS_UART_PORT, CONFIG_PMS_PIN_TX, CONFIG_PMS_PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (error_3 != ESP_OK) {
        ESP_LOGE(__func__, "Failed to set UART pins");
        uart_driver_delete(CONFIG_PMS_UART_PORT);
        return ESP_ERROR_PMS7003_INIT_UART_FAILED;
    }

    // Xóa buffer UART
    uart_flush(CONFIG_PMS_UART_PORT);

    ESP_LOGI(__func__, "PMS7003 UART port initialize successful.");
    return ESP_OK;
}

// static esp_err_t pms7003_checkCRC(uint8_t data[32]);

esp_err_t pms7003_activeMode(void)
{
    esp_err_t errorReturn = uart_write_bytes(CONFIG_PMS_UART_PORT, pms7003_commandActiveMode, sizeof(pms7003_commandActiveMode));
    if (errorReturn == -1)
    {
        ESP_LOGE(__func__, "Set active mode failed.");
        return ESP_ERROR_PMS7003_SET_ACTIVE_MODE_FAILED;
    }
    else
    {
        ESP_LOGI(__func__, "Set active mode successfuly.");
        return ESP_OK;
    }
}

esp_err_t pms7003_readData(const int pms_modeAmbience, uint32_t *pm1_0, uint32_t *pm2_5, uint32_t *pm10)
{
    uint8_t rawData[128];
    bool check = false;
    SemaphoreHandle_t print_muxtex = NULL;
    print_muxtex = xSemaphoreCreateMutex();

    // Đọc dữ liệu thô với thời gian chờ dài hơn
    int lenghtSensorDataArray;
    lenghtSensorDataArray = uart_read_bytes(CONFIG_PMS_UART_PORT, rawData, RX_BUFFER_SIZE, 1000 / portTICK_PERIOD_MS);
    
    // Thêm log để debug
    ESP_LOGI(__func__, "Bytes received: %d", lenghtSensorDataArray);
    if (lenghtSensorDataArray > 0) {
        ESP_LOGI(__func__, "Raw data: ");
        for (int i = 0; i < lenghtSensorDataArray; i++) {
            printf("%02X ", rawData[i]);
        }
        printf("\n");
    }
    
    if (lenghtSensorDataArray <= 0) {
        ESP_LOGE(__func__, "No data received from sensor");
        *pm1_0 = PMS_ERROR_INVALID_VALUE;
        *pm2_5 = PMS_ERROR_INVALID_VALUE;
        *pm10 = PMS_ERROR_INVALID_VALUE;
        xSemaphoreGive(print_muxtex);
        vSemaphoreDelete(print_muxtex);
        return ESP_ERROR_PMS7003_READ_DATA_FAILED;
    }

    xSemaphoreTake(print_muxtex, portMAX_DELAY);

    // Tìm kiếm byte bắt đầu trong toàn bộ buffer
    for (size_t i = 0; i < lenghtSensorDataArray - 1; i++)
    {
        if (rawData[i] == START_CHARACTER_1 &&   // Kiểm tra byte bắt đầu thứ nhất
            rawData[i + 1] == START_CHARACTER_2) // Kiểm tra byte bắt đầu thứ hai
        {
            uint8_t startByte;
            startByte = i;
            startByte += (pms_modeAmbience == indoor) ? 4 : 10; // atmospheric từ byte thứ 10, standard từ byte thứ 4

            // Kiểm tra xem có đủ dữ liệu không
            if (startByte + 6 > lenghtSensorDataArray) {
                ESP_LOGE(__func__, "Incomplete data frame");
                break;
            }

            *pm1_0 = ((rawData[startByte] << 8) + rawData[startByte + 1]);
            *pm2_5 = ((rawData[startByte + 2] << 8) + rawData[startByte + 3]);
            *pm10 = ((rawData[startByte + 4] << 8) + rawData[startByte + 5]);

            // Kiểm tra giá trị hợp lệ
            if (*pm1_0 > 1000 || *pm2_5 > 1000 || *pm10 > 1000) {
                ESP_LOGE(__func__, "Invalid sensor values detected");
                break;
            }

            ESP_LOGI(__func__, "PMS7003 sensor read data successful.");
            ESP_LOGI(__func__, "PM1.0: %" PRIu32 "ug/m3  PM2.5: %" PRIu32 "ug/m3  PM10: %" PRIu32 "ug/m3.\r", *pm1_0, *pm2_5, *pm10);
            xSemaphoreGive(print_muxtex);
            vSemaphoreDelete(print_muxtex);
            check = true;
            break;
        }
    }

    if (check == false)
    {
        *pm1_0 = PMS_ERROR_INVALID_VALUE;
        *pm2_5 = PMS_ERROR_INVALID_VALUE;
        *pm10 = PMS_ERROR_INVALID_VALUE;
        ESP_LOGE(__func__, "PMS7003 sensor read data failed.");
        xSemaphoreGive(print_muxtex);
        vSemaphoreDelete(print_muxtex);
        return ESP_ERROR_PMS7003_READ_DATA_FAILED;
    }
    return ESP_OK;
}
