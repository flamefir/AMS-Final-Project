#include <Arduino.h>

class ssd1963_gpio
{
private:
    gpio_num_t dataPins_[8] = {};
    gpio_num_t controlPins_[4] = {};
    gpio_config_t io_config_;
    gpio_config_t control_config_;
public:
    ssd1963_gpio();
    ~ssd1963_gpio();
    void initGpioPins(gpio_num_t controlPins[], gpio_num_t dataPins[]);
    void sendData(uint8_t data);
    void RST_setLevel(bool level);
    void RS_setLevel(bool level);
    void WR_setLevel(bool level);
    void CS_setLevel(bool level);
    void WR_pulse();
};