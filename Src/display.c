#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"
#include "images/images.h"
#include "main.h"

extern osMessageQId displayQueueHandle;
extern status_t status;

osEvent event;


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
//		   TM_ILI9341_DrawFilledRectangle(0, 55, 36, 85, ILI9341_COLOR_BLACK);
	}
	else if ( 	X > 45 && X < 75 &&
				Y > 200 && Y < 240) {
//		   TM_ILI9341_DrawFilledRectangle(50, 204, 86, 234, ILI9341_COLOR_BLACK);
	}
	else
		TM_ILI9341_DrawCircle(X, Y, 10, ILI9341_COLOR_BLUE);
}


void StartDisplayTask(void const * argument)
{
   TM_ILI9341_Init();
   TM_ILI9341_DrawFilledRectangle(0, 120, 320, 121, ILI9341_COLOR_WHITE);
   TM_ILI9341_DrawFilledRectangle(106, 0, 107, 240*2, ILI9341_COLOR_WHITE);
   TM_ILI9341_DrawFilledRectangle(213, 0, 214, 240*2, ILI9341_COLOR_WHITE);

   TM_ILI9341_Puts(20,0,"Engine", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(145,00,"Acc.", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(245,00,"Solar", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

   TM_ILI9341_Puts(20,122,"Engine", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(122,122,"Exhaust", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(250,122,"Room", &TM_Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);


   TM_ILI9341_DrawImage(6, 88, 95, 30, 0);
   TM_ILI9341_DrawImage(112, 88, 95, 30, 0);
   TM_ILI9341_DrawImage(219, 88, 95, 30, 0);


/* GAUGE TEST */
   TM_ILI9341_DrawImage(16, 90, 7, 26, 1);
   TM_ILI9341_DrawImage(16+8, 90, 7, 26, 1);

   TM_ILI9341_DrawImage(122, 90, 7, 26, 1);
   TM_ILI9341_DrawImage(122+8, 90, 7, 26, 1);
   TM_ILI9341_DrawImage(122+16, 90, 7, 26, 2);
   TM_ILI9341_DrawImage(122+24, 90, 7, 26, 2);


   TM_ILI9341_DrawImage(229, 90, 7, 26, 1);
   TM_ILI9341_DrawImage(229+8, 90, 7, 26, 1);
   TM_ILI9341_DrawImage(229+16, 90, 7, 26, 2);
   TM_ILI9341_DrawImage(229+24, 90, 7, 26, 2);
   TM_ILI9341_DrawImage(229+32, 90, 7, 26, 2);
   TM_ILI9341_DrawImage(229+40, 90, 7, 26, 3);
   TM_ILI9341_DrawImage(229+48, 90, 7, 26, 3);
   TM_ILI9341_DrawImage(229+56, 90, 7, 26, 3);
   TM_ILI9341_DrawImage(229+64, 90, 7, 26, 3);

/* TEXT TEST*/
   TM_ILI9341_Puts(1, 25, "12.21V", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(113, 25, "13.40V", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(220, 25, "17.03V", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

   TM_ILI9341_Puts(26, 60, "  5%", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(138, 60, " 35%", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(245, 60, "100%", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

   TM_ILI9341_DrawImage(86, 50, 40, 20, BOFF);

   TM_ILI9341_DrawImage(0, 55, 36, 30, ERR); // error

   TM_ILI9341_DrawImage(50, 204, 36, 30, ERR); // error

   TM_ILI9341_DrawImage(1, 145, 30, 90, 4);
   TM_ILI9341_DrawImage(113, 145, 30, 90, 4);
   TM_ILI9341_DrawImage(220, 145, 30, 90, 4);

   TM_ILI9341_Puts(30, 150, " 87~C", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(142, 150, " 65~C", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(247, 150, "25.4~C", &TM_Font_11x18, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);

   TM_ILI9341_Puts(31, 175, "Min: 20", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(143, 175, "Min: 50", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(250, 175, "Min: 20.0", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

   TM_ILI9341_Puts(31, 190, "Max: 120", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(143, 190, "Max: 150", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(250, 190, "Max: 26.0", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

   TM_ILI9341_DrawFilledRectangle(14, 152, 18, 220, ILI9341_COLOR_RED); /*Range 152 - 220*/
   TM_ILI9341_DrawFilledRectangle(126, 180, 130, 220, ILI9341_COLOR_GREEN);
   TM_ILI9341_DrawFilledRectangle(233, 165, 237, 220, ILI9341_COLOR_ORANGE);

   for(;;)
   {
	   uint32_t data;
	   event = osMessageGet( displayQueueHandle, 0xFFFFFFFF);
	   data = *(uint32_t *)(event.value.v);

       drawPoint(data & 0xFFFF, (data & 0xFFFF0000) >> 16);

   }
}
