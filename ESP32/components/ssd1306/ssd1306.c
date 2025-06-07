#include <stdio.h>
#include "ssd1306.h"


i2c_master_bus_handle_t i2c_bus = NULL;
esp_lcd_panel_io_handle_t io_handle = NULL;
esp_lcd_panel_handle_t panel_handle = NULL;
static lv_obj_t *label_adc = NULL;
const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();

static void gpio36_init(void){
    /*
    *----Function to set-up correctly HELTEC-ESP32S3 embedded i2c OLED display
    *----Vext PIN (gpio36) needs to get LOW before starts I2C bus 
    */
    gpio_config_t gpio_conf = {
        .pin_bit_mask = 1ULL << HVEX_PIN_NUM ,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&gpio_conf);
    gpio_set_level(HVEX_PIN_NUM , 0);

}

void i2c_master_init(void){
    
    gpio36_init(); //SET HVEX_PIN (36) -> LOW
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .i2c_port = I2C_BUS_PORT,
        .sda_io_num = PIN_NUM_SDA,
        .scl_io_num = PIN_NUM_SCL,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));
}



void ssd1306_init(void){
   
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_HW_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,               // According to SSD1306 datasheet
        .lcd_cmd_bits = LCD_CMD_BITS,   // According to SSD1306 datasheet
        .lcd_param_bits = LCD_CMD_BITS, // According to SSD1306 datasheet
        .dc_bit_offset = 6, 

    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));
    
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = PIN_NUM_RST,
    };
        esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    

    lvgl_port_init(&lvgl_cfg);

        const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_H_RES * LCD_V_RES,
        .double_buffer = true,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);

    /* Rotation of the screen */
    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);
}

void lvgl_init(lv_disp_t **lvglDisp){
    
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_H_RES * LCD_V_RES,
        .double_buffer = true,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };

    *lvglDisp = lvgl_port_add_disp(&disp_cfg);
}

void example_lvgl_demo_ui(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    label_adc = lv_label_create(scr); 
    lv_label_set_long_mode(label_adc, LV_LABEL_LONG_WRAP);
    lv_label_set_text(label_adc, "Iniciando...");
    lv_obj_set_width(label_adc, disp->driver->hor_res);
    lv_obj_align(label_adc, LV_ALIGN_TOP_MID, 0, 0);
}

void update_adc_label(int voltage, int temperature)
{
    if(label_adc) {
        char buf[64];
        snprintf(buf, sizeof(buf), "VoltajeADC: %d mV\nTemperatura: %d C", voltage, temperature);
        lv_label_set_text(label_adc, buf);
    }
}


void update_servo_label(int servo_angle)
{
    if(label_adc) {
        char buf[64];
        snprintf(buf, sizeof(buf), "Servo Angle: %d ", servo_angle);
        lv_label_set_text(label_adc, buf);
    }
}


void update_generic_label(void * genericData){

    lv_label_set_text(label_adc, genericData);
    
}
