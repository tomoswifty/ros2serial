#include <Arduino.h>
#include <ArduinoJson.h>

// グローバル変数
float g_linear_x = 0.0;
float g_angular_z = 0.0;

// 送受信用のJsonDocument
StaticJsonDocument<200> receive_data;
StaticJsonDocument<200> send_data;

void receiveJson() {
  if (Serial.available() > 0) {
    DeserializationError err1;
    while (!(err1.code() == DeserializationError::Ok)) {
      String recived_serial_data = Serial.readStringUntil('\n');
      err1 = deserializeJson(receive_data, recived_serial_data);
    }
    g_linear_x  = receive_data["linear_x"] | 0.0;
    g_angular_z = receive_data["angular_z"] | 0.0;
  }
}

void sendJson() {
  // encoder_left, encoder_right に受信値を代入
  send_data["encoder_left"] = g_linear_x;
  send_data["encoder_right"] = g_angular_z;
  serializeJson(send_data, Serial);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  // 送受信データの初期化
//   receive_data["linear_x"] = 0.00;
//   receive_data["angular_y"] = 0.00;
  send_data["encoder_left"]  = 0.00;
  send_data["encoder_right"] = 0.00;

  receiveJson();
  sendJson();

  delay(10); // 通信負荷軽減のため
}
