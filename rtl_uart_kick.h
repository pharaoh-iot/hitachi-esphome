// 日立 TaiSEIA 原廠模組（RTL8720C）ESPHome 用的小修正
// 授權：LGPL-2.1（同 taixia 元件），見 LICENSE

#pragma once
// RTL8720C 用的兩個修正（LibreTiny 1.13.0）
#include <Arduino.h>
#include <Serial.h>
extern "C" {
#include <cmsis.h>
#include <hal_wdt.h>
}

// RingBufferN 的 _numElems 在中斷（++）和主程式（--）同時改，沒有保護；
// 撞到一次就永遠少算 1，最後一個位元組要等下一筆資料才看得到，taixia 收不到完整回應。
// 這裡每次用 head/tail 算出實際數量校正回來。
template <typename Tag, typename Tag::type M> struct RtlRob {
  friend typename Tag::type rtl_rx_buf_member(Tag) { return M; }
};
struct RtlRxBufTag {
  typedef SerialRingBuffer *SerialClass::*type;
  friend type rtl_rx_buf_member(RtlRxBufTag);
};
template struct RtlRob<RtlRxBufTag, &SerialClass::rxBuf>;

inline void rtl_uart0_heal() {
  SerialRingBuffer *buf = Serial0.*rtl_rx_buf_member(RtlRxBufTag());
  if (buf == nullptr)
    return;
  __disable_irq();
  int real = (buf->_iHead - buf->_iTail + LT_SERIAL_BUFFER_SIZE) % LT_SERIAL_BUFFER_SIZE;
  if (real != 0 && buf->_numElems != real)
    buf->_numElems = real;
  __enable_irq();
}

// LibreTiny 的 lt_reboot 只重置 CPU，Wi-Fi 沒重置，重開後常連不上；改用 Realtek OTA 同款的看門狗完整重置
inline void rtl_full_reset() { hal_misc_rst_by_wdt(); }
