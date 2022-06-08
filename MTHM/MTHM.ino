#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define SERVER_IP "192.168.1.42"

#ifndef STASSID
#define STASSID "ssid"
#define STAPSWD "password"
#endif

#define SOIL_SENSOR 14
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {

  Serial.begin(115200);
  dht.begin();

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSWD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  float Temp_C = dht.readTemperature();
  float Temp_F = dht.readTemperature(true);
  int Soil_Data = analogRead(Soil_Sensor);
  if(isnan(Temp_C) || isnan(Temp_F) || isnan(Soil_Data)){
    Serial.println("Failed to read from DHT11 Sensor and Soil Sensor!");
    return;
  }
  
  // wait for WiFi connection
  // 와이파이 연결 시도
  // 와이파이 연결 성공 시 작동
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin\n");
    // configure traged server and url
    // 서버와 url로 연결, http 통신 시작
    http.begin(client, "http://" SERVER_IP "/postplain/"); //HTTP
    // 헤더 설정
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST\n");
    // start connection and send HTTP header and body
    // 연결 시작, http header와 body를 전송
    int httpCode = http.POST("{\"hello\":\"world\"}");

    // httpCode will be negative on error
    // httpcode는 에러가 났을 때 음수
    // httpcode가 양수일 때 = 전송과정에서 에러가 발생하지 않음
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // header가 전송되었고, 서버에서 header를 처리함
      Serial.printf("[HTTP] POSTED code: %d\n", httpCode);

      // file found at server
      // 서버로부터 파일을 찾음
      // 전송 상태가 올바르면 작동
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println("received payload : <<");
        Serial.println(payload);
        Serial.println(">>");
      }
    }
    // httpcode가 음수일 때 = 전송할 때 에러 발생
    else {
      Serial.printf("[HTTP] POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(10000);
}
