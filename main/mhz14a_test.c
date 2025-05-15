#include <stdio.h>
#include "esp_log.h"
#include "mhz14a.h" 

uint32_t co2;

void mhz14a_task(void *pvParameters) {
    uint32_t *co2_t = malloc(sizeof(uint32_t));
    uart_config_t mhz14a_uart_config = MHZ14A_UART_CONFIG_DEFAULT();
    ESP_ERROR_CHECK_WITHOUT_ABORT(mhz14a_initUART(&mhz14a_uart_config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(mhz14a_setRangeSetting(co2_range0To5000));
    while (1){
        if(mhz14a_getDataFromSensorViaUART(co2_t) != ESP_OK){
            ESP_LOGE(__func__, "Failed to get data from MH-Z14A sensor.");
        } else {
            ESP_LOGI(__func__, "CO2 concentration: %lu ppm", *co2_t);
            co2 = *co2_t; // Lưu giá trị CO2 vào biến toàn cục
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Đợi 5 giây trước khi đọc lại
    };
}

