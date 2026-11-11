#pragma once
#include <Arduino.h>

#define LED1 2                         // dung chung mach TH1
static const uint32_t timerMs   = 100; // chu ky dat
static const uint32_t CPU_BAN_MS = 150; // "CPU ban": moi vong loop() bi chiem 150ms

volatile bool cpuBan = false;          // gui ky tu 'b' qua Serial de bat/tat

// Thong ke do lech (jitter) giua khoang thuc te va chu ky dat, don vi us.
struct ThongKe {
  volatile uint32_t lanCuoi = 0, soMau = 0, lechMax = 0;
  volatile uint64_t tongLech = 0;
  void IRAM_ATTR ghiNhan(uint32_t now) {
    if (lanCuoi) {
      int32_t lech = (int32_t)(now - lanCuoi) - (int32_t)(timerMs * 1000);
      uint32_t a = lech < 0 ? -lech : lech;
      tongLech += a;
      if (a > lechMax) lechMax = a;
      soMau++;
    }
    lanCuoi = now;
  }
};

inline void docLenh() {                // bat/tat tai gia
  while (Serial.available()) {
    if (Serial.read() == 'b') {
      cpuBan = !cpuBan;
      Serial.printf("# CPU ban = %d\n", cpuBan);
    }
  }
}

inline void busyWork() {               // chiem CPU nhung van cho phep ngat
  if (!cpuBan) return;
  uint32_t t = millis();
  while (millis() - t < CPU_BAN_MS) delayMicroseconds(100);
}

// In moi 2s theo dinh dang Teleplot ">ten:gia_tri"
inline void inThongKe(const char* ten, ThongKe& tk) {
  static uint32_t lanIn = 0;
  if (millis() - lanIn < 2000) return;
  lanIn = millis();
  noInterrupts();
  uint32_t n = tk.soMau, mx = tk.lechMax;
  uint64_t tong = tk.tongLech;
  interrupts();
  uint32_t tb = n ? (uint32_t)(tong / n) : 0;
  Serial.printf(">%s_lech_tb_us:%u\n>%s_lech_max_us:%u\n", ten, tb, ten, mx);
  Serial.printf("# %s: %u mau, lech TB=%u us, lech max=%u us, cpuBan=%d\n",
                ten, n, tb, mx, cpuBan);
}
