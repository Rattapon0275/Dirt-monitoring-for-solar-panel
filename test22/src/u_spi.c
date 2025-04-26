#include "u_spi.h"
#include "u_main.h"

static const char *TAG2 = "MAX6675";
spi_device_handle_t spi1;
float temp1;

// Function to initialize SPI for MAX6675
esp_err_t spi_init(spi_device_handle_t *spi) {
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 2, // MAX6675 returns 16 bits of data
    };

    ESP_ERROR_CHECK(spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000, // 1 MHz
        .mode = 0,                 // SPI mode 0 (CPOL=0, CPHA=0)
        .spics_io_num = PIN_NUM_CS,
        .queue_size = 1,
    };

    return spi_bus_add_device(SPI_HOST, &devcfg, spi);
}

void init_spi(void){

    ESP_ERROR_CHECK(spi_init(&spi1));
}

// Function to read temperature from MAX6675
esp_err_t max6675_read_temperature(spi_device_handle_t spi, float *temperature) {
    uint8_t data[2]; // MAX6675 outputs 16-bit data
    spi_transaction_t t = {
        .length = 16,  // 16 bits
        .rx_buffer = data,
    };

    esp_err_t ret = spi_device_transmit(spi, &t);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG2, "SPI transmit failed");
        return ret;
    }

    // Combine the two bytes into a single 16-bit value
    uint16_t raw_data = (data[0] << 8) | data[1];

    // Check if the thermocouple is connected (D0 bit = 0 when connected)
    if (raw_data & 0x0004) {
        ESP_LOGW(TAG2, "Thermocouple not connected");
        return ESP_ERR_INVALID_STATE;
    }

    // Extract the temperature data (bits 15:3) and calculate temperature
    *temperature = ((raw_data >> 3) * 0.24); // Temperature in Celsius
    return ESP_OK;
}

void read_max6675_data(void){
    esp_err_t ret = max6675_read_temperature(spi1, &temp1);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG2, "Temperature: %.2f °C", temp1);
        sensor1.Temp = temp1;
    } else if (ret == ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG2, "No thermocouple detected");
    } else {
        ESP_LOGE(TAG2, "Error reading temperature: %s", esp_err_to_name(ret));
    }
}