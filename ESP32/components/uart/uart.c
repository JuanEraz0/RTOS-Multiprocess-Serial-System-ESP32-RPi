#include "uart.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#define UART_PORT (UART_NUM_0)
#define UART_TX_PIN (GPIO_NUM_6)
#define UART_RX_PIN (GPIO_NUM_5)
#define RTS (UART_PIN_NO_CHANGE)
#define CTS (UART_PIN_NO_CHANGE)
#define UART_BAUD_RATE (115200)
#define BUF_SIZE (1024)




void uart_config_init(void){
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB
    };

    uart_driver_install(UART_PORT, BUF_SIZE * 2, BUF_SIZE * 2,0,NULL, 0);
    uart_param_config(UART_PORT, &uart_config);

    uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, RTS, CTS);
    
}