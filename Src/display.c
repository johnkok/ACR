#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "images/images.h"
#include "main.h"

extern osMessageQId displayQueueHandle;
extern status_t status;

osEvent event;
#define BAT_1_X_OFFSET 16
#define BAT_2_X_OFFSET 122
#define BAT_3_X_OFFSET 229

uint8_t voltage2percent(uint16_t voltage) {

	if (voltage < 11000) {
		return 0;
	}
	if (voltage > 12800) {
		return 100;
	}
	voltage -= 11000;

	/**
	 * 11V -> 0%
	 * 12.8V -> 100%
	 * range 0 - 1800
	 */
	return voltage / 18;
}

void drawPoint(uint16_t X, uint16_t Y)
{

	/* Bridge pressed */
	if (X > 80 && X < 130 &&
		Y > 45 && Y < 75) {
		if (status.bridge) {
			status.bridge = 0;
			TM_ILI9341_DrawImage(86, 50, 40, 20, BOFF);
		}
		else {
			status.bridge = 1;
			TM_ILI9341_DrawImage(86, 50, 40, 20, BON);
		}
	}
	else if ( 	X > 0 && X < 40 &&
				Y > 50 && Y < 85) {
		   TM_ILI9341_DrawFilledRectangle(0, 55, 36, 85, ILI9341_COLOR_BLACK);
	}
	else if ( 	X > 45 && X < 75 &&
				Y > 200 && Y < 240) {
		   TM_ILI9341_DrawFilledRectangle(50, 204, 86, 234, ILI9341_COLOR_BLACK);
	}
	else
		TM_ILI9341_DrawCircle(X, Y, 10, ILI9341_COLOR_BLUE);
}


void StartDisplayTask(void const * argument)
{
    char buffer[16];
    uint8_t gauge_index;

    TM_ILI9341_Init();

    /* Cross */
    TM_ILI9341_DrawFilledRectangle(0, 120, 320, 121, ILI9341_COLOR_WHITE);
    TM_ILI9341_DrawFilledRectangle(106, 0, 107, 240*2, ILI9341_COLOR_WHITE);
    TM_ILI9341_DrawFilledRectangle(213, 0, 214, 240*2, ILI9341_COLOR_WHITE);

    /* Texts */
    TM_ILI9341_Puts(20,0,"Engine", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(145,00,"Acc.", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(245,00,"Solar", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    TM_ILI9341_Puts(20,122,"Engine", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(122,122,"Exhaust", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(235,122,"Status", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

    /* Batteries */
    TM_ILI9341_DrawImage(6, 88, 95, 30, BAT);
    TM_ILI9341_DrawImage(112, 88, 95, 30, BAT);
    TM_ILI9341_DrawImage(219, 88, 95, 30, BAT);

    /* Thermometers */
    TM_ILI9341_DrawImage(1, 145, 30, 90, THERM);
    TM_ILI9341_DrawImage(113, 145, 30, 90, THERM);
    TM_ILI9341_Puts(220,180,"Room temp:", &TM_Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
    TM_ILI9341_Puts(220,147,"Bridge:", &TM_Font_7x10, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

   for(;;)
   {
	   uint32_t data;
	   event = osMessageGet( displayQueueHandle, 0xFFFFFFFF);
	   data = *(uint32_t *)(event.value.v);

	   if (data){
           drawPoint(data & 0xFFFF, (data & 0xFFFF0000) >> 16);
	   }
	   else {
		   /********************/
		   /* Update bridge */
		   /********************/
		   snprintf(buffer, 16, "ON");
		   TM_ILI9341_Puts(255,160,buffer, &TM_Font_11x18, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);

		   /********************/
		   /* Update Room temp */
		   /********************/
		   snprintf(buffer, 16, "%2.1d,%1.1d~C",
				   status.temp_3 / 100 ,
				   (status.temp_3 % 100) / 10);
		   TM_ILI9341_Puts(235,195,buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   /***************/
		   /* Update Time */
		   /***************/
		   snprintf(buffer, 16, "%2.2d:%2.2d:%2.2d",
				   status.time / (60 * 60),
				   (status.time / 60) % 60,
				   status.time % 60);
		   TM_ILI9341_Puts(225,220,buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   /****************/
		   /* Update bat 1 */
		   /****************/
		   snprintf(buffer, 16, "%2.1d.%2.2dV", (status.bat_1 / 1000), (status.bat_1 % 1000)/10);
		   TM_ILI9341_Puts(1, 25, buffer, &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
		   snprintf(buffer, 16, "%3.1d%%", voltage2percent(status.bat_1));
		   TM_ILI9341_Puts(30, 60, buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   if (voltage2percent(status.bat_1) < 60)
			   TM_ILI9341_DrawImage(0, 55, 36, 30, ERR); // error

           TM_ILI9341_DrawImage(6, 88, 95, 30, BAT);
		   if (voltage2percent(status.bat_1) > 10)
		       TM_ILI9341_DrawImage(BAT_1_X_OFFSET, 90, 7, 26, 1);
		   if (voltage2percent(status.bat_1) > 20)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 8, 90, 7, 26, 1);
		   if (voltage2percent(status.bat_1) > 30)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 16, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_1) > 40)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 24, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_1) > 50)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 32, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_1) > 60)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 40, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_1) > 70)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 48, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_1) > 80)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 56, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_1) > 90)
			   TM_ILI9341_DrawImage(BAT_1_X_OFFSET + 64, 90, 7, 26, 3);

		   /****************/
		   /* Update Solar */
		   /****************/
		   snprintf(buffer, 16, "%2.1d.%2.2dV", (status.solar / 1000), (status.solar % 1000)/10);
		   TM_ILI9341_Puts(220, 25, buffer, &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
		   snprintf(buffer, 16, "%3.1d%%", voltage2percent(status.solar*2/3));
		   TM_ILI9341_Puts(245, 60, buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

           TM_ILI9341_DrawImage(219, 88, 95, 30, BAT);
		   if (voltage2percent(status.solar*2/3) > 10)
		       TM_ILI9341_DrawImage(BAT_3_X_OFFSET, 90, 7, 26, 1);
		   if (voltage2percent(status.solar*2/3) > 20)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 8, 90, 7, 26, 1);
		   if (voltage2percent(status.solar*2/3) > 30)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 16, 90, 7, 26, 2);
		   if (voltage2percent(status.solar*2/3) > 40)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 24, 90, 7, 26, 2);
		   if (voltage2percent(status.solar*2/3) > 50)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 32, 90, 7, 26, 2);
		   if (voltage2percent(status.solar*2/3) > 60)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 40, 90, 7, 26, 3);
		   if (voltage2percent(status.solar*2/3) > 70)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 48, 90, 7, 26, 3);
		   if (voltage2percent(status.solar*2/3) > 80)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 56, 90, 7, 26, 3);
		   if (voltage2percent(status.solar*2/3) > 90)
			   TM_ILI9341_DrawImage(BAT_3_X_OFFSET + 64, 90, 7, 26, 3);

		   /*****************/
		   /* Update temp 1 */
		   /*****************/
		   snprintf(buffer, 16, "%3.1d~C", status.temp_1 / 100);
		   TM_ILI9341_Puts(30, 150, buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   if (status.temp_1 > 8500)
		       TM_ILI9341_DrawImage(50, 204, 36, 30, ERR); // error

		   snprintf(buffer, 16, "Max:%3.1d~c", status.temp_1_max / 100);
		   TM_ILI9341_Puts(31, 175, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		   snprintf(buffer, 16, "Min:%3.1d~c", status.temp_1_min / 100);
		   TM_ILI9341_Puts(31, 190, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

		   /* temp gauge range: 152 - 220*/
		   TM_ILI9341_DrawImage(1, 145, 30, 90, THERM);
		   gauge_index = 220 - (status.temp_1 / 100);
		   if (gauge_index < 152) gauge_index = 152;
		   if (gauge_index > 220) gauge_index = 220;
		   if (status.temp_1 > 9000)
			   TM_ILI9341_DrawFilledRectangle(14, gauge_index, 18, 220, ILI9341_COLOR_RED); /*Range 152 - 220*/
		   else if (status.temp_1 > 8000)
			   TM_ILI9341_DrawFilledRectangle(14, gauge_index, 18, 220, ILI9341_COLOR_ORANGE); /*Range 152 - 220*/
		   else
			   TM_ILI9341_DrawFilledRectangle(14, gauge_index, 18, 220, ILI9341_COLOR_GREEN); /*Range 152 - 220*/

		   /* Update temp 2 */
		   snprintf(buffer, 16, "%3.1d~C", status.temp_2 / 100);
		   TM_ILI9341_Puts(142, 150, buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   if (status.temp_2 > 8500)
			   TM_ILI9341_DrawImage(162, 204, 36, 30, ERR); // error
		   snprintf(buffer, 16, "Max:%3.1d~c", status.temp_2_max / 100);
		   TM_ILI9341_Puts(143, 175, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
		   snprintf(buffer, 16, "Min:%3.1d~c", status.temp_2_min / 100);
		   TM_ILI9341_Puts(143, 190, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

		   /* temp gauge range: 152 - 220*/
		   TM_ILI9341_DrawImage(113, 145, 30, 90, THERM);
		   gauge_index = 220 - (status.temp_2 / 100);
		   if (gauge_index < 152) gauge_index = 152;
		   if (gauge_index > 220) gauge_index = 220;
		   if (status.temp_2 > 9000)
			   TM_ILI9341_DrawFilledRectangle(126, gauge_index, 130, 220, ILI9341_COLOR_RED); /*Range 152 - 220*/
		   else if (status.temp_2 > 8000)
			   TM_ILI9341_DrawFilledRectangle(126, gauge_index, 130, 220, ILI9341_COLOR_ORANGE); /*Range 152 - 220*/
		   else
			   TM_ILI9341_DrawFilledRectangle(126, gauge_index, 130, 220, ILI9341_COLOR_GREEN); /*Range 152 - 220*/

		   /****************/
		   /* Update bat 2 */
		   /****************/
		   snprintf(buffer, 16, "%2.1d.%2.2dV", (status.bat_2 / 1000), (status.bat_2 % 1000)/10);
		   TM_ILI9341_Puts(113, 25, buffer, &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
		   snprintf(buffer, 16, "%3.1d%%", voltage2percent(status.bat_2));
		   TM_ILI9341_Puts(143, 60, buffer, &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

		   if (voltage2percent(status.bat_2) < 60)
			   TM_ILI9341_DrawImage(112, 55, 36, 30, ERR); // error

           TM_ILI9341_DrawImage(112, 88, 95, 30, BAT);
		   if (voltage2percent(status.bat_2) > 10)
		       TM_ILI9341_DrawImage(BAT_2_X_OFFSET, 90, 7, 26, 1);
		   if (voltage2percent(status.bat_2) > 20)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 8, 90, 7, 26, 1);
		   if (voltage2percent(status.bat_2) > 30)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 16, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_2) > 40)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 24, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_2) > 50)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 32, 90, 7, 26, 2);
		   if (voltage2percent(status.bat_2) > 60)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 40, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_2) > 70)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 48, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_2) > 80)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 56, 90, 7, 26, 3);
		   if (voltage2percent(status.bat_2) > 90)
			   TM_ILI9341_DrawImage(BAT_2_X_OFFSET + 64, 90, 7, 26, 3);
	   }
   }
}
