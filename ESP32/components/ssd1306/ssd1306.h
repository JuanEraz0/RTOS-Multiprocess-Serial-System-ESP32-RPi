#ifndef SS1306_H
#define SS1306_H

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lvgl_port.h"
#include "driver/gpio.h" 
#include "esp_lcd_panel_vendor.h"



#define I2C_BUS_PORT  1

#define LCD_PIXEL_CLOCK_HZ    (500000)
#define PIN_NUM_SDA           GPIO_NUM_17
#define PIN_NUM_SCL           GPIO_NUM_18
#define PIN_NUM_RST           GPIO_NUM_21
#define I2C_HW_ADDR           0x3C
#define HVEX_PIN_NUM           GPIO_NUM_36
#define LCD_H_RES     128
#define LCD_V_RES     64
// Bit number used to represent command and parameter
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8



void i2c_master_init(void);

void ssd1306_init(void);

void lvgl_init( lv_disp_t **lvglDisp);

void example_lvgl_demo_ui(lv_disp_t *disp);
void update_adc_label(int voltage, int temperature);







#endif //SS1306_H
