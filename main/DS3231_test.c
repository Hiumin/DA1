#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ds3231.h>
#include <string.h>
#include "DS3231Time.h"
#include "driver/i2c.h"

#define CONFIG_I2C_MASTER_SDA GPIO_NUM_26
#define CONFIG_I2C_MASTER_SCL GPIO_NUM_27


i2c_dev_t ds3231;
#define I2C_PORT I2C_NUM_0
#define SDA_GPIO GPIO_NUM_26
#define SCL_GPIO GPIO_NUM_27
static const char *TAG = "ds3231_test";

char time_str[64];

esp_err_t ds3231_test(void *pvParameters) {
    esp_err_t ret = ds3231_initialize(&ds3231, I2C_PORT, SDA_GPIO, SCL_GPIO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize DS3231: 0x%x", ret);
        vTaskDelete(NULL);
        return ret;
    }
    printf("Initialize DS3231 successful!\n");

    while (1) {
        ret = ds3231_convertTimeToString(&ds3231, time_str, sizeof(time_str), 1);  // format: dd-mm-yyyy hh:mm:ss
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to convert time to string: 0x%x", ret);
            vTaskDelete(NULL);
            return ret;
        }
        ESP_LOGI(TAG, "⏰ Thời gian hiện tại: %s", time_str);
        vTaskDelay(pdMS_TO_TICKS(5000));  // delay 5 giây
    }
    return ESP_OK;
}