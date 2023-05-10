/************************************************************
  File name: "ssd1963_displayDriver.hpp"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ssd1963.
  
  Max. uC clock frequency = 1/240 MHz (Tclk = 4.1667 ns)

  Connections:
  

  Malte Flammild, 19. April 2023
************************************************************/  
#include <driver/timer.h>
#include <driver/gpio.h>
#include <driver/ledc.h>
#include "ssd1963_gpio.hpp"
#include "Color.hpp"
#include "N4094.hpp"
#include "font.hpp"

class ssd1963
{
private:
    void ssdDataSlow(uint8_t value);
    void ssdCmdSlow(uint8_t value);
    N4094 shiftRegister_;
    ssd1963_gpio gpioControl_;
public:
    explicit ssd1963(N4094 shiftRegister, ssd1963_gpio gpioControl);
    ~ssd1963();
    void DisplayInit();
    void DisplayReset();
    void DisplayOff();
    void DisplayOn();
    void EnterSleepMode();
    void ExitSleepMode();
    void MemoryAccessControl(unsigned char parameter);
    void InterfacePixelFormat(unsigned char parameter);
    void SetColumnAddress(uint16_t Start, uint16_t End);
    void SetPageAddress(uint16_t Start, uint16_t End);
    void MemoryWrite();
    void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue);
    /*test*/
    void setColRowStartStop(uint16_t x_ColStart, uint16_t y_RowStart, uint16_t x_ColEnd, uint16_t y_RowEnd);
    void fill(uint16_t x_ColStart, uint16_t y_RowStart, uint16_t x_ColEnd, uint16_t y_RowEnd, Color color);
    void writeChar(uint16_t x_ColStart, uint16_t y_RowStart, char character, Color color, font * font, bool fontsize_8);
    void writeString(uint16_t x_ColStart, uint16_t y_RowStart, char * stringArray, Color color, font * font, bool fontsize_8);
};

































// void init_timer_clk()
// {
//   gpio_pad_select_gpio(GPIO_NUM_5); //set gpio pad as gpio pin
//   gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);


//     // "out-of-order err" dot operator syntax ikke supported, se ssd1963_gpio med gpio_config_t 
//   ledc_timer_config_t ledc_timer = {
//     .duty_resolution = LEDC_TIMER_1_BIT, // resolution of PWM duty
//     .freq_hz = 40000000,                      // frequency of PWM signal. 40 mHz.
//     LEDC_HIGH_SPEED_MODE,   // timer mode
//     .timer_num = LEDC_TIMER_1,            // timer index
//     .clk_cfg = LEDC_APB_CLK,             // Auto select the source clock
//   };
//   ledc_timer_config(&ledc_timer);

//   ledc_channel_config_t ledc_channel = {
//     .channel    = LEDC_CHANNEL_0,
//     .duty       = 0,
//     .gpio_num   = GPIO_NUM_5,
//     .speed_mode = LEDC_HIGH_SPEED_MODE,
//     .hpoint     = 0,
//     .timer_sel  = LEDC_TIMER_1
//   };
//   ledc_channel_config(&channel_config);

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
//}



