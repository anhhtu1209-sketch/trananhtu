// Cach 2: millis() non-blocking — nhip phu thuoc loop() quay lai nhanh den dau
#include "common.h"
ThongKe tk;
uint32_t loopTick = 0;

void updateLoopTimer() {
  if (millis() - loopTick < timerMs) return;      // chua den gio
  loopTick += timerMs;                            // giu nhip deu
  if (millis() - loopTick >= timerMs)
    loopTick = millis();                          // tre qua -> bat nhip
  tk.ghiNhan(micros());                           // do khoang
  digitalWrite(LED1, !digitalRead(LED1));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  Serial.println("# vd2_millis: gui 'b' de bat/tat CPU ban");
}

void loop() {
  docLenh();
  updateLoopTimer();
  busyWork();                                     // loop() bi chiem -> tre
  inThongKe("millis", tk);
}
