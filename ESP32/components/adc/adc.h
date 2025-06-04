#ifndef ADC_H
#define ADC_H

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"


void adc_config_init(void);
esp_err_t adc_lecture(int *adc_raw_value);



#endif // ADC_H

