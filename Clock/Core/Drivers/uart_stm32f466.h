#ifndef UART_STM32F466_H
#define UART_STM32F466_H
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;

void platform_uart_init(void);
void platform_uart_write_byte(uint8_t byte);    

#endif