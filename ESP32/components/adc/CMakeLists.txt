idf_component_register(SRCS "adc.c"
                      INCLUDE_DIRS "."
                       REQUIRES driver
                       REQUIRES esp_adc)

                       