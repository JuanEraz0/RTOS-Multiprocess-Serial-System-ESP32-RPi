
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


/*---- project includes ----*/
#include "uart.h"
#include "adc.h"
#include "pwm.h"
#include "ssd1306.h"

static const char *TAG = "UART_COMMUNICATION";

static esp_err_t drivers_setup();

int adcRawValue;
int voltage;
int temperature;


void app_main(){
    
    ESP_ERROR_CHECK(drivers_setup());
    ESP_LOGI(TAG, "DRIVERS SETTED UP CORRECTLY");
    while(1){
        if(adc_lecture(&adcRawValue) == ESP_OK){
            voltage = 3300*adcRawValue/4096;
            temperature = voltage/10;
            ESP_LOGI(TAG, "Raw ADC lecture: %d, Voltage: %d mV, Temperature (ºC) %d", adcRawValue, voltage, temperature);
        } 
        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}

static esp_err_t drivers_setup(){
    
    uart_config_init();
    adc_config_init();
    //i2c_master_init();
    ssd1306_init();

    return ESP_OK;

}