// Thuc day chay lai tu setup(): Wi-Fi AP + web hien nguyen nhan thuc va bien RTC/thuong
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_sleep.h"
#define LED_PIN 2

RTC_DATA_ATTR int soLanThuc = 0;
int demThuong = 0;
WebServer server(80);
uint32_t tBatDau, tLed;

const char* tenNguyenNhan(esp_sleep_wakeup_cause_t c) {
  switch (c) {
    case ESP_SLEEP_WAKEUP_TIMER: return "TIMER";
    case ESP_SLEEP_WAKEUP_EXT0:  return "EXT0 (nut ngoai)";
    case ESP_SLEEP_WAKEUP_UNDEFINED: return "Cap nguon / reset (khong phai deep sleep)";
    default: return "Khac";
  }
}

void trangChu() {
  String s = "<meta charset=utf-8><meta http-equiv=refresh content=2>";
  s += "<h2>ESP32 sau khi thuc day</h2>";
  s += "<p>Nguyen nhan thuc: <b>" + String(tenNguyenNhan(esp_sleep_get_wakeup_cause())) + "</b></p>";
  s += "<p>soLanThuc (RTC_DATA_ATTR, giu): <b>" + String(soLanThuc) + "</b></p>";
  s += "<p>demThuong (RAM thuong, mat): <b>" + String(demThuong) + "</b></p>";
  s += "<p>Se ngu lai sau: " + String(30 - (millis() - tBatDau) / 1000) + " s</p>";
  server.send(200, "text/html", s);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  soLanThuc++;          // RTC: cong don qua cac lan ngu
  demThuong++;          // thuong: luon = 1 sau moi lan thuc
  Serial.printf("\nNguyen nhan thuc: %s | soLanThuc=%d demThuong=%d\n",
                tenNguyenNhan(esp_sleep_get_wakeup_cause()), soLanThuc, demThuong);
  WiFi.softAP("ESP32-TH2");
  server.on("/", trangChu);
  server.begin();
  tBatDau = millis();
}

void loop() {
  server.handleClient();
  if (millis() - tLed >= 250) {            // LED nhap nhay bao CPU con chay
    tLed = millis();
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
  if (millis() - tBatDau > 30000) {        // 30s sau lai ngu 10s
    digitalWrite(LED_PIN, LOW);
    esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);
    esp_deep_sleep_start();
  }
}
