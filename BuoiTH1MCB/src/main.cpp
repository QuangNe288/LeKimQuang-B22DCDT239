#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 4        // GPIO nối DATA
#define DHTTYPE DHT11   // Chọn DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.println("DHT11 test with ESP32");
}

void loop() {
  float h = dht.readHumidity();      // Độ ẩm
  float t = dht.readTemperature();   // Nhiệt độ (°C)

  // Kiểm tra lỗi đọc
  if (isnan(h) || isnan(t)) {
    Serial.println("Không đọc được dữ liệu từ DHT11!");
    return;
  }

  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print(" °C | Do am: ");
  Serial.print(h);
  Serial.println(" %");

  delay(2000); // DHT11 cần delay >= 2s
}
