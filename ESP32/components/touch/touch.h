#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>

#include "driver/touch_pad.h"
#include "driver/gpio.h"

#define N (500) //touch threshold
#define TOUCH_PAD_NO_CHANGE (-1)
#define TOUCH_THRESH_USE (20000)
#define TOUCH_FILTER_MODE_EN (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)



void touch_init(void);

void servo_touch_read(void);

void dcmotor_touch_read(void);