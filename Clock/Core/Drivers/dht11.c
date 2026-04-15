#include "dht11.h"
#include <string.h>

uint8_t dht11_data[5];
int8_t bit_index = 0;
DHT_State dht_state = DHT_IDLE;

extern TIM_HandleTypeDef htim3;

uint16_t dht_humidity = 0;
uint16_t dht_temperature = 0;

void DHT_init(void) {
    HAL_TIM_Base_Start_IT(&htim3);
    HAL_Delay(1000); 
}

void DHT_Start(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = dht11_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);

    HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_RESET);
    HAL_Delay(18);

    HAL_GPIO_WritePin(dht11_GPIO_Port, dht11_Pin, GPIO_PIN_SET);
    for(volatile int i=0; i<100; i++); 

    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP; 
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(dht11_GPIO_Port, &GPIO_InitStruct);

    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
}

void DHT_Read(void) {
    bit_index = -2; 
    memset(dht11_data, 0, sizeof(dht11_data));
    dht_state = DHT_RECEIVE;
    DHT_Start(); 
}

void DHT_Get_Results(void) {
    uint8_t check = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3];
    if (check == dht11_data[4] && check > 0) {
        dht_humidity = (dht11_data[0] * 10) + dht11_data[1];
        dht_temperature = (dht11_data[2] * 10) + dht11_data[3];
    }
}

void DHT_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (dht_state != DHT_RECEIVE) return;

    uint32_t pulse_width = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    __HAL_TIM_SET_COUNTER(htim, 0);

    // Bit_index 0 to 39 are the actual 40 data bits
    if (bit_index >= 0 && bit_index < 40) {
        dht11_data[bit_index / 8] <<= 1;
        if (pulse_width > DHT_BIT_THRESHOLD) {
            dht11_data[bit_index / 8] |= 1;
        }
    }
    
    bit_index++;

    if (bit_index >= 40) {
        HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
        DHT_Get_Results();
        dht_state = DHT_IDLE;
    }
}