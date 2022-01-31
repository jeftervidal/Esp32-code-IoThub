/**
 * A simple Azure IoT example for sending telemetry.
 */

#include <WiFi.h>
#include "Esp32MQTTClient.h"

// Please input the SSID and password of WiFi
const char* ssid     = "Bradock-2G";
const char* password = "josi280520";

//DHT11 Libs
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2
// Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

//#define DHTTYPE    DHT11     // DHT 11

#define DHTTYPE    DHT22     // DHT 22 (AM2302SharedAccessKey)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)


//

 sensors_event_t event_temperature;
 sensors_event_t event_humity;

DHT_Unified dht(DHTPIN, DHTTYPE);

/*String containing Hostname, Device Id & Device Key in the format:                         */
/*  "HostName=<host_name>;DeviceId=<device_id>;=<device_key>"                */
/*  "HostName=<host_name>;DeviceId=<device_id>;SharedAccessSignature=<device_sas_token>"    */
static const char* connectionString = "HostName=Teste1-IH.azure-devices.net;DeviceId=ESP32-Final-azure-aks;SharedAccessKey=qtQQiVscZJ3647uATd1GP6Ecf1RmRBk7ePBYn5EmDFY=";

static bool hasIoTHub = false;

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Starting connecting WiFi.");
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!Esp32MQTTClient_Init((const uint8_t*)connectionString))
  {
    hasIoTHub = false;
    Serial.println("Initializing IoT hub failed.");
    return;
  }
  hasIoTHub = true;

///jeff

delay(1000);
 
  dht.temperature().getEvent(&event_temperature );
  if (isnan(event_temperature.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event_temperature.temperature);
    Serial.println(F("°C"));
  }
 // Get humidity event and print its value.
  dht.humidity().getEvent(&event_humity);
  if (isnan(event_humity.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event_humity.relative_humidity);
    Serial.println(F("%"));
    
}
}
void loop() {


////

delay(1000);
 
  dht.temperature().getEvent(&event_temperature );
  if (isnan(event_temperature.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event_temperature.temperature);
    Serial.println(F("°C"));
  }
 // Get humidity event and print its value.
  dht.humidity().getEvent(&event_humity);
  if (isnan(event_humity.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event_humity.relative_humidity);
    Serial.println(F("%"));
  }

  ////
  Serial.println("start sending events.");
  if (hasIoTHub)
  {
    char buff[128];

    // replace the following line with your data sent to Azure IoTHub
         
         snprintf(buff, 128, "{Mini Projeto integrador - ESP - IoTHub/EventHub - Conteiner - AKS \", \"temp\": %.2f, \"hum\": %.2f}",event_temperature.temperature,event_humity.relative_humidity);
         Serial.println (buff);
    ///
   
    
    if (Esp32MQTTClient_SendEvent(buff))
    {
      Serial.println("Sending data succeed");
    }
    else
    {
      Serial.println("Failure...");
    }
    delay(1000);
  }
}
