//This code is the thing for CPM(Compliment to Plants Module)
#include <DFPlayer_Mini_Mp3.h>
#include <ESP8266WiFi.h>

#ifndef STASSID
#define STASSID "ssid"
#define STAPSWD "password"
#endif

const char* ssid = STASSID;
const char* password = STAPSWD;

const char* host = "link";
const uint16_t port = 17;

#define SoundRx Rx
#define SoundTx Tx
#define Person 1
#define LED1 2
#define LED2 3
int mp3_num = 1;
int mp3_max = 5;

void setup() {
  Serial.begin(9600);
  mp3_set_serial(Serial);
  mp3_set_volume(30);

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
  Serial.print("IP address : ");
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

  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("test text from CPM");
  }

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  String Data = "";
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Data += ch;
  }
  Serial.println(Data);

  if(Data == "ON") Mode = true;
  else if(Data == "OFF") Mode = false;
  if(Mode){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  } else{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  int PIR = digitalRead(Person);
  if(PIR){
    mp3_play(mp3_num);
    mp3_num++;
    if(mp3_num > mp3_max){
      mp3_num = 1;
    }
  }

  if (wait) {
    delay(300000);  //delay 5 minutes
  }
  wait = true;
}
