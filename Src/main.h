#ifndef MAIN_H_
#define MAIN_H_
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

typedef struct status_s {
	volatile uint16_t bat_1;
	volatile uint16_t bat_2;
	volatile uint16_t solar;
	volatile uint16_t temp_1;
	volatile uint16_t temp_1_min;
	volatile uint16_t temp_1_max;
	volatile uint16_t temp_2;
	volatile uint16_t temp_2_min;
	volatile uint16_t temp_2_max;
	volatile uint16_t temp_3;
	uint8_t bridge;
	uint32_t time;
}status_t;

#endif
