# TH2 — Timer, non-blocking và Deep Sleep

Họ tên: ........  MSSV: ........  Lớp: BCSE 2025

Phần cứng: ESP32 DevKit, mạch **dùng chung sơ đồ TH1** (LED GPIO2, nút GPIO4, biến trở GPIO34) — xem `sodo/GHI_CHU.md`.
Build: PlatformIO, `platform = espressif32@6.5.0` (Arduino core 2.x, vì dùng `timerBegin(0,80,true)`).

## Bài 1 — So sánh delay() / millis() / timer phần cứng
Code: `bai1/src/vd1_delay.cpp`, `vd2_millis.cpp`, `vd3_hwtimer.cpp`
(chọn `pio run -e <tên> -t upload`). Chu kỳ đặt 100 ms; gửi ký tự `b` qua Serial Monitor để bật/tắt "CPU bận"
(mỗi vòng loop() bị chiếm 150 ms). Chương trình in độ lệch (jitter) trung bình/lớn nhất theo định dạng Teleplot.

| Cách | Jitter TB (CPU rảnh) | Jitter max (CPU rảnh) | Jitter TB (CPU bận) | Jitter max (CPU bận) |
|---|---|---|---|---|
| delay() | ... | ... | ... | ... |
| millis() | ... | ... | ... | ... |
| timer phần cứng | ... | ... | ... | ... |

(Dán ảnh chụp Serial Monitor/đồ thị vào `anh/bai1_*.png`.)

**Đã quan sát được:** ...
**Kết luận:** (cách nào bám nhịp tốt khi CPU bận, cách nào không, và vì sao) ...

## Bài 2 — Deep sleep định giờ và so sánh các mức tiêu thụ điện
Code: `bai2/src/vd1_bien_giu.cpp` (RTC_DATA_ATTR vs biến thường), `vd2_thuc_day.cpp` (thức dậy chạy lại từ `setup()`, web AP),
`vd3_cac_che_do.cpp` (chuỗi Active → Modem-sleep → Light-sleep → Deep-sleep, kèm đánh thức bằng nút ngoài ext0).

Cách so sánh (gián tiếp, qua hành vi): LED, tick `millis()` trên Serial, kết nối Wi-Fi AP, giá trị `demThuong` (RAM thường) và `soLanThuc` (RTC), `esp_sleep_get_wakeup_cause()`.

| Chế độ | CPU | Wi-Fi/radio | RAM thường | Sau khi thức | Dấu hiệu quan sát được |
|---|---|---|---|---|---|
| Active | chạy | bật | giữ | — | ... |
| Modem-sleep | chạy | tắt theo chu kỳ | giữ | — | ... |
| Light-sleep | dừng | tắt | giữ | tiếp tục tại chỗ | ... |
| Deep-sleep | dừng | tắt | MẤT | chạy lại từ setup() | ... |

`esp_sleep_get_wakeup_cause()`: lần cấp nguồn đầu = `...`; thức bằng timer = `...`; thức bằng nút (ext0) = `...`

**Giới hạn:** không đo được dòng thật bằng ampere vì (1) ... (2) ...
**Kết luận:** ...

## Trả lời nhanh câu hỏi ôn tập (tự viết lại bằng lời của em)
1. ...
