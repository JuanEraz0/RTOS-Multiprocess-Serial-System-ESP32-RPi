#include "interrupt.h"
#include "esp_log.h"
static gpio_config_t gpioIrqConfig;

static const char *INT_TAG = "INTERRUPT";

esp_err_t init_isr(void){
    gpioIrqConfig.pin_bit_mask = (1ULL << IRQ_BUTTON);
    gpioIrqConfig.mode = GPIO_MODE_DEF_INPUT;
    gpioIrqConfig.pull_up_en = GPIO_PULLUP_ENABLE;
    gpioIrqConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpioIrqConfig.intr_type = GPIO_INTR_NEGEDGE;

    ESP_ERROR_CHECK(gpio_config(&gpioIrqConfig));

    gpio_install_isr_service(0);
    gpio_isr_handler_add(IRQ_BUTTON, isr_handler, NULL);

    return ESP_OK;
}
void isr_handler(void *){
    //ESP_LOGI(INT_TAG, "Hardware interrupt via PUSH BUTTON");
    
}