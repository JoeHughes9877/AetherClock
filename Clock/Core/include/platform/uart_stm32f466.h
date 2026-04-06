#ifndef UART_STM32F466_H
#define UART_STM32F466_H
#include "stm32f4xx_hal.h"

//Makes it visable to higher layers
extern UART_HandleTypeDef huart2;

//Ensure any file with the header can call it 
void platform_uart_init(void);

#endif