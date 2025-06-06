
/*----Kernel Includes----*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/*---- std includes ----*/
#include <stdio.h>
#include <stdint.h>

/*---- esp includes ----*/
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"

/*---- project includes ----*/
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "ssd1306.h"

static const char *TAG = "UART_COMMUNICATION";
static const char *I2C_TAG = "I2C_BUS_CONFIGURATION";

static esp_err_t drivers_setup();

int adcRawValue;
int voltage;
int temperature;

lv_disp_t *disp = NULL;


void app_main(){
    ESP_ERROR_CHECK(drivers_setup());
    ESP_LOGI(TAG, "DRIVERS SETTED UP CORRECTLY");

    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);
    
    //  First time to create UI
    if (lvgl_port_lock(0)) {
        example_lvgl_demo_ui(disp);
        lvgl_port_unlock();
    }

    while(1){
        if(adc_lecture(&adcRawValue) == ESP_OK){
            voltage = 3300 * adcRawValue / 4096;
            temperature = voltage / 10;

            ESP_LOGI(TAG, "Raw ADC: %d, Voltage: %d mV, Temp: %d ºC", adcRawValue, voltage, temperature);

            // Update Text in UI
            if (lvgl_port_lock(0)) {
                update_adc_label(voltage, temperature);
                lvgl_port_unlock();
            }
           vTaskDelay(pdMS_TO_TICKS(1000));
            if (lvgl_port_lock(0)) {
                update_generic_label("PWM: ");
                lvgl_port_unlock();
            }

        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


static esp_err_t drivers_setup(){
    
    uart_config_init();
    adc_config_init();
    i2c_master_init();
    ssd1306_init();
    lvgl_init(&disp);

    return ESP_OK;

}