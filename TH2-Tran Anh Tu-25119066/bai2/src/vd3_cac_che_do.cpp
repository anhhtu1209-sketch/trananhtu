// Bai tap: quan sat gian tiep 4 muc tieu thu dien qua hanh vi (khong do dong that)
// Chuoi: ACTIVE 5s -> MODEM-SLEEP 5s -> LIGHT-SLEEP 3x2s -> DEEP-SLEEP 8s (hoac nhan nut GPIO4)
// Mo rong: danh thuc deep sleep bang nut ngoai (ext0). Gia dinh nut noi GPIO4 -> GND,
// keo len noi: nhan = muc 0. Sua BTN_LEVEL neu mach TH1 khac.
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#define LED_PIN 2
#define BTN_PIN GPIO_NUM_4
#define BTN_LEVEL 0

RTC_DATA_ATTR int soLanThuc = 0;   // song sot deep sleep
int demThuong = 0;                 // mat sau deep sleep, giu qua light sleep

const char* tenNguyenNhan(esp_sleep_wakeup_cause_t c) {
  switch (c) {
    case ESP_SLEEP_WAKEUP_TIMER: return "TIMER";
    case ESP_SLEEP_WAKEUP_EXT0:  return "EXT0";
    case ESP_SLEEP_WAKEUP_UNDEFINED: return "UNDEFINED (cap nguon)";
    default: return "KHAC";
  }
}

void chay(const char* ten, uint32_t ms, uint32_t nhayMs) {
  Serial.printf("[%s] CPU chay, LED nhay %ums\n", ten, nhayMs);
  uint32_t t0 = millis(), tLed = 0, tIn = 0;
  while (millis() - t0 < ms) {
    if (millis() - tLed >= nhayMs) { tLed = millis(); digitalWrite(LED_PIN, !digitalRead(LED_PIN)); }
    if (millis() - tIn >= 1000)    { tIn = millis(); Serial.printf("  tick millis=%lu\n", millis()); }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  soLanThuc++; demThuong++;
  Serial.printf("\n=== BOOT === nguyen nhan=%s | soLanThuc(RTC)=%d | demThuong(RAM)=%d\n",
                tenNguyenNhan(esp_sleep_get_wakeup_cause()), soLanThuc, demThuong);

  // 1) ACTIVE: Wi-Fi AP bat, CPU chay lien tuc
  WiFi.softAP("ESP32-TH2");
  chay("ACTIVE", 5000, 100);

  // 2) MODEM-SLEEP: CPU van chay, radio duoc ngat theo chu ky
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  esp_wifi_set_ps(WIFI_PS_MAX_MODEM);
  chay("MODEM-SLEEP", 5000, 500);
  WiFi.mode(WIFI_OFF);

  // 3) LIGHT-SLEEP: CPU dung, thuc lai TIEP TUC TAI CHO, RAM thuong con nguyen
  for (int i = 1; i <= 3; i++) {
    demThuong += 10;
    Serial.printf("[LIGHT-SLEEP] lan %d, demThuong=%d, millis=%lu -> ngu 2s\n",
                  i, demThuong, millis());
    Serial.flush();
    digitalWrite(LED_PIN, LOW);
    esp_sleep_enable_timer_wakeup(2000000ULL);
    esp_light_sleep_start();
    Serial.printf("   ...thuc tai cho, demThuong=%d, millis=%lu (khong chay lai setup)\n",
                  demThuong, millis());
  }

  // 4) DEEP-SLEEP: thuc = khoi dong lai tu setup(), RAM thuong mat
  Serial.println("[DEEP-SLEEP] ngu 8s (hoac nhan nut GPIO4 de danh thuc bang ext0)");
  Serial.flush();
  digitalWrite(LED_PIN, LOW);
  rtc_gpio_pullup_en(BTN_PIN);
  rtc_gpio_pulldown_dis(BTN_PIN);
  esp_sleep_enable_ext0_wakeup(BTN_PIN, BTN_LEVEL);
  esp_sleep_enable_timer_wakeup(8ULL * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {}
