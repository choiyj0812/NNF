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

void setup() {
  Serial.begin(9600);

  //인터넷과의 연결시도
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //연결 실패 시 무한 시도
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //연결 성공 시 IP주소 출력
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

  //클라이언트로 서버와 연결 시도
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  //연결 성공 시 데이터 전송
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from ESP8266");
  }

  //받은 데이터가 없으면 대기 후 연결 해제
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  //서버로부터 데이터를 받음
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

  if (wait) {
    delay(300000);
  }
  wait = true;
}
