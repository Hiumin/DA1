#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "bme280.h"
#include "mhz14a.h"
#include "pms7003.h"
#include "esp_mac.h"
#include "sdcard.h"

//PM7003
extern void pms7003_task(void *pvParameters);

//MH-Z14A
extern void init_mhz14a_sensor();
extern void read_mhz14a_data();

//BME280
extern esp_err_t bme280_check_connection();
extern void read_and_save_bme280_data(void);

void app_main(void)
{
    //PMS7003
    xTaskCreate(pms7003_task, "pms7003_task", 4096, NULL, 5, NULL);
    
    //BME280
    bme280_check_connection();
    read_and_save_bme280_data();

    //MH-Z14A
    init_mhz14a_sensor();
    read_mhz14a_data();
}
