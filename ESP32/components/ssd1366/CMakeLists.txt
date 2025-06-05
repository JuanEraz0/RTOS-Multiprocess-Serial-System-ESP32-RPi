idf_component_register(SRCS "ssd1366.c"
                      INCLUDE_DIRS "."
                       REQUIRES driver
                       REQUIRES esp_driver_i2c)

                       