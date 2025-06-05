#ifndef SS1306_H
#define SS1306_H




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









#endif //SS1306_H
