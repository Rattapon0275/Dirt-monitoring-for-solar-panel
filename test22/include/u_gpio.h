#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h" // For esp_rom_delay_us()

#define PUMP_GPIO 2  // Built-in LED pin on most ESP32 boards

#define BitSet(w,b)   w|=(1<<b)
#define BitClear(w,b) w&=~(1<<b)
#define BitRead(w,b)  (w >> b)&0x01
#define BitToggle(w,b) w^=(1<<b)

void init_gpio(void);
void on_off_pump(unsigned char level);
