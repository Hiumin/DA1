#include <stdio.h>
#include "esp_log.h"
#include "sdcard.h"

// Cấu hình chân SPI
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

// Mount point
#define MOUNT_POINT "/sdcard"

esp_vfs_fat_sdmmc_mount_config_t mount_config = {
    .format_if_mount_failed = false,
    .max_files = 5,
    .allocation_unit_size = 16 * 1024
};

spi_bus_config_t spi_bus_cfg = {
    .mosi_io_num = PIN_NUM_MOSI,
    .miso_io_num = PIN_NUM_MISO,
    .sclk_io_num = PIN_NUM_CLK,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .max_transfer_sz = 4000
};

sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
sdmmc_card_t sdcard;

void init_sdcard(void)
{
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    host.max_freq_khz = 400; // Tốc độ tối đa của thẻ SD (400kHz)

    esp_err_t ret = sdcard_initialize(&mount_config, &sdcard, &host, &spi_bus_cfg, &slot_config);
    if (ret == ESP_OK)
    {
        ESP_LOGI("SDcard", "SD card initialized successfully.");
    }
    else
    {
        ESP_LOGE("SDcard", "Failed to initialize SD card.");
    }
}

void write_file(const char *nameFile, char *context)
{
    // Ghi dữ liệu vào file test.txt
    esp_err_t ret = sdcard_writeDataToFile(nameFile, "%s", context);    
    if (ret == ESP_OK)
    {
        ESP_LOGI("SDcard", "File written successfully.");
    }
    else
    {
        ESP_LOGE("SDcard", "Failed to write file.");
    }
}

void read_file(const char *nameFile)
{
    char data[128];
    esp_err_t ret = sdcard_readDataFromFile(nameFile, "%s", data);
    if (ret == ESP_OK)
    {
        ESP_LOGI("SDcard", "File content: %s", data);
    }
    else if (ret == ESP_ERR_NOT_FOUND)
    {
        ESP_LOGE("SDcard", "File not found.");
    }
    else
    {
        ESP_LOGE("SDcard", "Failed to read file.");
    }
}

void delete_file(const char *nameFile)
{
    esp_err_t ret = sdcard_removeFile(nameFile);
    if (ret == ESP_OK)
    {
        ESP_LOGI("SDcard", "File deleted successfully.");
    }
    else
    {
        ESP_LOGE("SDcard", "Failed to delete file.");
    }
}

void rename_file(const char *oldNameFile, char *newNameFile)
{
    esp_err_t ret = sdcard_renameFile(oldNameFile, newNameFile);
    
    if (ret == ESP_OK)
    {
        ESP_LOGI("SDcard", "File renamed successfully.");
    }
    else
    {
        ESP_LOGE("SDcard", "Failed to rename file.");
    }
}

