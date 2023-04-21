#include <Arduino.h>
#include <driver/timer.h>
#include <driver/gpio.h>
#include <ssd1963_displayDriver.hpp>
#include "HR2046.hpp"

// Data port definitions:
// #define DB0 GPIO_NUM_21
// #define DB1 GPIO_NUM_22
// #define DB2 GPIO_NUM_10
// #define DB3 GPIO_NUM_17
// #define DB4 GPIO_NUM_32
// #define DB5 GPIO_NUM_33
// #define DB6 GPIO_NUM_25
// #define DB7 GPIO_NUM_26

#define DB0 GPIO_NUM_13
#define DB1 GPIO_NUM_12
#define DB2 GPIO_NUM_21
#define DB3 GPIO_NUM_27
#define DB4 GPIO_NUM_26
#define DB5 GPIO_NUM_25
#define DB6 GPIO_NUM_33
#define DB7 GPIO_NUM_32

// Control port definitions:
#define RST_PIN GPIO_NUM_4
#define WR_PIN GPIO_NUM_0
#define RS_PIN GPIO_NUM_2
#define CS_PIN GPIO_NUM_16

// Touch pins definitions
#define TOUCH_IRQ GPIO_NUM_34 
#define TOUCH_CS GPIO_NUM_5

inline float randf()
{
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

inline float rand(int min, int max)
{
  return min + rand() % (max + 1 - min);
}

gpio_num_t gpioDataPins[8] = {DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};
gpio_num_t controlPins[4] = {RST_PIN, RS_PIN, WR_PIN, CS_PIN};

SPIClass vspi(VSPI);
//Initialize display driver
ssd1963 ssd1963Driver(controlPins, gpioDataPins);
//Initialize touch driver
HR2046 touch(vspi, TOUCH_CS);

//Stopwatch 

// unsigned long start_time; // store the start time
// unsigned long elapsed_time; // store the elapsed time
// bool is_running = false; // flag - the stopwatch is running

// hw_timer_t * timer = NULL; // initialize the timer object

// void IRAM_ATTR onTimer() {
//   elapsed_time = millis() - start_time;
// }

static bool touchFlag = false;
void IRAM_ATTR touchIrq()
{
  if (!touchFlag)
  {
    touchFlag = true;
  }
}

void setup(){
    Serial.begin(115200);

    vspi.begin();
    vspi.setFrequency(1000000);

    ssd1963Driver.DisplayInit();
    touch.init();
    pinMode(TOUCH_IRQ, INPUT_PULLUP);
    attachInterrupt(TOUCH_IRQ, touchIrq, FALLING);
}

void loop(){
    //Serial.println("Yes i am here");
    // while (true)
    // {
    //     auto x = rand(0, 800);
    //     auto y = rand(0, 480);
    //     auto w = rand(10, 150);
    //     auto h = rand(10, 150);
    //     if (x + w > 800 || y + h > 480)
    //     {
    //     continue;
    //     }

    //     ssd1963Driver.fill(x, y, w, h, 0xff, 0xff, 0xff);
    //     break;
    // }

    if (touchFlag)
    {
        uint16_t x;
        uint16_t y;
        uint32_t z;
        touch.readRaw(x, y, z);
        if (z > 200)
        {
            printf("Touch: [%d, %d, %d]\n", x, y, z);
        }

        delay(10);

        touchFlag = false;
    }
    //delay(500);
}



// void setup() {
//   Serial.begin(115200);


//   //CLOCK TIMER
//   init_timer
// }

// void loop() {
//   if (Serial.available() > 0) {
//     char input = Serial.read();
//     if (input == 's') { // start stopwatch
//       start_time = millis();
//       is_running = true;
//       timerAlarmEnable(timer); // enable interrupt
//       Serial.println("Stopwatch started.");
//     }
//     else if (input == 'x') { // stop stopwatch
//       elapsed_time = millis() - start_time;
//       is_running = false;
//       timerAlarmDisable(timer); // disable interrupt
//       Serial.print("Elapsed time: ");
//       Serial.print(elapsed_time);
//       Serial.println(" ms");
//     }
//   }

//   if (is_running) {
//     Serial.print("Elapsed time: ");
//     Serial.print(elapsed_time);
//     Serial.println(" ms");
//   }
// }

