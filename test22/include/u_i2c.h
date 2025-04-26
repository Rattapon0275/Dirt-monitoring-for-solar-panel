#include <stdio.h>
#include "driver/i2c.h"
#include "esp_log.h"

// I2C configuration
#define I2C_MASTER_SCL_IO        22    // GPIO number for I2C SCL
#define I2C_MASTER_SDA_IO        21    // GPIO number for I2C SDA
#define I2C_MASTER_NUM           I2C_NUM_0  // I2C port number
#define I2C_MASTER_FREQ_HZ       100000     // I2C frequency
#define I2C_MASTER_TIMEOUT_MS    1000       // I2C timeout in milliseconds

// INA226 configuration
#define INA226_ADDR              0x40  // I2C address of INA226
#define REG_CONFIG               0x00  // Configuration register
#define REG_SHUNT_VOLTAGE        0x01  // Shunt voltage register
#define REG_BUS_VOLTAGE          0x02  // Bus voltage register
#define REG_POWER                0x03  // Power register
#define REG_CURRENT              0x04  // Current register

// BH1750 configuration
#define BH1750_SENSOR_ADDR         0x23  // BH1750 I2C address
#define BH1750_CMD_START           0x10  // Continuously H-Resolution Mode
#define BH1750_CMD_RESET           0x07
#define BH1750_CMD_POWER_ON        0x01

esp_err_t i2c_master_init(void);
esp_err_t write_register(uint8_t reg, uint16_t value);
esp_err_t read_register(uint8_t reg, uint16_t *value);
void read_ina226_data(void);

esp_err_t bh1750_write_command(uint8_t command);
esp_err_t bh1750_read_data(uint16_t *data);
void bh1750_reset();
void read_bh1750_data(void);