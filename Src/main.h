#ifndef MAIN_H_
#define MAIN_H_
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

typedef struct status_s {
	volatile uint16_t bat_1;
	volatile uint16_t bat_2;
	volatile uint16_t solar;
	volatile uint16_t temp_1;
	volatile uint16_t temp_2;
	volatile uint16_t temp_3;
	unsigned char bridge;
}status_t;

#endif
