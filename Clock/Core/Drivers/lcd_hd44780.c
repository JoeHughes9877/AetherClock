#include "lcd_hd44780.h" 
#include "i2c_interface.h"
#include "main.h"
#include <stdint.h>

void LCD_Init() {
    HAL_Delay(50); //wake up 

    // Set 4-bit mode
    LCD_Write_Nibble(0x03, 0);
    HAL_Delay(5);
    LCD_Write_Nibble(0x03, 0);
    HAL_Delay(1);
    LCD_Write_Nibble(0x03, 0);
    HAL_Delay(1);

    LCD_Write_Nibble(0x02, 0); 
    HAL_Delay(1);

    // 4 bit mode in 2 lines 5x8 dots
    LCD_Send_Byte(0x28, 0);
    HAL_Delay(1);
    LCD_Send_Byte(0x0C, 0); 
    HAL_Delay(1);
    LCD_Send_Byte(0x06, 0); 
    HAL_Delay(1);
    
    LCD_Clear();
}

void LCD_Clear() {
    LCD_Send_Byte(0x01, 0); // Clear display command
    HAL_Delay(2); // Delay for clear command
}

void LCD_Send_String(const char *str) {
    while (*str) {
        LCD_Send_Byte(*str++, 1); 
    }
}

void LCD_Send_Byte(uint8_t byte, uint8_t rs) {
    LCD_Write_Nibble(byte >> 4, rs); // Send higher nibble
    LCD_Write_Nibble(byte & 0x0F, rs); // Send lower nibble
}

void LCD_Write_Nibble(uint8_t nibble, uint8_t rs) {
    uint8_t data = (nibble << 4) | rs | BACKLIGHT | PIN_EN;
    I2C_Interface_Write(LCD_ADDR, data);
    HAL_Delay(1); // Delay for pulse

    //turn off EN bit
    data &= ~PIN_EN; // Clear EN bit
    I2C_Interface_Write(LCD_ADDR, data);
    HAL_Delay(1); // Delay for pulse
}
    