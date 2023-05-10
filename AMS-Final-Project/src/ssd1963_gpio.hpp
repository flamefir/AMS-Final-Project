#include <Arduino.h>

class ssd1963_gpio
{
private:
    gpio_num_t controlPins_[4] = {};
    gpio_config_t io_config_;
    gpio_config_t control_config_;
public:
    ssd1963_gpio(gpio_num_t controlPins[]);
    ~ssd1963_gpio();
    void initGpioPins();
    void RST_setLevel(bool level);
    void RS_setLevel(bool level);
    void WR_setLevel(bool level);
    void CS_setLevel(bool level);
    void WR_pulse();
};