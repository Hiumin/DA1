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
#include "ds3231.h"
#include "DS3231Time.h"

//SDCARD
extern void init_sdcard(void);
extern void write_file(const char *nameFile, char *context);
extern void read_file(const char *nameFile);
extern void delete_file(const char *nameFile);
extern void rename_file(const char *oldNameFile, char *newNameFile);


//PM7003
extern void pms7003_task(void *pvParameters);

//MH-Z14A
extern void mhz14a_task(void *pvParameters);

//BME280
// extern void bme280_task(void *pvParameters);

//DS3231
extern void ds3231_test(void *pvParameters);

void app_main(void)
{
    init_sdcard();

    write_file("test", "abc");
    read_file("test");
    // rename_file("test", "test1");
    // delete_file("test1");    
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreate(ds3231_test, "ds3231_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);

    //PMS7003
    xTaskCreate(pms7003_task, "pms7003_task", 4096, NULL, 5, NULL);
    // BME280
    // xTaskCreate(bme280_task, "bme280_task", 4096, NULL, 5, NULL);
    // MH-Z14A
    // xTaskCreate(mhz14a_task, "mhz14a_task", 4096, NULL, 5, NULL);

    
}