#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_mac.h"
#include "bme280.h"
#include "sdcard.h"

void bme280_task(void *pvParameters) {
    bmp280_t bme280_device;
    bmp280_params_t bme280_params;
    float temperature, pressure, humidity;
    ESP_ERROR_CHECK_WITHOUT_ABORT(i2cdev_init());
    ESP_LOGI(__func__, "Initialize BME280 sensor(I2C/Wire%d).", CONFIG_BME_I2C_PORT);

    ESP_ERROR_CHECK_WITHOUT_ABORT(bme280_init(&bme280_device, &bme280_params, BME280_ADDRESS,
                                              CONFIG_BME_I2C_PORT, CONFIG_BME_PIN_NUM_SDA, CONFIG_BME_PIN_NUM_SCL));
    
    
    while(1){
        if (bme280_readSensorData(&bme280_device, &temperature, &pressure, &humidity) == ESP_OK) {
            ESP_LOGI("BME280", "Nhiet do: %.2f °C, Ap suat: %.2f hPa, Do am: %.2f%%", 
                     temperature, pressure / 100.0, humidity);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Đọc mỗi 5 giây
    }
}
