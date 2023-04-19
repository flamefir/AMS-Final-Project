#include "ssd1963_gpio.hpp"

#define RST_HIGH digitalWrite(rst, HIGH)
#define RST_LOW digitalWrite(rst, LOW)

#define RS_HIGH digitalWrite(rs, HIGH)
#define RS_LOW digitalWrite(rs, LOW)

#define WR_HIGH digitalWrite(wr, HIGH)
#define WR_LOW digitalWrite(wr, LOW)

#define CS_HIGH digitalWrite(cs, HIGH)
#define CS_LOW digitalWrite(cs, LOW)

#define PULSE \
    WR_LOW;   \
    WR_HIGH;

void ssdDataSlow(unsigned char c)
{
    CS_LOW;
    RS_LOW;
    WR_HIGH;
    sreg.write(value);
    PULSE;
    CS_HIGH;
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