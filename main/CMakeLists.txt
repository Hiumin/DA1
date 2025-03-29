idf_component_register(SRCS "mhz14a.c" "main.c" "pms7003.c" "bme280.c"
                    INCLUDE_DIRS "."
                    REQUIRES BME280 i2cdev driver PMS7003 MH_Z14A FileManager)

