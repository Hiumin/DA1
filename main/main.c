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
extern esp_err_t init_sdcard(void);
extern esp_err_t write_file(const char *nameFile, char *context);
extern esp_err_t read_file(const char *nameFile);
extern esp_err_t delete_file(const char *nameFile);
extern esp_err_t rename_file(const char *oldNameFile, char *newNameFile);


//PM7003
extern uint32_t Pm1_0, Pm2_5, Pm10;
extern esp_err_t pms7003_task(void *pvParameters);

//MH-Z14A
extern uint32_t co2;
extern esp_err_t mhz14a_task(void *pvParameters);

//BME280
extern float temperature, pressure, humidity;
extern esp_err_t bme280_task(void *pvParameters);

//DS3231
extern i2c_dev_t ds3231;
extern char time_str[64];
extern esp_err_t ds3231_test(void *pvParameters);



void save_data_to_sdcard(void *pvParameters){
    char namefile[64];
    // Lấy thời gian hiện tại cho tên file
    
    while(1){
        // Ghi dữ liệu vào file
        char sensor_data[128];
        ds3231_convertTimeToString(&ds3231, namefile, sizeof(namefile), 5);
        sprintf(sensor_data, "%s,%.2f,%.2f,%.2f,%lu,%lu,%lu,%lu",time_str,temperature,humidity,pressure / 100.0,Pm1_0, Pm2_5, Pm10,co2);
        write_file(namefile, sensor_data);
        vTaskDelay(pdMS_TO_TICKS(5000)); // Đọc mỗi 5 giây
    }
}

void app_main(void)
{
    // SDCARD
    init_sdcard();
    //read_file("test");
    //rename_file("test", "test1");
    //delete_file("test1"); 

    ESP_ERROR_CHECK(i2cdev_init());

    // DS3231
    xTaskCreate(ds3231_test, "ds3231_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
  
    //PMS7003
    xTaskCreate(pms7003_task, "pms7003_task", 4096, NULL, 5, NULL);

    // BME280
    xTaskCreate(bme280_task, "bme280_task", 4096, NULL, 5, NULL);

    // MH-Z14A
    xTaskCreate(mhz14a_task, "mhz14a_task", 4096, NULL, 5, NULL);

    //Luu dữ liệu vào thẻ SD
    xTaskCreate(save_data_to_sdcard, "save_data_to_sdcard", 4096, NULL, 5, NULL);
}