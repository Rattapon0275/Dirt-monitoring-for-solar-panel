#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "esp_task_wdt.h"

#include "u_main.h"
#include "u_adc.h"
#include "u_gpio.h"
#include "u_i2c.h"
#include "u_spi.h"

#define WIFI_SSID      "Mai_Chai_Hotpot_Na"
#define WIFI_PASSWORD  "12022544pon"

#define NETPIE_BROKER_URI "mqtt://broker.netpie.io:1883"
#define NETPIE_CLIENT_ID  "8f1d7d10-0090-4ecf-89c8-05886c21dbb4"
#define NETPIE_USERNAME   "5cXCmoAr3pC7U6AM3NXkoEK61gy9xJMG"
#define NETPIE_PASSWORD   "ekgZ6vngvzXWKdkBJMQcu3ohJy67LDNV"

#define WATCHDOG_TIMEOUT_SEC 5

static const char *TAG = "NETPIE_MQTT";
static EventGroupHandle_t wifi_event_group;
const int WIFI_CONNECTED_BIT = BIT0;
static esp_mqtt_client_handle_t mqtt_client = NULL;

const char* SensorTopic = "@msg/Sensor" ;
const char* FeedbackTopic = "@msg/Feedback" ;
const char* SendbackTopic = "@msg/Sendback" ;
const char* ShadowTopic ="@shadow/data/update";

char message[100];
SENSOR_DATA sensor1;
uint16_t calibration_value = 838;

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "WiFi disconnected, reconnecting...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ESP_LOGI(TAG, "WiFi connected");
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Connected to NETPIE");
            esp_mqtt_client_subscribe(client, SensorTopic, 0);
            esp_mqtt_client_subscribe(client, FeedbackTopic, 0);
            esp_mqtt_client_subscribe(client, SendbackTopic, 0);
            esp_mqtt_client_subscribe(client, ShadowTopic, 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Disconnected from NETPIE");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received message from NETPIE:");
            ESP_LOGI(TAG, "Topic: %.*s", event->topic_len, event->topic);
            ESP_LOGI(TAG, "Data: %.*s", event->data_len, event->data);
            if(strncmp(event->topic, FeedbackTopic, event->topic_len) == 0){
                if(strncmp(event->data, "Clean", event->data_len) == 0 && sensor1.reqclean == 0){
                    sensor1.reqclean = 1;
                    sensor1.cleantime = 0;
                }else if(strncmp(event->data, "Stop", event->data_len) == 0 && sensor1.reqclean == 1){                    
                    sensor1.reqclean = 0;
                    sensor1.cleantime = 0;
                }
            }            
            break;
        default:
            break;
    }
}

void mqtt_send_message(const char *topic, const char *message) {
    if (mqtt_client) {
        esp_mqtt_client_publish(mqtt_client, topic, message, 0, 1, 0);
        ESP_LOGI(TAG, "Sent message to %s: %s", topic, message);
    } else {
        ESP_LOGW(TAG, "MQTT client is not initialized");
    }
}

static void mqtt_app_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .address.uri = NETPIE_BROKER_URI,
        },
        .credentials = {
            .client_id = NETPIE_CLIENT_ID,
            .username = NETPIE_USERNAME,
            .authentication.password = NETPIE_PASSWORD,
        }
    };
    
    xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, portMAX_DELAY);
    ESP_LOGI(TAG, "Starting MQTT client");
    
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, mqtt_client);
    esp_mqtt_client_start(mqtt_client);
}

static void wifi_init(void) {
    wifi_event_group = xEventGroupCreate();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_got_ip);
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
        },
    };
    
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
}

void app_main(void) {
    // Disable the Task Watchdog Timer
    esp_task_wdt_deinit();

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    wifi_init();
    mqtt_app_start();

    uint16_t config = 0x4127;
    write_register(REG_CONFIG, config);

    i2c_master_init();
    bh1750_write_command(BH1750_CMD_START);
    init_adc();
    init_spi();
    init_gpio();

    sensor1.reqclean = 0;

    while (1) {        
        write_register(0x05, calibration_value);

        read_bh1750_data();
        read_rain_data();
        read_ina226_data(); 
        read_dirt_data();
        if(sensor1.Rain == 1){
            sensor1.Dirt = 0;
        }
        sprintf(message, "{\"data\" : {\"Dirt\" : %d }}", sensor1.Dirt);
        mqtt_send_message(ShadowTopic, message);
        read_max6675_data();
        sprintf(message, "{ \"Sensor\", %.1f,%.1f,%.1f,%d,%d,%.1f,%d, \"Sensor\"}", sensor1.Volt, sensor1.Amp, 
            sensor1.Pow, sensor1.Light, sensor1.Rain, sensor1.Temp, sensor1.Dirt);
        mqtt_send_message(SensorTopic, message);

        if(sensor1.clean == 0 && sensor1.reqclean == 1){
            on_off_pump(1);
            mqtt_send_message(SendbackTopic, "1");
            sensor1.clean = 1;
        }
        if(sensor1.clean == 1 && sensor1.reqclean == 0){
            on_off_pump(0);
            mqtt_send_message(SendbackTopic, "0");
            sensor1.clean = 0;
        }
        if(sensor1.clean == 1){
            sensor1.cleantime++;
            //Clean Time = Delay(s) x cleantime = 10 x 1 = 10s
            if(sensor1.cleantime > 5){
                sensor1.cleantime = 0;
                sensor1.reqclean = 0;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(2000));        
    }
}

