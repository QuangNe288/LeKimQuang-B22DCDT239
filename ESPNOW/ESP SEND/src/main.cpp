#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// 👉 ĐỔI MAC RECEIVER
uint8_t receiverMAC[] = {0x00, 0x4B, 0x12, 0x3B, 0x64, 0xAC};

typedef struct {
  float temp;
  float hum;
} Data;

Data data;

// Callback gửi
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUCCESS" : "FAIL");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);

  dht.begin();

  Serial.println("=== SENDER DHT11 ===");

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init FAILED");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  // cấu hình peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Add peer FAILED");
    return;
  }

  Serial.println("Sender ready");
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // ❗ kiểm tra lỗi DHT
  if (isnan(t) || isnan(h)) {
    Serial.println("Lỗi đọc DHT11!");
    delay(2000);
    return;
  }

  data.temp = t;
  data.hum  = h;

  // gửi dữ liệu
  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&data, sizeof(data));

  if (result == ESP_OK) {
    Serial.println("Send OK");
  } else {
    Serial.println("Send Error");
  }

  Serial.print("Temp: ");
  Serial.print(data.temp);
  Serial.print(" °C | Hum: ");
  Serial.print(data.hum);
  Serial.println(" %");

  Serial.println("-------------------");

  delay(2000); // DHT11 cần delay >= 2s
}