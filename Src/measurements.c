#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

volatile uint16_t bat_1, bat_2, temp_1, temp_2;
uint8_t meas_pending = 0;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern void display_values(uint16_t bat_1, uint16_t bat_2, uint16_t temp_1, uint16_t temp_2);

void StartMeasureTask(void const * argument) {
	ADC_ChannelConfTypeDef adc1_ch;
	ADC_ChannelConfTypeDef adc2_ch;

	while (1) {
		while (!meas_pending) {
			osDelay(10);
		}

		/* Update ADC1 values*/
		adc1_ch.Channel = ADC_CHANNEL_0;
		adc1_ch.Rank = 1;
		adc1_ch.SamplingTime = 12;
		adc1_ch.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc1, &adc1_ch) == HAL_OK) {
			if (HAL_ADC_Start(&hadc1) == HAL_OK) {
			}
		}

		/* Update ADC1 values*/
		adc2_ch.Channel = ADC_CHANNEL_2;
		adc2_ch.Rank = 1;
		adc2_ch.SamplingTime = 12;
		adc2_ch.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc2, &adc2_ch) == HAL_OK) {
			if (HAL_ADC_Start(&hadc2) == HAL_OK) {
			}
		}

		osDelay(1);
		bat_1 = HAL_ADC_GetValue(&hadc1);
		temp_1 = HAL_ADC_GetValue(&hadc2);

		/* Update ADC1 values*/
		adc1_ch.Channel = ADC_CHANNEL_1;
		adc1_ch.Rank = 1;
		adc1_ch.SamplingTime = 12;
		adc1_ch.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc1, &adc1_ch) == HAL_OK) {
			if (HAL_ADC_Start(&hadc1) == HAL_OK) {
			}
		}

		/* Update ADC1 values*/
		adc2_ch.Channel = ADC_CHANNEL_3;
		adc2_ch.Rank = 1;
		adc2_ch.SamplingTime = 12;
		adc2_ch.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc2, &adc2_ch) == HAL_OK) {
			if (HAL_ADC_Start(&hadc2) == HAL_OK) {
			}
		}
		osDelay(1);
		bat_2 = HAL_ADC_GetValue(&hadc1);
		temp_2 = HAL_ADC_GetValue(&hadc2);

		meas_pending = 0;

		display_values(bat_1, bat_2, temp_1, temp_2);
	}
}
