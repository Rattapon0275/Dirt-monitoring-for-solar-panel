
#include "u_gpio.h"

void init_gpio(void)
{

    gpio_reset_pin(PUMP_GPIO);
    gpio_set_direction(PUMP_GPIO, GPIO_MODE_OUTPUT);
    on_off_pump(0);

    return;
}

void on_off_pump(unsigned char level)
{
    if(!level){
        gpio_set_level(PUMP_GPIO, 1);
    }else{
        gpio_set_level(PUMP_GPIO, 0);
    }
}

