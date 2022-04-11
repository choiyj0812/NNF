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

//SoundRx - MP3Tx, SoundTx - MP3Rx
#define SoundRx Rx
#define SoundTx Tx
#define Person 1
#define LED 2
//mp3_num : Number of MP3 files to run
//mp3_max : Number of MP3 files
int mp3_num = 1;
int mp3_max = 5;

void setup() {
  Serial.begin(9600);
  mp3_set_serial(Serial);
  mp3_set_volume(30);

  //Attempt to connect to the Internet
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //If the connection fails, an infinite number of attempts
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If the connection fails, keep trying to connect
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  static bool wait = false;

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(" : ");
  Serial.println(port);

  //Attempt to connect with the server
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  //Send test data if connection is successful
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

  //Wait for a certain amount of time before disconnecting if no more data is received
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  //Received data from server
  Serial.println("receiving from remote server");
  String Data_String = "";
  int Data_Int = 0;
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Data += ch;
  }
  Data_Int = Data_String.toInt();
  Serial.println(Data_String);
  
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  //PIR sensor
  int PIR = digitalRead(Person);
  if(PIR){
    mp3_play(mp3_num);
    mp3_num++;
    if(mp3_num > mp3_max){
      mp3_num = 1;
    }
  }

  if (wait) {
    delay(30000);
  }
  wait = true;
}
