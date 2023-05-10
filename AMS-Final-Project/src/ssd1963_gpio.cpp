#include "ssd1963_gpio.hpp"
#include <iostream>
#include <algorithm>
#include <driver/gpio.h>

ssd1963_gpio::ssd1963_gpio(gpio_num_t controlPins[])
{
    std::copy(controlPins, controlPins + 4, controlPins_);
}

ssd1963_gpio::~ssd1963_gpio()
{
}

void ssd1963_gpio::initGpioPins()
{
    esp_err_t err;
   
    control_config_ = {
        (1ULL << controlPins_[0]) |    // pin_bit_mask
        (1ULL << controlPins_[1]) |
        (1ULL << controlPins_[2]) |
        (1ULL << controlPins_[3]),  /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        GPIO_MODE_OUTPUT,               /*!< GPIO mode: set input/output mode                     */
        GPIO_PULLUP_DISABLE,            /*!< GPIO pull-up                                         */
        GPIO_PULLDOWN_DISABLE,          /*!< GPIO pull-down                                       */
        GPIO_INTR_DISABLE               /*!< GPIO interrupt type                                  */                  
    };
    err = gpio_config(&control_config_);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }

    for(int i=0; i<4; i++)
    {
        err = gpio_set_level(controlPins_[i], 0);
        if(err != ESP_OK)
        {
            Serial.print(err);
        }
    }
    delayMicroseconds(100);
} 

void ssd1963_gpio::RST_setLevel(bool level){
    esp_err_t err;
    gpio_set_level(controlPins_[0], level);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }
}

void ssd1963_gpio::RS_setLevel(bool level){
    esp_err_t err;
    gpio_set_level(controlPins_[1], level);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }
}

void ssd1963_gpio::WR_setLevel(bool level){
    esp_err_t err;
    gpio_set_level(controlPins_[2], level);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }
}

void ssd1963_gpio::CS_setLevel(bool level){
    esp_err_t err;
    gpio_set_level(controlPins_[3], level);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }
}

void ssd1963_gpio::WR_pulse(){
    gpio_set_level(controlPins_[2], 0);
    NOP();
    NOP();
    NOP();
    NOP();
    gpio_set_level(controlPins_[2], 1);

}

