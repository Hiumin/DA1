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
    ESP_LOGI("SDcard", "Starting SD card initialization...");

    // Cấu hình bắt buộc
    slot_config.gpio_cs = PIN_NUM_CS;
    slot_config.host_id = host.slot;
    host.max_freq_khz = 400; // khởi tạo an toàn

    // Gọi hàm từ component sdcard
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

