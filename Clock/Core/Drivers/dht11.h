#ifndef DHT11_H
#define DHT11_H

#include "main.h"
#include <stdint.h>

typedef enum {
    DHT_IDLE = 0,
    DHT_RECEIVE
} DHT_State;

// 100us threshold splits the difference between '0' (78us) and '1' (120us)
#define DHT_BIT_THRESHOLD 100

/* Public variables */
extern uint8_t dht11_data[5];
extern int8_t bit_index;
extern DHT_State dht_state;

extern uint16_t dht_humidity;
extern uint16_t dht_temperature;

/* Functions */
void DHT_init(void);
void DHT_Read(void);
void DHT_Get_Results(void);
void DHT_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* DHT11_H */