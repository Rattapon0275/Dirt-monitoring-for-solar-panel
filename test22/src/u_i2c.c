#include "u_i2c.h"
#include "u_main.h"

float energy_prev, power_prev;

static const char *TAG = "INA226";
static const char *TAG1 = "BH1750";

// Function to initialize I2C
esp_err_t i2c_master_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0));
    energy_prev = 0;
    power_prev = 0;
    return ESP_OK;
}

// Function to write to INA226 registers
esp_err_t write_register(uint8_t reg, uint16_t value) {
    uint8_t data[3];
    data[0] = reg;
    data[1] = (value >> 8) & 0xFF;
    data[2] = value & 0xFF;
    return i2c_master_write_to_device(I2C_MASTER_NUM, INA226_ADDR, data, sizeof(data), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

// Function to read from INA226 registers
esp_err_t read_register(uint8_t reg, uint16_t *value) {
    uint8_t data[2];
    esp_err_t ret = i2c_master_write_read_device(I2C_MASTER_NUM, INA226_ADDR, &reg, 1, data, sizeof(data), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    if (ret == ESP_OK) {
        *value = (data[0] << 8) | data[1];
    }
    return ret;
}

// Function to read and print INA226 data
void read_ina226_data(void) {
    uint16_t shunt_voltage_raw, bus_voltage_raw, current_raw, power_raw;
    float shunt_voltage, bus_voltage, current, power;

    if (read_register(REG_SHUNT_VOLTAGE, &shunt_voltage_raw) == ESP_OK) {
        shunt_voltage = shunt_voltage_raw * 2.5e-6; // Convert to volts
        ESP_LOGI(TAG, "Shunt Voltage: %.6f V", shunt_voltage);
        /*current = shunt_voltage * 10 * 1000;
        ESP_LOGI(TAG, "Current: %f mA", current);
        sensor1.Amp = current;*/
    } //else current = 0;

    if (read_register(REG_BUS_VOLTAGE, &bus_voltage_raw) == ESP_OK) {
        bus_voltage = bus_voltage_raw * 1.25e-3; // Convert to volts
        ESP_LOGI(TAG, "Bus Voltage: %.3f V", bus_voltage);
        sensor1.Volt = bus_voltage;
        /*power = bus_voltage * current;
        ESP_LOGI(TAG, "Power: %f mW", power);
        sensor1.Pow = power;*/
    }

    if (read_register(REG_CURRENT, &current_raw) == ESP_OK) {
        current = current_raw * 61.035 * 0.001; // Convert to amps (calibration dependent)
        //ESP_LOGI(TAG, "Current: %f A", current);
        ESP_LOGI(TAG, "Current: %f mA", current);
        sensor1.Amp = current;
    }

    if (read_register(REG_POWER, &power_raw) == ESP_OK) {
        power = power_raw * 61.035 * 0.001 * 25.0; // Convert to watts (calibration dependent)
        //ESP_LOGI(TAG, "Power: %f W", power);
        ESP_LOGI(TAG, "Power: %f mW", power);
        sensor1.Pow = power;
        /*energy = energy_prev + (power+power_prev)/2.0;
        ESP_LOGI(TAG, "Energy: %f Ws", energy);
        power_prev = power;
        energy_prev = energy;*/
    }
}

// Function to write a command to the BH1750 sensor
esp_err_t bh1750_write_command(uint8_t command) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (BH1750_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, command, true));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    return ret;
}

// BH1750 Reset Function
void bh1750_reset() {
    bh1750_write_command(BH1750_CMD_POWER_ON);
    vTaskDelay(pdMS_TO_TICKS(10));  // Wait a bit
    bh1750_write_command(BH1750_CMD_RESET);
    vTaskDelay(pdMS_TO_TICKS(10));  // Wait a bit
    bh1750_write_command(BH1750_CMD_START);
    vTaskDelay(pdMS_TO_TICKS(10));  // Wait a bit
}

// Function to read light intensity data from BH1750
esp_err_t bh1750_read_data(uint16_t *data) {
    uint8_t buf[2];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    ESP_ERROR_CHECK(i2c_master_start(cmd));
    ESP_ERROR_CHECK(i2c_master_write_byte(cmd, (BH1750_SENSOR_ADDR << 1) | I2C_MASTER_READ, true));
    ESP_ERROR_CHECK(i2c_master_read(cmd, buf, sizeof(buf), I2C_MASTER_LAST_NACK));
    ESP_ERROR_CHECK(i2c_master_stop(cmd));
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    if (ret == ESP_OK) {
        *data = (buf[0] << 8) | buf[1]; // Combine high and low bytes
    }
    return ret;
}

void read_bh1750_data(void){
    uint16_t lux = 0;
    esp_err_t ret = bh1750_read_data(&lux);
    if (ret == ESP_OK) {
        float lux_value = lux / 1.2; // Convert to lux (from raw data)
        ESP_LOGI(TAG1, "Ambient Light: %.2f lux", lux_value);
        sensor1.Light = lux_value;
    } else {
        ESP_LOGE(TAG1, "Failed to read data from BH1750");
    }
}