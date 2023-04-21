#include "ssd1963_gpio.hpp"
#include <iostream>
#include <algorithm>
#include <driver/gpio.h>

ssd1963_gpio::ssd1963_gpio()
{
}

ssd1963_gpio::~ssd1963_gpio()
{
}

void ssd1963_gpio::initGpioPins(gpio_num_t controlPins[], gpio_num_t dataPins[])
{
    esp_err_t err;
    std::copy(dataPins, dataPins + 8, dataPins_);
    std::copy(controlPins, controlPins + 4, controlPins_);

    /// @brief The following code, takes multiple pins in the ESP32 env and chains them together,
    /// This enables sending data on multiple pins like uint8_t, by using gpio_set_level
    io_config_ = {
        (1ULL << dataPins[0]) |    // pin_bit_mask
        (1ULL << dataPins[1]) |
        (1ULL << dataPins[2]) |
        (1ULL << dataPins[3]) |
        (1ULL << dataPins[4]) |
        (1ULL << dataPins[5]) |
        (1ULL << dataPins[6]) |
        (1ULL << dataPins[7]),      /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
        GPIO_MODE_OUTPUT,               /*!< GPIO mode: set input/output mode                     */
        GPIO_PULLUP_DISABLE,            /*!< GPIO pull-up                                         */
        GPIO_PULLDOWN_DISABLE,          /*!< GPIO pull-down                                       */
        GPIO_INTR_DISABLE               /*!< GPIO interrupt type                                  */                  
    };
    err = gpio_config(&io_config_);
    if(err != ESP_OK)
    {
        Serial.print(err);
    }

    control_config_ = {
        (1ULL << dataPins[0]) |    // pin_bit_mask
        (1ULL << dataPins[1]) |
        (1ULL << dataPins[2]) |
        (1ULL << dataPins[3]) |
        (1ULL << dataPins[4]),  /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
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

    // Set all pins to 0 for init
    for(int i=0; i<8; i++)
    {
        err = gpio_set_level(dataPins_[i], 0);
        if(err != ESP_OK)
        {
            Serial.print(err);
        }
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

void ssd1963_gpio::sendData(uint8_t data)
{
    esp_err_t err;
    for(int i=0; i<8; i++)
    {
        err = gpio_set_level(dataPins_[i], (data >> i) & 1);
        if(err != ESP_OK)
        {
            Serial.print(err);
        }
    }
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
    gpio_set_level(controlPins_[2], 1);

}

