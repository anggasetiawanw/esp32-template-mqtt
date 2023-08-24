#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_sntp.h"
#include "esp_system.h"
#include "esp_tls.h"
#include "mqtt_client.h"

#define DEFAULT_WIFI_SSID "SSID"
#define DEFAULT_WIFI_PASS "PASSWORD"

const char *MQTT_HOST = "HOST-MQTT";
const char *MQTT_USERNAME = "USERNAME";
const char *MQTT_PASSWORD = "PASSWORD";

esp_mqtt_client_config_t mqttCfg;
esp_mqtt_client_handle_t mqttClient;

const char *topic = "TOPIC-MQTT";

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event) {
    if (event->event_id == MQTT_EVENT_CONNECTED) {

        esp_mqtt_client_subscribe(mqttClient, topic, 0);

    } else if (event->event_id == MQTT_EVENT_DISCONNECTED) {
        ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_DISCONNECTED", event->event_id);

    } else if (event->event_id == MQTT_EVENT_SUBSCRIBED) {
        ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_SUBSCRIBED", event->msg_id, event->event_id);
    } else if (event->event_id == MQTT_EVENT_UNSUBSCRIBED) {
        ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_UNSUBSCRIBED", event->msg_id, event->event_id);
    } else if (event->event_id == MQTT_EVENT_PUBLISHED) {
        ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_PUBLISHED", event->event_id);
    } else if (event->event_id == MQTT_EVENT_DATA) {
        ESP_LOGI("TEST", "MQTT msgid= %d event: %d. MQTT_EVENT_DATA", event->msg_id, event->event_id);
        ESP_LOGI("TEST", "Topic length %d. Data length %d", event->topic_len, event->data_len);
        ESP_LOGI("TEST", "Incoming data: %.*s %.*s\n", event->topic_len, event->topic, event->data_len, event->data);

        // ISI CODINGAN DISIINI JIKA DATA MASUK

    } else if (event->event_id == MQTT_EVENT_BEFORE_CONNECT) {
        ESP_LOGI("TEST", "MQTT event: %d. MQTT_EVENT_BEFORE_CONNECT", event->event_id);
    }
    return ESP_OK;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    Serial.println("---WIFI INITIALIZATION---");
    WiFi.mode(WIFI_STA);  // explicitly set mode, esp defaults to STA+AP
    // preferences.putBool(DEFAULTSSID_STATUS_ADDRESS, false);
    WiFi.begin(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println("---END WIFI INITIALIZATION---");

    Serial.println("---MQTT INITIALIZATION---");
    mqttCfg.host = MQTT_HOST;
    mqttCfg.port = 1883;
    mqttCfg.keepalive = 60;
    mqttCfg.username = MQTT_USERNAME;
    mqttCfg.password = MQTT_PASSWORD;
    mqttCfg.event_handle = mqtt_event_handler;
    mqttCfg.lwt_msg = "0";
    mqttCfg.lwt_msg_len = 1;
    mqttCfg.lwt_retain = 1;
    mqttCfg.transport = MQTT_TRANSPORT_OVER_TCP;

    Serial.println("   Connecting to MQTT Server....");
    mqttClient = esp_mqtt_client_init(&mqttCfg);
    esp_err_t err = esp_mqtt_client_start(mqttClient);
    ESP_LOGI("TEST", "Client connect. Error = %d %s", err, esp_err_to_name(err));
    if (err != ESP_OK) {
        Serial.println("[MQTT] Failed to connect to MQTT server at setup");
        // rebootEspWithReason("   Failed MQTT Connection");
    }
    Serial.println("---END MQTT INITIALIZATION---");
}

void loop() {
    // put your main code here, to run repeatedly:
}
