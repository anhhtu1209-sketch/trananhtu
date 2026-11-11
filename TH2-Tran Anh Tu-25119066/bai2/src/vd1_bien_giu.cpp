// Deep sleep dinh gio 10s: bien RTC giu, bien thuong mat
#include <Arduino.h>
#include "esp_sleep.h"
#define LED_PIN 2

RTC_DATA_ATTR int soLanThuc = 0;  // GIU qua deep sleep (vung nho RTC)
int demThuong = 0;                // MAT qua deep sleep (RAM thuong)

void lamDeepSleep() {
  soLanThuc++;                    // RTC: cong don
  demThuong++;                    // thuong: se mat
  Serial.printf("soLanThuc(RTC)=%d  demThuong(RAM)=%d -> ngu 10s\n",
                soLanThuc, demThuong);
  Serial.flush();
  digitalWrite(LED_PIN, LOW);     // tat LED
  esp_sleep_enable_timer_wakeup(10ULL * 1000000ULL);  // 10 giay
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);    // LED sang = dang thuc
  Serial.printf("\nThuc day, nguyen nhan = %d (0 = cap nguon, 4 = timer)\n",
                (int)esp_sleep_get_wakeup_cause());
  delay(3000);                    // thuc 3s de quan sat
  lamDeepSleep();
}

void loop() {}                    // khong bao gio toi day
