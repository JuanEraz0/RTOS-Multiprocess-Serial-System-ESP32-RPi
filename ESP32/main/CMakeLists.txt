idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES lvgl uart adc pwm ssd1306 touch driver interrupt )
