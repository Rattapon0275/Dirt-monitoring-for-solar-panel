#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include "u_gpio.h"
#include "u_adc.h"
#include "u_main.h"

esp_adc_cal_characteristics_t adc_chars;
esp_adc_cal_value_t val_type;

// ADC Configuration
#define ADC_CHANNEL    ADC1_CHANNEL_6   // GPIO34 (ADC1 Channel 6)
#define ADC_WIDTH      ADC_WIDTH_BIT_12 // 12-bit resolution (0 - 4095)
#define ADC_ATTEN      ADC_ATTEN_DB_11  // Input voltage range 0-3.9V
#define rain_constant  3500

static const char *TAG5 = "RAIN";

#define TOUCH_PAD_PIN TOUCH_PAD_NUM0  // GPIO4 = T0 on ESP32
#define TAG6 "TOUCH_SENSOR"

// Function to configure ADC
void init_adc(void) {
    // Configure ADC width and channel
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
    val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, 1100, &adc_chars);

    // Initialize the touch pad driver
    touch_pad_init();
    
    // Set the touch pad to the desired channel (e.g., T0 pin)
    touch_pad_config(TOUCH_PAD_NUM0, 0);  // Configure Touch pad T0

    sensor1.Dirt = 0;

}

int adc_input_get(void)
{
    uint32_t adc_reading = adc1_get_raw(ADC_CHANNEL);
    return adc_reading;
}

void read_rain_data(void)
{
    int rain = adc_input_get();
    char rain_score = 0;
    if(rain < rain_constant){
        rain_score = 1;
        ESP_LOGI(TAG5, "Raining");
    }else{
        ESP_LOGI(TAG5, "No rain");
    }
    sensor1.Rain = rain_score;
}

void read_dirt_data(void){

    uint16_t touch_value;
    ESP_ERROR_CHECK(touch_pad_read(TOUCH_PAD_PIN, &touch_value));
    if(touch_value > 113){ //No dirt
        sensor1.Dirt = 0;
    }
    if(touch_value < 113){ //Dirt is found
        sensor1.Dirt = 1;
    }
    ESP_LOGI(TAG6, "Touch Value: %d", touch_value);
}