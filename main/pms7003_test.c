#include <stdio.h>
#include "pms7003.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <ds3231.h>
#include <string.h>
#include "DS3231Time.h"

uint32_t Pm1_0, Pm2_5, Pm10;
esp_err_t pms7003_task(void *pvParameters)
{
    uint32_t pm1_0, pm2_5, pm10;
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB
    };

    // Cấu hình UART
    esp_err_t err;
    err = pms7003_initUart(&uart_config);
    if (err != ESP_OK)
    {
        ESP_LOGE("PMS7003", "Failed to initialize UART: 0x%x", err);
        vTaskDelete(NULL);  // Dừng Task nếu có lỗi
        return err;
    }

    //Kích hoạt Active Mode
    err = pms7003_activeMode();
    if (err != ESP_OK)
    {
        ESP_LOGE("PMS7003", "Failed to set Active Mode: 0x%x", err);
        vTaskDelete(NULL);
        return err;
    }

    while (1)
    {
        err = pms7003_readData(indoor, &pm1_0, &pm2_5, &pm10);
        if (err == ESP_OK)
        {
            ESP_LOGI("PMS7003", "PM1.0: %lu µg/m³, PM2.5: %lu µg/m³, PM10: %lu µg/m³", pm1_0, pm2_5, pm10);
            Pm1_0 = pm1_0;
            Pm2_5 = pm2_5;
            Pm10 = pm10;
        }
        else
        {
            ESP_LOGW("PMS7003", "Failed to read data: 0x%x", err);
        }

        vTaskDelay(pdMS_TO_TICKS(5000)); // Đọc mỗi 5 giây ...
    }
    return ESP_OK;
}

