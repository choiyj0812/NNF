#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DFPlayer_Mini_Mp3.h>

#define ssid "user"
#define pswd "00000000"

#define SoundRx Rx
#define SoundTx Tx
#define Person 1
#define LED1 2
#define LED2 3
int mp3_num = 1;
int mp3_max = 5;

ESP8266WiFiMulti WiFiMulti;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  mp3_set_serial(Serial);
  mp3_set_volume(30);
  delay(1);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, pswd);

}

void loop() {
  static bool Mode = false;
  
  // wait for WiFi connection
  // 와이파이 연결
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // 연결 성공 시
    if (http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html")) {  // HTTP


      Serial.print("[HTTP] GET...\n");
      // start connection and send HTTP header
      // 서버와의 연결 시도 및 http 헤더 전송
      int httpCode = http.GET();

      // httpCode will be negative on error
      // 에러가 나지 않는다면 httpcode는 양수
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // http 헤더가 전송된 뒤 서버응답 헤더가 처리됨
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        // 서버에서 파일을 찾음
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          // 문자열을 받아옴
          String payload = http.getString();
          Serial.println(payload);
        }
      }
      // httpcode < 0
      else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    }
    // 연결 실패 시
    else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  }

  // 입력받은 문자열에 따라 Mode 변경
  if(payload == "ON") Mode = true;
  else if(payload == "OFF") Mode = false;

  // Mode에 따라 LED를 조절
  if(Mode){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }
  else{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }

  // 사람이 감지되면 칭찬 재생
  int PIR = digitalRead(Person);
  if(PIR){
    mp3_play(mp3_num);
    mp3_num++;
    if(mp3_num > mp3_max) mp3_num = 1;
  }

  // 10초 대기
  delay(10000);
}
