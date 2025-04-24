#include <stdio.h>
#include "esp_log.h"
#include "mhz14a.h" 

#define CONFIG_MHZ14A_PWM_PIN    32  
#define CONFIG_HD_PIN    33  

uart_config_t mhz14a_uart_config = MHZ14A_UART_CONFIG_DEFAULT(); // Configuration for MH-Z14A uart port



void read_mhz14a_data(void) {
    uint32_t *co2_t = malloc(sizeof(uint32_t));
    ESP_ERROR_CHECK_WITHOUT_ABORT(mhz14a_initUART(&mhz14a_uart_config));
    ESP_ERROR_CHECK_WITHOUT_ABORT(mhz14a_setRangeSetting(co2_range0To5000));
    while (mhz14a_getDataFromSensorViaUART(&co2_t) != ESP_OK)
        ;
}

