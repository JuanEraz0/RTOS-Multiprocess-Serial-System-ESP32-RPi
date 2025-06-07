idf_component_register(SRCS "pwm.c"
                      INCLUDE_DIRS "."
                      REQUIRES driver
                      REQUIRES esp_driver_mcpwm)
