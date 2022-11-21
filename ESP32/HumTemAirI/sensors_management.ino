#include "DHT.h"
#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <AsyncTCP.h>

/* Change the following constants with your wifi data */
#define WIFI_SSID "PUT HERE YOUR WIFI SSID"
#define WIFI_PASSWORD "PUT HERE YOUR WIFI PASSWORD"

/*  
  Mosquitto MQTT Broker local ip address
  (e.g 192.168.1.170)
*/
#define MQTT_HOST IPAddress(192, 168, 1, 170)

/* 
  For a cloud MQTT broker, type the domain name
  (e.g #define MQTT_HOST "example.com")
*/
#define MQTT_PORT 1883

/* Temperature MQTT Topics */
#define MQTT_PUB_TEMP "sensors/temperature"
#define MQTT_PUB_HUM  "sensors/umidity"
#define MQTT_PUB_AIR  "sensors/air_quality"
#define MQ135_THRESHOLD_1 5000 // Good air threshold

/* Digital pin connected to the DHT sensor */
#define DHTPIN 4  

/* Sensor is connected to GPIO 2 (Analog ADC2_CH2) */  
const int senPin = 36;

/* Uncomment whatever DHT sensor type you're using */ 
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)   

/* Initialize DHT sensor */
DHT dht(DHTPIN, DHTTYPE);

/* Variables to hold sensor readings */ 
float temp;
float hum;

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

void connectToWifi() {
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  Serial.printf("[WiFi-event] event: %d\n", event);
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("Disconnected from MQTT.");
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) {
  Serial.print("Publish acknowledged.");
  Serial.print("packetId: ");
  Serial.println(packetId);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  dht.begin();
  
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  /* If your broker requires authentication (username and password), set them below */
  //mqttClient.setCredentials("REPlACE_WITH_YOUR_USER", "REPLACE_WITH_YOUR_PASSWORD");

  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  /*
    Every X number of seconds (interval = 10 seconds)
    it publishes a new MQTT message
  */  
  if (currentMillis - previousMillis >= interval) {
    /* Save the last time a new reading was published */ 
    previousMillis = currentMillis;
    /* New DHT sensor readings */ 
    hum = dht.readHumidity();
    /* Read temperature as Celsius (the default) */
    temp = dht.readTemperature();

    /* Uncomment to read temperature as Fahrenheit (isFahrenheit = true) */ 
    //temp = dht.readTemperature(true);

    /* Read air quality */ 
    int air = analogRead(senPin);
    
   if(aire < MQ135_THRESHOLD_1){
      Serial.print("Fresh Air: ");
    } else {
      Serial.print("Poor Air:");
    }
   Serial.print(air); // analog data
   Serial.println("PP");

   Serial.print("Temperature --> "); 
   Serial.print(temp); // analog data
   Serial.print("Humidity --> ");
   Serial.print(hum); // analog data

    // Check if any reads failed and exit early (to try again).
    if (isnan(temp) || isnan(hum)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    
    /* Publish an MQTT message on topic "sensors/temperature" */
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_TEMP, packetIdPub1);
    Serial.printf("Message: %.2f \n", temp);


    /* Publish an MQTT message on topic "sensors/humidity" */
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", MQTT_PUB_HUM, packetIdPub2);
    Serial.printf("Message: %.2f \n", hum);


    /* Publish an MQTT message on topic "sensors/air_quality" */
    uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_AIR, 1, true, String(aire).c_str());                            
    //Serial.print("Publishing on topic %s at QoS 1, packetId: %i", MQTT_PUB_AIRE, packetIdPub3);
    Serial.print(air);
    
  }
}
