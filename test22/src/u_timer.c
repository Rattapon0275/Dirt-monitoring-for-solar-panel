#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/timer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "u_gpio.h"
#include "u_adc.h"

//Define constants for timer0
#define TIMER_BASE_CLK        APB_CLK_FREQ
#define TIMER_DIVIDER         16  //  Hardware timer clock divider
#define TIMER_SCALE           (TIMER_BASE_CLK / TIMER_DIVIDER)  // Timer scale (Hz)
#define TIMER_INTERVAL_SEC    5 // Timer interval in seconds
#define TIMER_GROUP           TIMER_GROUP_0
#define TIMER_INDEX           TIMER_0

/*-----------------------------------
 *----Interrupt callback function----
 *-----------------------------------*/

void IRAM_ATTR timer_isr_callback(void *param) {
    int timer_idx = (int)param;

    // Clear interrupt status
    timer_group_clr_intr_status_in_isr(TIMER_GROUP, TIMER_INDEX);

    // Re-enable the alarm
    timer_group_enable_alarm_in_isr(TIMER_GROUP, TIMER_INDEX);

    /*-----Start User Program-----*/

    
    /*-----End User Program-----*/
}

/*-----------------------------------
 *-------Initialized function--------
 *-----------------------------------*/

// Function to initialize the timer
void init_timer() {
    // Configure timer parameters
    timer_config_t config = {
        .divider = TIMER_DIVIDER,                    // Clock divider
        .counter_dir = TIMER_COUNT_UP,               // Count-up mode
        .counter_en = TIMER_PAUSE,                   // Start in paused state
        .alarm_en = TIMER_ALARM_EN,                  // Enable alarms
        .auto_reload = TIMER_AUTORELOAD_EN,          // Auto-reload enabled
    };
    timer_init(TIMER_GROUP, TIMER_INDEX, &config);   // Initialize timer

    // Set timer counter value to 0
    timer_set_counter_value(TIMER_GROUP, TIMER_INDEX, 0);

    // Set the alarm value in seconds
    timer_set_alarm_value(TIMER_GROUP, TIMER_INDEX, TIMER_INTERVAL_SEC * TIMER_SCALE);

    // Enable alarm
    timer_enable_intr(TIMER_GROUP, TIMER_INDEX);

    // Register ISR handler
    timer_isr_register(TIMER_GROUP, TIMER_INDEX, timer_isr_callback, 
                       (void *)TIMER_INDEX, ESP_INTR_FLAG_IRAM, NULL);

    // Start timer
    timer_start(TIMER_GROUP, TIMER_INDEX);
}