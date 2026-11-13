# 日立冷氣／除濕機原廠 Wi-Fi 模組刷 ESPHome（基本版）

把日立原廠 Wi-Fi 模組（**RC-W04XE**、**RC-W05XE**，晶片 RTL8720C）直接刷成 ESPHome，
不用另外加開發板，就能在 Home Assistant 本地控制，不經過雲端。

基本版內容：開關、模式、溫度／濕度、風量、風向等基本控制，機器狀態讀值，今日用電與累計用電。

> ⚠️ **風險自負**：刷機後原廠 App 無法使用，也會失去原廠保固，刷之前請自行評估。
> 本專案**不提供、也不教學**原廠韌體的取得與備份；原廠韌體是日立／江森自控的著作，**請勿散布**。
> ⚠️ **0x1000～0x3FFF 是每顆模組自己的系統資料，寫入時要保留，不能被覆蓋。**
> 本專案與日立公司無關，「日立」「Hitachi」及型號僅用於說明相容機型。

## 已測試機型

| 類型 | 型號 | 模組 |
|---|---|---|
| 窗型冷氣 | RA-40HR | RC-W04XE |
| 分離式冷氣 | RAS-50NJP1 | RC-W05XE |
| 除濕機 | RD-240HH、RD-360HH1、RD-18FC | RC-W04XE |

## 刷機方法（UART）

需要：USB to TTL 轉接板兩片（① 3.3V 訊號用、② 5V 供電用）、330Ω 電阻一顆、
[ltchiptool](https://github.com/libretiny-eu/ltchiptool)。接線見 `wiring-rc-w04xe.svg`。

1. 模組從機器拆下來，不接機器。
2. 接線：
   - J14 三孔：GND → ①GND、中間孔 → ①RXD、方孔 → ①TXD（① 設在 3.3V）
   - ② 的 5V／GND → 模組 VBUS／GND（供電）
   - J15 方孔與圓孔之間接 330Ω（進入下載模式）
3. 先接好 330Ω 再通電，模組就會進入下載模式。
4. 用 ESPHome 編譯下方的設定檔，再用 ltchiptool 寫入。
   ⚠️ **0x1000～0x3FFF 是每顆模組自己的系統資料，寫入時要保留，不能被覆蓋。**
5. 拿掉 330Ω、斷電再通電，模組就會跑 ESPHome。之後更新可以用 OTA。

寫入中斷沒關係：330Ω 接著重新通電，再寫一次即可（一樣要注意：**0x1000～0x3FFF 是每顆模組自己的系統資料，寫入時要保留，不能被覆蓋。**）

編譯和寫入的詳細步驟，請詢問 [Claude](https://claude.ai)：把這份說明和設定檔給它，請它一步一步帶你做。

## 設定檔

| 檔案 | 用途 |
|---|---|
| `hitachi-ac.yaml` | 冷氣基本設定檔 |
| `hitachi-dehumidifier.yaml` | 除濕機基本設定檔 |
| `example-ac.yaml`、`example-dehumidifier.yaml` | 每台機器的小檔案範例，改名稱即可 |
| `rtl_uart_kick.h` | RTL8720C 序列埠修正（放在同一個資料夾） |
| `components/taixia/` | TaiSEIA 通訊元件（改自 tsunglung/taixia） |

連不上 Wi-Fi 時，5 分鐘後會開熱點（名稱＝主機名稱，密碼 `pharaoh11-14`），用手機連上即可設定 Wi-Fi。

## 授權與致謝

- 本專案以 **GNU LGPL-2.1** 授權，全文見 `LICENSE`。
- `components/taixia/` 改自 [tsunglung/taixia](https://github.com/tsunglung/taixia)（LGPL-2.1，commit 581bd57），感謝原作者。
  修改處標有「本機修改」註解：送指令後等待回應改用 `millis()` 計時（RTL8720C 上原本會太早逾時）、
  binary_sensor 狀態更新修正、冷氣運轉項目（preset）重設修正。
- 程式的修改、整理與測試，由 Pharaoh 與 AI 助理 [Claude](https://claude.ai)（Anthropic）一起完成，感謝 Claude 的協助。
- 使用 [ESPHome](https://esphome.io)（Python 部分 MIT、C++ 部分 GPLv3）與 [LibreTiny](https://github.com/libretiny-eu/libretiny)（MIT）。
- 如果你把用這些檔案編譯的韌體提供給別人，依 ESPHome 的 GPLv3 規定，請同時提供原始碼。
