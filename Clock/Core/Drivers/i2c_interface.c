#include "main.h"
#include "stm32f4xx_hal_i2c.h"
#include "i2c_interface.h"

extern I2C_HandleTypeDef hi2c1; 

void I2C_Interface_Write(uint8_t slave_addr, uint8_t data) {
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(slave_addr << 1), &data, 1, HAL_MAX_DELAY);
}