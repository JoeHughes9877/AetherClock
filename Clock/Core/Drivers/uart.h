#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_write(const char *str);
void uart_write_byte(uint8_t byte);

#endif