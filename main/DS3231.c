#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds3231.h>
#include <string.h>
#include "DS3231Time.h"

#define CONFIG_I2C_MASTER_SDA GPIO_NUM_21
#define CONFIG_I2C_MASTER_SCL GPIO_NUM_22

// void ds3231_test(void *pvParameters)
// {
//     i2c_dev_t dev;
//     memset(&dev, 0, sizeof(i2c_dev_t));

//     ESP_ERROR_CHECK(ds3231_init_desc(&dev, 0, CONFIG_I2C_MASTER_SDA, CONFIG_I2C_MASTER_SCL));

//     // setup datetime: 2016-10-09 13:50:10
//     struct tm time = {
//         .tm_year = 2025 - 1900, //since 1900 (2016 - 1900)
//         .tm_mon  = 3,  // 0-based
//         .tm_mday = 14,
//         .tm_hour = 23,
//         .tm_min  = 57,
//         .tm_sec  = 10
//     };
//     ESP_ERROR_CHECK(ds3231_set_time(&dev, &time));

//     vTaskDelay(pdMS_TO_TICKS(2000));

//     while (1)
//     {
//         float temp;

//         vTaskDelay(pdMS_TO_TICKS(1000));

//         if (ds3231_get_temp_float(&dev, &temp) != ESP_OK)
//         {
//             printf("Could not get temperature\n");
//             continue;
//         }

//         if (ds3231_get_time(&dev, &time) != ESP_OK)
//         {
//             printf("Could not get time\n");
//             continue;
//         }
//         printf("%04d-%02d-%02d %02d:%02d:%02d, %.2f deg Cel\n", time.tm_year /*Add 1900 for better readability*/, time.tm_mon + 1,
//             time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec, temp);
//     }
// }

i2c_dev_t ds3231;
#define I2C_PORT I2C_NUM_0
#define SDA_GPIO GPIO_NUM_26
#define SCL_GPIO GPIO_NUM_27
static const char *TAG = "ds3231_test";
void ds3231_test(void *pvParameters) {
    esp_err_t ret = ds3231_initialize(&ds3231, I2C_PORT, SDA_GPIO, SCL_GPIO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Khởi tạo DS3231 thất bại!");
        vTaskDelete(NULL);
        return;
    }

    // Gán thời gian ban đầu nếu cần (chỉ bật khi cần set thời gian)
    /*
    struct tm init_time = {
        .tm_year = 2025 - 1900,
        .tm_mon  = 4 - 1,
        .tm_mday = 17,
        .tm_hour = 20,
        .tm_min  = 45,
        .tm_sec  = 0
    };
    ds3231_setTime(&ds3231, &init_time);
    ESP_LOGI(TAG, "Đã đặt lại thời gian");
    */

    char time_str[64];

    while (1) {
        // Lấy và hiển thị thời gian hiện tại
        ds3231_convertTimeToString(&ds3231, time_str, sizeof(time_str), 1);  // format: dd-mm-yyyy hh:mm:ss
        ESP_LOGI(TAG, "⏰ Thời gian hiện tại: %s", time_str);


        // Kiểm tra nếu có ngày mới

        // Lấy và in epoch time


        vTaskDelay(pdMS_TO_TICKS(1000));  // delay 1 giây
    }

    // Không bao giờ đến đây, nhưng phòng ngừa
    vTaskDelete(NULL);
}