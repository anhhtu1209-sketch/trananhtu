// Cach 1: delay() — chu ky thuc = (thoi gian viec) + delay
#include "common.h"
ThongKe tk;

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  Serial.println("# vd1_delay: gui 'b' de bat/tat CPU ban");
}

void loop() {
  docLenh();
  tk.ghiNhan(micros());                 // do khoang giua 2 lan chay
  digitalWrite(LED1, !digitalRead(LED1));
  busyWork();                           // cong viec khac cua chuong trinh
  delay(timerMs);                       // block CPU
  inThongKe("delay", tk);
}
