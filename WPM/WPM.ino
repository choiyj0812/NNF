//This code is the thing for WPM(Water Plants Module)
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "WiFi's name"
#define STAPSK  "WiFi's password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

const char* host = "host's address";
const uint16_t port = 17;

#define Motor_1 7
#define Motor_2 8

void setup() {
  Serial.begin(9600);
  pinMode(Motor_1, OUTPUT);
  pinMode(Motor_2, OUTPUT);
  delay(30000);

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
  static bool wait = false;
  static bool Mode = false;

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

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("WiFi connecting clear");
      client.stop();
      delay(60000);
      return;
    }
  }

  String command = "";
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    command += ch;
  }
  Serial.println(command);

  if(command == "ON") Mode = true;
  else if(command == "OFF") Mode = false;
  if(Mode){
    digitalWrite(Motor_1, HIGH);
    digitalWrite(Motor_2, LOW);
  } else{
    digitalWrite(Motor_1, LOW);
    digitalWrite(Motor_2, LOW);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();

  if (wait) {
    delay(300000); //delay 5 minutes
  }
  wait = true;
}
