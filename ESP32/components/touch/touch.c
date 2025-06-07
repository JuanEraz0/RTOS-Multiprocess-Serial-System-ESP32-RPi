#include "touch.h"



void touch_init(void){
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_config(1, TOUCH_THRESH_USE);
    touch_pad_config(1, TOUCH_THRESH_USE);
    touch_pad_config(1, TOUCH_THRESH_USE);
    touch_pad_config(1, TOUCH_THRESH_USE);
    touch_pad_config(1, TOUCH_THRESH_USE);
    touch_pad_config(1, TOUCH_THRESH_USE);
}

void servo_touch_read(void){

}

void dcmotor_touch_read(void){
    
}









