/************************************************************
  File name: "TFTdriver.c"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ILI 9341.
  
  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connections:
  DB15-DB8:   PORT A
  DB7-DB0:    PORT C

  RESETx:     PORT G, bit 0
  CSx:        PORT G, bit 1
  WRx:        PORT G, bit 2
  RS (=D/Cx): PORT D, bit 7

  Henning Hargaard, February 15, 2018
************************************************************/  
#include <driver/timer.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
//#include <esp_timer.h>
#include "gpio_ssd1963.h"

#define SSD_FAST
#define SSD_WR_DATA(x)	ssdDataSlow(x)
#define SSD_WR_CMD(x)	ssdCmdSlow(x)

/* Implement later, for now we only run in slow mode */
// #ifndef SSD_FAST
// #else
// #define SSD_WR_DATA(x)	ssdDataSlow(x)
// #define SSD_WR_CMD(x)	ssdCmdSlow(x)
// #endif

static inline void ssd_cmd(const unsigned char k[static 1], unsigned data_len)
{
	SSD_WR_CMD(*k);
	k++;
	while (data_len) {
		SSD_WR_DATA(*k);
		k++;
		data_len--;
	}
}

void init_timer_clk()
{
  gpio_pad_select_gpio(GPIO_NUM_5); //set gpio pad as gpio pin
  gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);

  ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_1_BIT, // resolution of PWM duty
    .freq_hz = 40000000,                      // frequency of PWM signal. 40 mHz.
    .speed_mode = LEDC_HIGH_SPEED_MODE,   // timer mode
    .timer_num = LEDC_TIMER_1,            // timer index
    .clk_cfg = LEDC_APB_CLK,             // Auto select the source clock
  };
  ledc_timer_config(&ledc_timer);

  ledc_channel_config_t ledc_channel = {
    .channel    = LEDC_CHANNEL_0,
    .duty       = 0,
    .gpio_num   = GPIO_NUM_5,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = LEDC_TIMER_1
  };
  ledc_channel_config(&channel_config);

  // timer_config_t config = {
  //     .alarm_en = TIMER_ALARM_EN,
  //     .counter_en = TIMER_PAUSE,
  //     .intr_type = TIMER_INTR_LEVEL, //If running in alarm mode
  //     .counter_dir = TIMER_COUNT_UP, 
  //     .auto_reload = TIMER_AUTORELOAD_EN,        
  //     .divider = 2 //APB_CLK_FREQ / 2 = 40MHz
  // };

  // timer_init(TIMER_GROUP_1, TIMER_1, &config);
  // timer_set_counter_value(TIMER_GROUP_1, TIMER_1, 0); //auto reload to 0    
  // timer_set_alarm_value(TIMER_GROUP_1, TIMER_1, 1); //alarm every 1 tick (2 base timer clock cycles) aka. output compare interrupt
  // timer_enable_intr(TIMER_GROUP_1, TIMER_1); // Enable the timer interrupt
  
  // timer_isr_callback_add(TIMER_GROUP_1, TIMER_1, timer_clk_isr_callback, 0);


  // timer_start(TIMER_GROUP_1, TIMER_1);
}


#define SSD_CMD0(k)		ssd_cmd(k, sizeof(k)-1)
#define SSD_CMD(...)		SSD_CMD0(((unsigned char[]){ __VA_ARGS__ }))

#define SSD_NOP()			                        SSD_CMD(0x00)
#define SSD_SOFT_RESET()		                    SSD_CMD(0x01)
#define SSD_GET_POWER_MODE()	                    SSD_CMD(0x0a)
#define SSD_GET_ADDRESS_MODE()		                SSD_CMD(0x0b)
#define SSD_GET_DISPLAY_MODE()		                SSD_CMD(0x0d)
#define SSD_GET_TEAR_EFFECT_STATUS()	            SSD_CMD(0x0e)
#define SSD_ENTER_SLEEP_MODE()		                SSD_CMD(0x10)
#define SSD_EXIT_SLEEP_MODE()		                SSD_CMD(0x11)
#define SSD_ENTER_PARTIAL_MODE()	                SSD_CMD(0x12)
#define SSD_ENTER_NORMAL_MODE()		                SSD_CMD(0x13)
#define SSD_EXIT_INVERT_MODE()		                SSD_CMD(0x20)
#define SSD_ENTER_INVERT_MODE()		                SSD_CMD(0x21)
#define SSD_SET_GAMMA_CURVE(g)		                SSD_CMD(0x26, 1 << (g))
#define SSD_SET_DISPLAY_OFF()                       SSD_CMD(0x28)
#define SSD_SET_DISPLAY_ON()                        SSD_CMD(0x29)
#define SSD_SET_COLUMN_ADDRESS(start_col,end_col)   SSD_CMD(0x2a, (start_col) >> 8, (start_col) & 0xff, (end_col) >> 8, (end_col) & 0xff)
#define SSD_SET_PAGE_ADDRESS(start_page,end_page)   SSD_CMD(0x2b, (start_page) >> 8, (start_page) & 0xff, (end_page) >> 8, (end_page) & 0xff)
#define SSD_WRITE_MEMORY_START()                    SSD_CMD(0x2c)
#define SSD_READ_MEMORY_START()                     SSD_CMD(0x2e)
#define SSD_SET_PARTIAL_AREA(start_row,end_row)     SSD_CMD(0x30, (start_row) >> 8, (start_row), (end_row) >> 8, (end_row))
#define SSD_SET_SCROLL_AREA(tfa,vsa,bfa)            SSD_CMD(0x33, (tfa) >> 8, (tfa), (vsa) >> 8, (vsa), (bfa) >> 8, (bfa))
#define SSD_SET_TEAR_OFF()		                    SSD_CMD(0x34)
#define SSD_SET_TEAR_ON(v_and_h_blank)	            SSD_CMD(0x35, !!(v_and_h_blank))
#define SSD_SET_ADDRESS_MODE(arg)	                SSD_CMD(0x36, (arg))
#define SSD_SET_SCROLL_START(vsp)	                SSD_CMD(0x37, (vsp) >> 8, (vsp))
#define SSD_EXIT_IDLE_MODE()		                SSD_CMD(0x38)
#define SSD_ENTER_IDLE_MODE()		                SSD_CMD(0x39)
#define SSD_WRITE_MEMORY_CONTINUE()	                SSD_CMD(0x3c)
#define SSD_READ_MEMORY_CONTINUE()	                SSD_CMD(0x3e)
#define SSD_SET_TEAR_SCANLINE(n)	                SSD_CMD(0x44, (n) >> 8, (n))
#define SSD_GET_SCANLINE()		                    SSD_CMD(0x45)
#define SSD_READ_DDB()			                    SSD_CMD(0xa1) 
#define SSD_SET_LCD_MODE(a,b,hdp,vdp,g)             SSD_CMD(0xb0, (a), (b), (hdp) >> 8, (hdp), (vdp) >> 8, (vdp), (g))
#define SSD_GET_LCD_MODE()		                    SSD_CMD(0xb1) 
#define SSD_SET_HORI_PERIOD(ht,hps,hpw,lps,lpspp)   SSD_CMD(0xb4, (ht) >> 8, (ht), (hps) >> 8, (hps), (hpw), (lps) >> 8, (lps), (lpspp))
#define SSD_GET_HORI_PERIOD()		                SSD_CMD(0xb5)
#define SSD_SET_VERT_PERIOD(vt,vps,vpw,fps)         SSD_CMD(0xb6, (vt) >> 8, (vt), (vps) >> 8, (vps), (vpw), (fps) >> 8, (fps))
#define SSD_GET_VERT_PERIOD()		                SSD_CMD(0xb7) 
#define SSD_SET_GPIO_CONF(a,b)		                SSD_CMD(0xb8, (a), (b))
#define SSD_GET_GPIO_CONF()		                    SSD_CMD(0xb9) 
#define SSD_SET_GPIO_VALUE(a)		                SSD_CMD(0xba, (a))
#define SSD_GET_GPIO_STATUS()		                SSD_CMD(0xbb)
#define SSD_SET_POST_PROC(a,b,c,d)	                SSD_CMD(0xbc, (a), (b), (c), (d))
#define SSD_GET_POST_PROC()		                    SSD_CMD(0xbd)
#define SSD_SET_PWM_CONF(pwmf,pwm,c,d,e,f)	        SSD_CMD(0xbe, (pwmf), (pwm), (c), (d), (e), (f))
#define SSD_GET_PWM_CONF()		                    SSD_CMD(0xbf) 
#define SSD_SET_LCD_GEN0(a,gf,gr,f,gp)              SSD_CMD(0xc0, (a), (gf) >> 8, (gf), (gr) >> 8, (gr), (f) | (gp) >> 8, (gp))
#define SSD_GET_LCD_GEN0()		                    SSD_CMD(0xc1)
#define SSD_SET_LCD_GEN1(a,gf,gr,f,gp)              SSD_CMD(0xc2, (a), (gf) >> 8, (gf), (gr) >> 8, (gr), (f) | (gp) >> 8, (gp))
#define SSD_GET_LCD_GEN1()		                    SSD_CMD(0xc3)
#define SSD_SET_LCD_GEN2(a,gf,gr,f,gp)              SSD_CMD(0xc4, (a), (gf) >> 8, (gf), (gr) >> 8, (gr), (f) | (gp) >> 8, (gp))
#define SSD_GET_LCD_GEN2()		                    SSD_CMD(0xc5)
#define SSD_SET_LCD_GEN3(a,gf,gr,f,gp)              SSD_CMD(0xc6, (a), (gf) >> 8, (gf), (gr) >> 8, (gr), (f) | (gp) >> 8, (gp))
#define SSD_GET_LCD_GEN3()		                    SSD_CMD(0xc7)
#define SSD_SET_GPIO0_ROP(a,b)		                SSD_CMD(0xc8, (a), (b))
#define SSD_GET_GPIO0_ROP()		                    SSD_CMD(0xc9) 
#define SSD_SET_GPIO1_ROP(a,b)		                SSD_CMD(0xca, (a), (b))
#define SSD_GET_GPIO1_ROP()		                    SSD_CMD(0xcb)
#define SSD_SET_GPIO2_ROP(a,b)	    	            SSD_CMD(0xcc, (a), (b))
#define SSD_GET_GPIO2_ROP()		                    SSD_CMD(0xcd) 
#define SSD_SET_GPIO3_ROP(a,b)	    	            SSD_CMD(0xce, (a), (b))
#define SSD_GET_GPIO3_ROP()		                    SSD_CMD(0xcf) 
#define SSD_SET_DBC_CONF(a)		                    SSD_CMD(0xd0, (a))
#define SSD_GET_DBC_CONF()		                    SSD_CMD(0xd1) 
#define SSD_SET_DBC_TH(th1,th2,th3)                 SSD_CMD(0xd4, (th1) >> 16, (th1) >> 8, (th1), (th2) >> 16, (th2) >> 8, (th2), (th3) >> 16, (th3) >> 8, (th3))
#define SSD_GET_DBC_TH()		                    SSD_CMD(0xd5)
#define SSD_SET_PLL(a)			                    SSD_CMD(0xe0, (a))
#define SSD_SET_PLL_MN(m,n,c)		                SSD_CMD(0xe2, (m), (n), (c))
#define SSD_GET_PLL_MN()		                    SSD_CMD(0xe3) 
#define SSD_GET_PLL_STATUS()		                SSD_CMD(0xe4) 
#define SSD_SET_DEEP_SLEEP()		                SSD_CMD(0xe5)
#define SSD_SET_LSHIFT_FREQ(fpr)	                SSD_CMD(0xe6, (fpr) >> 16, (fpr) >> 8, (fpr))
#define SSD_GET_LSHIFT_FREQ()		                SSD_CMD(0xe7) 
#define SSD_SET_PIXEL_DATA_INTERFACE(a)	            SSD_CMD(0xf0, (a))
#define SSD_GET_PIXEL_DATA_INTERFACE()	            SSD_CMD(0xf1) 

void DisplayInit();
void DisplayOff();
void DisplayOn();
void SleepOut();
void MemoryAccessControl(unsigned char parameter);
void InterfacePixelFormat(unsigned char parameter);
void WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green);
void SetColumnAddress(unsigned int Start, unsigned int End);
void SetPageAddress(unsigned int Start, unsigned int End);
void MemoryWrite();
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, 
                   unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue);



