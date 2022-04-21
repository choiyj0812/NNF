//This code is the thing for MTHM
//(Measurement Temperature, Humidity Module)
#include <ESP8266WiFi.h>
#include <DHT.h>

#ifndef STASSID
#define STASSID "WiFi's name"
#define STAPSWD "WiFi's password"
#endif

const char* ssid = STASSID;
const char* password = STAPSWD;

const char* host = "host's address";
const uint16_t port = 17;

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht11(DHTPIN, DHTTYPE);
#define Soil_Sensor A0

void setup() {
  Serial.begin(9600);
  dht11.begin();

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float Temp_C = dht11.readTemperature();
  float Temp_F = dht11.readTemperature(true);
  int Soil_Data = analogRead(Soil_Sensor);
  if(isnan(Temp_C) || isnan(Temp_F) || isnan(Soil_Data)){
    Serial.println("Failed to read from DHT11 Sensor and Soil Sensor!");
    return;
  }
  
  static bool wait = false;

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(" : ");
  Serial.println(port);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  Serial.println("sending data to server");
  if (client.connected()) {
    client.println(Temp_C);
    client.println(Temp_F);
  }

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("WiFi connecting clear");
      client.stop();
      delay(60000);
      return;
    }
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  if (wait) {
    delay(300000); //delay 5 minutes
  }
  wait = true;
}
