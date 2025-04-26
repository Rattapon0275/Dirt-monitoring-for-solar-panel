#include <stdio.h>
#include "esp_adc_cal.h"
#include "driver/adc.h"
#include "driver/touch_pad.h"
#include "esp_system.h"
#include "esp_log.h"

void init_adc(void);
int adc_input_get(void);
void read_rain_data(void);
void read_dirt_data(void);