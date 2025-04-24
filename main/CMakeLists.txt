idf_component_register(SRCS "sdcard_test.c" "mhz14a_test.c" "DS3231_test.c" "bme280_test.c" "main.c" "pms7003_test.c"
                    INCLUDE_DIRS "."
                    REQUIRES BME280 i2cdev driver PMS7003 MH_Z14A FileManager DS3231 Time)

