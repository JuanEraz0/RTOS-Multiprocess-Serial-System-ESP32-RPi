#include "adc.h"
#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

#define ADC_CHANNEL      ADC_CHANNEL_9
#define ADC_ATTENUATION  ADC_ATTEN_DB_12

static adc_oneshot_unit_handle_t adc_handle;
static adc_cali_handle_t calibration_handle;

static bool adc_calibration_init(){
    adc_cali_curve_fitting_config_t calibration_config = {
        .unit_id = ADC_UNIT_2,
        .atten = ADC_ATTENUATION,
        .bitwidth = ADC_BITWIDTH_12
    };
    
    return adc_cali_create_scheme_curve_fitting(&calibration_config, &calibration_handle) == ESP_OK;
}

void adc_config_init(void){
    adc_oneshot_unit_init_cfg_t config_adc = {
        .unit_id = ADC_UNIT_2,
    };
    adc_oneshot_new_unit(&config_adc, &adc_handle);

    adc_oneshot_chan_cfg_t config_channel = {
        .atten = ADC_ATTENUATION,
        .bitwidth = ADC_BITWIDTH_12,
    };
    adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config_channel);

    if (!adc_calibration_init()) {
        printf("Error inicializando calibración del ADC\n");
    }
}

esp_err_t adc_lecture(int *adc_raw_value){
    return adc_oneshot_read(adc_handle, ADC_CHANNEL, adc_raw_value);
}
