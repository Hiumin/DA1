#include <stdio.h>
#include "pms7003.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <ds3231.h>
#include <string.h>
#include "DS3231Time.h"

// #define PMS_UART_PORT UART_NUM_2  // Cổng UART sử dụng
// #define PMS_PIN_TX (GPIO_NUM_16)  // Chân TX của ESP32
// #define PMS_PIN_RX (GPIO_NUM_17)  // Chân RX của ESP32

uint32_t Pm1_0, Pm2_5, Pm10;
void pms7003_task(void *pvParameters)
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
    if (pms7003_initUart(&uart_config) != ESP_OK)
    {
        ESP_LOGE("PMS7003", "Failed to initialize UART");
        vTaskDelete(NULL);  // Dừng Task nếu có lỗi
    }

    //Kích hoạt Active Mode
    if (pms7003_activeMode() != ESP_OK)
    {
        ESP_LOGE("PMS7003", "Failed to set Active Mode");
        vTaskDelete(NULL);
    }

    while (1)
    {
        if (pms7003_readData(indoor, &pm1_0, &pm2_5, &pm10) == ESP_OK)
        {
            ESP_LOGI("PMS7003", "PM1.0: %lu µg/m³, PM2.5: %lu µg/m³, PM10: %lu µg/m³", pm1_0, pm2_5, pm10);
            Pm1_0 = pm1_0;
            Pm2_5 = pm2_5;
            Pm10 = pm10;
        }
        else
        {
            ESP_LOGW("PMS7003", "Failed to read data");
        }

        vTaskDelay(pdMS_TO_TICKS(5000)); // Đọc mỗi 5 giây ...
    }
}

