#include "gpio_ssd1963.h"


void ssdDataSlow(unsigned char c)
{
	GPIO_CLR = LCD_WR_MASK;
	WAIT1;
	GPIO_CLR = ~LCD_D8(c) & GPIO_MASK;
	WAIT1;
	GPIO_SET = LCD_D8(c) & GPIO_MASK;
	WAIT2;
	GPIO_SET = LCD_WR_MASK;
	WAIT2;
}

void ssdCmdSlow(unsigned char c)
{
	GPIO_CLR = LCD_DC_MASK;
	WAIT1;
	GPIO_CLR = LCD_WR_MASK;
	WAIT1;
	GPIO_CLR = (~LCD_D8(c) & GPIO_MASK);
	GPIO_SET = (LCD_D8(c) & GPIO_MASK);
	WAIT1;
	GPIO_SET = LCD_WR_MASK;
	WAIT1;
	GPIO_SET = LCD_DC_MASK;
	WAIT2;
}