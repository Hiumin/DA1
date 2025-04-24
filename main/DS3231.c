#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds3231.h>
#include <string.h>
#include "DS3231Time.h"

#define CONFIG_I2C_MASTER_SDA GPIO_NUM_21
#define CONFIG_I2C_MASTER_SCL GPIO_NUM_22


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

    char time_str[64];

    while (1) {
        // Lấy và hiển thị thời gian hiện tại
        ds3231_convertTimeToString(&ds3231, time_str, sizeof(time_str), 5);  // format: dd-mm-yyyy hh:mm:ss
        ESP_LOGI(TAG, "⏰ Thời gian hiện tại: %s", time_str);


        vTaskDelay(pdMS_TO_TICKS(1000));  // delay 1 giây
    }

    // Không bao giờ đến đây, nhưng phòng ngừa
    vTaskDelete(NULL);
}