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

//SDCARD
ESP_LOGI(__func__, "Initialize SD card with SPI interface.");
    esp_vfs_fat_mount_config_t mount_config_t = MOUNT_CONFIG_DEFAULT();
    spi_bus_config_t spi_bus_config_t_1 = SPI_BUS_CONFIG_DEFAULT();
    sdmmc_host_t host_t = SDSPI_HOST_DEFAULT();
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = CONFIG_PIN_NUM_CS;
    slot_config.host_id = host_t.slot;

    sdmmc_card_t SDCARD;
#define FILE_NAME "test.txt"
extern esp_err_t sdcard_initialize(esp_vfs_fat_sdmmc_mount_config_t *_mount_config, sdmmc_card_t *_sdcard,
                            sdmmc_host_t *_host, spi_bus_config_t *_bus_config, sdspi_device_config_t *_slot_config);
extern esp_err_t sdcard_writeDataToFile(const char *nameFile, const char *format, ...);
extern esp_err_t sdcard_readDataToFile(const char *nameFile, const char *format, ...);

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
    //SDCARD
    sdcard_initialize(&mount_config_t, &SDCARD, &host_t, &spi_bus_config_t_1, &slot_config);
    //PMS7003
    xTaskCreate(pms7003_task, "pms7003_task", 4096, NULL, 5, NULL);
    
    //BME280
    bme280_check_connection();
    read_and_save_bme280_data();

    //MH-Z14A
    init_mhz14a_sensor();
    read_mhz14a_data();
}
// #include "pms7003.h"