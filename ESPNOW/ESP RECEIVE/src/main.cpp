#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  float temp;
  float hum;
} Data;

Data data;

// Callback nhận
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));

  Serial.println("=== NHẬN DỮ LIỆU ===");

  Serial.print("Temp: ");
  Serial.print(data.temp);
  Serial.print(" °C | Hum: ");
  Serial.print(data.hum);
  Serial.println(" %");

  Serial.print("From MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.println("------------------");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.print("MAC Receiver: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW lỗi");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Receiver ready");
}

void loop() {
}