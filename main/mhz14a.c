#include <stdio.h>
#include "esp_log.h"
#include "mhz14a.h" 

#define MHZ14A_UART_PORT UART_NUM_1
#define MHZ14A_TX_PIN    32  // Chọn GPIO 4 làm TX
#define MHZ14A_RX_PIN    33  // Chọn GPIO 5 làm RX

void init_mhz14a_sensor(void) {

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };

    // Khởi tạo UART cho MH-Z14A
    if (mhz14a_initUART(NULL) != ESP_OK) {
        ESP_LOGE("MH_Z14A", "Loi khi khoi tao UART cho MHZ14a.");
        return;
    }
    ESP_LOGI("MH_Z14A", "UART cho MHZ14a da duoc khoi tao.");

    uart_set_pin(MHZ14A_UART_PORT, MHZ14A_TX_PIN, MHZ14A_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

void read_mhz14a_data(void) {
    uint8_t co2_ppm;

    while (1) {
        if (mhz14a_getDataFromSensorViaUART(&co2_ppm) == ESP_OK) {
            ESP_LOGI("MH_Z14A", "Nong do CO2: %d ppm", co2_ppm);
        } else {
            ESP_LOGE("MH_Z14A", "Khong the doc du lieu tu MH-Z14A");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));// Đọc mỗi 5 giây
    }
}

