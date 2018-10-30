#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"

extern osMessageQId displayQueueHandle;


status_t status;
uint8_t meas_pending = 0;
uint32_t mdata = 0;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

#define VOLTAGE_DIV 24/4; /* Volts * 1000 */
#define TEMP_DIV 300/100;   /* Temp * 100 */

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
		status.bat_1 = HAL_ADC_GetValue(&hadc1) * VOLTAGE_DIV;
		status.temp_1 = HAL_ADC_GetValue(&hadc2) * TEMP_DIV;
		if (status.temp_1 > status.temp_1_max) status.temp_1_max = status.temp_1;
		if (status.temp_1 < status.temp_1_min) status.temp_1_min = status.temp_1;
		if (!status.temp_1_min) status.temp_1_min = status.temp_1;

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
		status.bat_2 = HAL_ADC_GetValue(&hadc1) * VOLTAGE_DIV;
		status.temp_2 = HAL_ADC_GetValue(&hadc2) * TEMP_DIV;
		if (status.temp_2 > status.temp_2_max) status.temp_2_max = status.temp_2;
		if (status.temp_2 < status.temp_2_min) status.temp_2_min = status.temp_2;
		if (!status.temp_2_min) status.temp_2_min = status.temp_2;

		/* Update ADC1 values (ch 13 - solar)*/
		adc1_ch.Channel = ADC_CHANNEL_13;
		adc1_ch.Rank = 1;
		adc1_ch.SamplingTime = 12;
		adc1_ch.Offset = 0;
		if (HAL_ADC_ConfigChannel(&hadc1, &adc1_ch) == HAL_OK) {
			if (HAL_ADC_Start(&hadc1) == HAL_OK) {
			}
		}
		osDelay(1);
		status.solar = HAL_ADC_GetValue(&hadc1) * VOLTAGE_DIV;

		status.time++;

		osMessagePut( displayQueueHandle, &mdata, 100);

		meas_pending = 0;
	}
}
