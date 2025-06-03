
/*----Kernel Includes----*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/*---- std includes ----*/
#include <stdio.h>
#include <stdint.h>

/*---- esp includes ----*/
#include "esp_log.h"

/*---- project includes ----*/
#include "uart.h"

static const char *TAG = "UART_COMMUNICATION";



static esp_err_t drivers_setup();



void app_main(){
    
    ESP_ERROR_CHECK(drivers_setup());
    ESP_LOGI(TAG, "DRIVERS SETTED UP CORRECTLY");
    while(1){
        ESP_LOGI(TAG, "HELLO FROM ESP32S3 UART");
        vTaskDelay(pdMS_TO_TICKS(100));
    }


}

static esp_err_t drivers_setup(){
    
    uart_config_init();

    return ESP_OK;

}