#include <Arduino.h>
#include <driver/timer.h>
#include <DisplayDriver_ssd1963.h>

#define BOARD_LED 2
//Stopwatch 

// unsigned long start_time; // store the start time
// unsigned long elapsed_time; // store the elapsed time
// bool is_running = false; // flag - the stopwatch is running

// hw_timer_t * timer = NULL; // initialize the timer object

// void IRAM_ATTR onTimer() {
//   elapsed_time = millis() - start_time;
// }




void setup() {
  Serial.begin(115200);


  //CLOCK TIMER
  init_timer
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 's') { // start stopwatch
      start_time = millis();
      is_running = true;
      timerAlarmEnable(timer); // enable interrupt
      Serial.println("Stopwatch started.");
    }
    else if (input == 'x') { // stop stopwatch
      elapsed_time = millis() - start_time;
      is_running = false;
      timerAlarmDisable(timer); // disable interrupt
      Serial.print("Elapsed time: ");
      Serial.print(elapsed_time);
      Serial.println(" ms");
    }
  }

  if (is_running) {
    Serial.print("Elapsed time: ");
    Serial.print(elapsed_time);
    Serial.println(" ms");
  }
}

