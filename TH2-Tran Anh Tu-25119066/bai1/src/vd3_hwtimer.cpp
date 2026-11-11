// Cach 3: timer phan cung — ngat dung chu ky, khong phu thuoc loop()
#include "common.h"
ThongKe tk;
hw_timer_t* hwTimer = nullptr;

void IRAM_ATTR onTimer() {            // ISR: ngan gon, khong Serial
  tk.ghiNhan(micros());
}

void khoiTaoTimer() {
  hwTimer = timerBegin(0, 80, true);  // timer0, chia 80 -> 1 tick = 1us
  timerAttachInterrupt(hwTimer, &onTimer, true);
  timerAlarmWrite(hwTimer, timerMs * 1000, true);
  timerAlarmEnable(hwTimer);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  khoiTaoTimer();
  Serial.println("# vd3_hwtimer: gui 'b' de bat/tat CPU ban");
}

void loop() {
  docLenh();
  busyWork();                         // CPU ban nhung ISR van chay dung gio
  inThongKe("hwtimer", tk);
}
