#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <stdio.h>
#include <driver/gpio.h>

#define IRQ_BUTTON GPIO_NUM_48

esp_err_t init_isr(void);
void isr_handler(void*);









#endif /* INTERRUPT_H */