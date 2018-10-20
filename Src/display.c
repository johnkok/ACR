#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_fonts.h"

extern osMessageQId displayQueueHandle;

osEvent event;

void drawPoint(uint16_t X, uint16_t Y)
{
	TM_ILI9341_DrawCircle(X, Y, 10, ILI9341_COLOR_BLUE);
}


void StartDisplayTask(void const * argument)
{

   TM_ILI9341_Init();

   TM_ILI9341_Puts(0,  5, "Battery monitor v2.0", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 30, "====================", &TM_Font_16x26, ILI9341_COLOR_YELLOW, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 55, "Display initialized!", &TM_Font_7x10, ILI9341_COLOR_GREEN, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 70, "Configuring ADC 1", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 85, "Configuring ADC 2", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 100, "Configuring NMEA0183", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 115, "Configuring NMEA2000", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 130, "Configuring USB", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 145, "Configuring Engine", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 160, "Configuring Bridge", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 175, "Configuring Buzzer", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 190, "Configuring Alarm", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_Puts(0, 205, "Configuring Clock", &TM_Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_BLACK);
   TM_ILI9341_DrawCircle(280, 200, 20, ILI9341_COLOR_BLUE);

   for(;;)
   {
	   uint32_t data;
	   event = osMessageGet( displayQueueHandle, 0xFFFFFFFF);
	   data = *(uint32_t *)(event.value.v);

       drawPoint(data & 0xFFFF, (data & 0xFFFF0000) >> 16);

   }
}