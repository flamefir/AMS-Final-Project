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
#include "ssd1963_gpio.hpp"

#define SSD_FAST
#define SSD_WR_DATA(x)	ssdDataSlow(x)
#define SSD_WR_CMD(x)	ssdCmdSlow(x)

/* Implement later, for now we only run in slow mode */
// #ifndef SSD_FAST
// #else
// #define SSD_WR_DATA(x)	ssdDataSlow(x)
// #define SSD_WR_CMD(x)	ssdCmdSlow(x)
// #endif


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

#define SSD_CMD0(k)		ssd_cmd(k, sizeof(k)-1)
#define SSD_CMD(...)		SSD_CMD0(((unsigned char[]){ __VA_ARGS__ }))

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



