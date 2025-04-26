#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"

#define SPI_HOST                SPI2_HOST  // Use SPI2 (VSPI)
#define PIN_NUM_MISO            19        // GPIO for MISO (data from MAX6675)
#define PIN_NUM_MOSI            -1        // Not used for MAX6675
#define PIN_NUM_CLK             18        // GPIO for SCK (clock)
#define PIN_NUM_CS              5         // GPIO for CS (chip select)

esp_err_t spi_init(spi_device_handle_t *spi);
void init_spi(void);
esp_err_t max6675_read_temperature(spi_device_handle_t spi, float *temperature);
void read_max6675_data(void);
