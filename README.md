# 🖥️ Retro OS Boot Simulator (ESP32-S3)

<p align="center">
  <img src="https://img.shields.io/badge/ESP32--S3-Boot%20Simulator-red?style=for-the-badge&logo=espressif" alt="ESP32-S3">
  <img src="https://img.shields.io/badge/TFT-ST7789%20240×320-blue?style=for-the-badge" alt="ST7789">
  <img src="https://img.shields.io/badge/Style-Y2K%20Retro-purple?style=for-the-badge" alt="Retro">
  <img src="https://img.shields.io/badge/Arduino-C%2B%2B-orange?style=for-the-badge&logo=arduino" alt="Arduino">
</p>

<p align="center">
  <b>Boot Animation • Welcome Screen • Error Dialogs • Blue Screen (Crash Simulation)</b>
</p>

---

## 📌 প্রজেক্ট সম্পর্কে

**Retro OS Boot Simulator** হলো `ESP32-S3` এবং `ST7789 240×320` TFT ডিসপ্লের জন্য তৈরি একটি নস্টালজিক **Windows XP Desktop OS Simulation** Tool।

এই প্রজেক্টে পুরনো দিনের ডেস্কটপ অপারেটিং সিস্টেমের একটি সম্পূর্ণ বুট সিকোয়েন্স রিক্রিয়েট করা হয়েছে — Boot animation থেকে শুরু করে Welcome screen, Desktop error popups, এবং সবশেষে ক্লাসিক **Crash / Blue Screen** পর্যন্ত।

Toolটি চারটি ধাপে কাজ করে:

```text
Boot Animation (looped)
        ↓
Welcome Screen (static)
        ↓
Desktop + Error Dialogs (cascading popups)
        ↓
Blue Screen (Crash Screen)
```

প্রতিটি স্ক্রিন সম্পূর্ণ **আলাদা `.h` ফাইলে** সংরক্ষিত, তাই যেকোনো একটি স্ক্রিন নিজের ইচ্ছামতো Replace, Edit বা Disable করা যায় — বাকি অংশ স্পর্শ না করেই।

> ⚠️ **নোট:** এটি কোনো নির্দিষ্ট Commercial Operating System-এর অফিসিয়াল রেপ্লিকা নয়। এটি শুধুই শিক্ষামূলক ও নস্টালজিয়া-থিমড একটি Fan-made Simulation, RGB565 image data ব্যবহার করে তৈরি।

---

## ✨ প্রধান বৈশিষ্ট্য

- 🥾 **Boot Animation** — Multi-frame looping boot sequence
- 🖼️ **Welcome Screen** — Static full-screen welcome image
- ⚠️ **Error Dialogs** — Cascading popup-style error frames
- 💙 **Blue Screen (Crash Screen)** — আলাদা `Bsod.h`-এ সংরক্ষিত, স্বাধীনভাবে কন্ট্রোলযোগ্য
- 🧩 **Modular `.h` Structure** — প্রতিটি স্ক্রিন আলাদা ফাইলে, সহজে Customize করা যায়
- 🎞️ PROGMEM-based RGB565 Image Streaming (RAM Overflow এড়ানোর জন্য Row-by-Row Draw)
- 🔁 প্রতিটি Screen-এর Loop Count ও Delay আলাদাভাবে Configure করা যায়
- 👆 XPT2046 Touch সাপোর্ট (ঐচ্ছিক, ভবিষ্যতে Interactive ফিচারের জন্য)
- ⚡ ESP32-S3 + ST7789 SPI Display সাপোর্ট

---

# 🔌 Pin-to-Pin Connection (ESP32-S3)

## 🖥️ ST7789 TFT → ESP32-S3

| ST7789 Pin | ESP32-S3 | কাজ |
|---|---:|---|
| **VCC** | **3.3V** | পাওয়ার |
| **GND** | **GND** | গ্রাউন্ড |
| **CS** | **GPIO 10** | Chip Select |
| **RST** | **GPIO 8** | Reset |
| **DC** | **GPIO 9** | Data / Command |
| **MOSI** | **GPIO 11** | SPI Data |
| **SCK** | **GPIO 13** | SPI Clock |
| **MISO** | **GPIO 12** | SPI Data Out |
| **LED** | **3.3V** | Backlight |

---

## 👆 XPT2046 Touch → ESP32-S3

TFT এবং XPT2046 একই SPI bus ব্যবহার করে। তবে দুটির **CS আলাদা**।

| XPT2046 Pin | ESP32-S3 | কাজ |
|---|---:|---|
| **TCLK / T_CLK** | **GPIO 13** | SPI Clock |
| **T_CS** | **GPIO 7** | Touch Chip Select |
| **T_DIN** | **GPIO 11** | SPI MOSI |
| **T_DO** | **GPIO 12** | SPI MISO |
| **T_IRQ** | **GPIO 6** | Touch Interrupt |

### 🔄 Shared SPI

```text
ESP32-S3           ST7789           XPT2046
──────────────────────────────────────────────
GPIO 13 ───────── SCK ──────────── TCLK
GPIO 11 ───────── MOSI ─────────── T_DIN
GPIO 12 ───────── MISO ─────────── T_DO

GPIO 10 ───────── CS
GPIO 7  ──────────────────────────  T_CS
GPIO 9  ───────── DC
GPIO 8  ───────── RST
GPIO 6  ──────────────────────────  T_IRQ
```

---

# 📋 সম্পূর্ণ GPIO তালিকা

| GPIO | সংযুক্ত অংশ | Signal |
|---:|---|---|
| **GPIO 6** | XPT2046 | T_IRQ |
| **GPIO 7** | XPT2046 | T_CS |
| **GPIO 8** | ST7789 | RST |
| **GPIO 9** | ST7789 | DC |
| **GPIO 10** | ST7789 | CS |
| **GPIO 11** | TFT + Touch | MOSI / T_DIN |
| **GPIO 12** | TFT + Touch | MISO / T_DO |
| **GPIO 13** | TFT + Touch | SCK / TCLK |

> ⚠️ ESP32-S3-এর নির্দিষ্ট বোর্ড/মডিউল ভেদে GPIO availability ভিন্ন হতে পারে। আপনার বোর্ডের Pinout Diagram দেখে প্রয়োজনে Pin নাম্বার পরিবর্তন করে নিন।

---

# 🎞️ Boot Sequence

Device চালু হলে নিচের ক্রমে স্ক্রিনগুলো প্রদর্শিত হয়:

```text
1. Boot Screen   → 9 Frames, 3 Loops, 160ms/frame
2. Welcome Screen → Static Image, 2 Loops, 2000ms hold
3. Desktop + Error → 4 Frames, 2000ms/frame
4. Blue Screen (Crash) → Static Image, 4000ms hold
```

সম্পূর্ণ সিকোয়েন্স শেষ হলে `loop()` আবার প্রথম থেকে শুরু হয়।

---

# 📁 File Structure ও কাজ

```text
Retro-OS-Boot-Simulator/
│
├── RetroOS_BootSimulator_ESP32S3.ino
├── Boot.h
├── Welcome.h
├── Error.h
├── Bsod.h
├── README.md
└── images/
    └── preview.png
```

| ফাইল | কাজ | Editable Define |
|---|---|---|
| **Boot.h** | Boot Animation Frames | `BOOT_IMG_FRAMES`, `BOOT_IMG_W/H` |
| **Welcome.h** | Static Welcome Screen | `WELCOME_IMG_W/H` |
| **Error.h** | Desktop Error Dialog Frames | `ERROR_IMG_FRAMES`, `ERROR_IMG_W/H` |
| **Bsod.h** | Blue Screen / Crash Screen | `BSOD_IMG_W/H` |

প্রতিটি Screen সম্পূর্ণ **Independent** — চাইলে শুধু `Bsod.h` Replace করে নিজের Crash Screen বসানো যাবে, বাকি ফাইলে হাত না দিয়েই।

---

# ⚙️ Sequence Timing Customize করা

`.ino` ফাইলে প্রতিটি Function-এ Loop Count ও Delay সরাসরি পরিবর্তনযোগ্য:

```cpp
void showBootScreen() {
  for (int loopNum = 0; loopNum < 3; loopNum++) {   // Loop Count
    ...
    delay(160);                                      // Frame Delay
  }
}

void showBSOD() {
  drawProgmemImage(bsod_img, BSOD_IMG_W, BSOD_IMG_H);
  delay(4000);                                        // Hold Duration
}
```

| পরিবর্তন করতে চাইলে | কোথায় দেখুন |
|---|---|
| Boot Loop সংখ্যা | `showBootScreen()` এর `loopNum < 3` |
| Welcome Screen সময় | `showWelcomeScreen()` এর `delay(2000)` |
| Error Dialog সময় | `showDesktopWithError()` এর `delay(2000)` |
| Blue Screen সময় | `showBSOD()` এর `delay(4000)` |

---

# 🖼️ নিজের Image দিয়ে Screen পরিবর্তন করা

প্রতিটি `.h` ফাইল একটি RGB565 PROGMEM Array আকারে Image সংরক্ষণ করে। নতুন Image বসাতে চাইলে:

```text
1. নিজের Image কে 320×240 (বা প্রয়োজনমতো Resolution) এ রিসাইজ করুন
2. RGB565 PROGMEM Array এ কনভার্ট করুন (image-to-array কনভার্টার টুল ব্যবহার করুন)
3. সংশ্লিষ্ট .h ফাইলের Array পরিবর্তন করুন
4. W/H Define ঠিক করে Upload করুন
```

> 💡 প্রতিটি Screen আলাদা ফাইলে থাকায়, শুধু একটি Screen Replace করার সময় বাকি Boot/Welcome/Error/Bsod Data অক্ষত থাকে।

---

# 📚 প্রয়োজনীয় Library

Arduino IDE-এর Library Manager থেকে ইনস্টল করুন:

| Library | ব্যবহার |
|---|---|
| **Adafruit GFX Library** | Graphics |
| **Adafruit ST7735 and ST7789 Library** | ST7789 Display |
| **XPT2046_Touchscreen** | XPT2046 Touch (ঐচ্ছিক) |
| **SPI.h** | SPI Communication |

`SPI.h` Arduino/ESP32 core-এর সঙ্গে সাধারণত থাকে।

---

# ⚙️ Arduino IDE Setup

```text
Board           : ESP32S3 Dev Module
Upload Speed     : 921600 (বা 115200 problem হলে)
Flash Frequency  : 80MHz (default)
Partition Scheme : Default (বড় Image Data-এর জন্য প্রয়োজনে "Huge APP" সিলেক্ট করুন)
Port             : ESP32-S3-এর COM Port
```

### Upload করার ধাপ

```text
Connect ESP32-S3
       ↓
Select "ESP32S3 Dev Module"
       ↓
Select COM Port
       ↓
Upload
       ↓
Boot Sequence স্বয়ংক্রিয়ভাবে শুরু হবে
```

---

# 🛠️ সমস্যা হলে

### TFT Screen Blank

এই connectionগুলো পরীক্ষা করুন:

```text
CS   → GPIO 10
RST  → GPIO 8
DC   → GPIO 9
MOSI → GPIO 11
SCK  → GPIO 13
MISO → GPIO 12
LED  → 3.3V
```

### Touch কাজ করছে না

```text
T_CS  → GPIO 7
T_IRQ → GPIO 6
TCLK  → GPIO 13
T_DIN → GPIO 11
T_DO  → GPIO 12
```

### Upload হচ্ছে না / "Sketch too big" Error

```text
Tools → Partition Scheme থেকে "Huge APP (3MB No OTA/1MB SPIFFS)"
সিলেক্ট করুন — কারণ প্রতিটি Screen-এর Image Data PROGMEM-এ
বেশ বড় জায়গা নেয়।
```

### Boot Loop / Reset হয়ে যাচ্ছে

```text
RAM Overflow এড়াতে drawProgmemImage() ফাংশন সম্পূর্ণ ফ্রেম
বাফার না করে Row-by-Row স্ট্রিম করে — যদি নিজের Image পরিবর্তন
করে থাকেন, তাহলে Resolution ঠিক আছে কি না চেক করুন।
```

### একটি নির্দিষ্ট Screen দেখা যাচ্ছে না

```text
সংশ্লিষ্ট .h ফাইলের Array এবং W/H Define ঠিক আছে কি না
যাচাই করুন, এবং .ino ফাইলে সেই #include ও Function Call
আছে কি না নিশ্চিত করুন।
```

---

# 📁 Repository Structure

```text
Retro-OS-Boot-Simulator/
│
├── RetroOS_BootSimulator_ESP32S3.ino
├── Boot.h
├── Welcome.h
├── Error.h
├── Bsod.h
├── README.md
└── images/
    └── preview.png
```

---

<p align="center">

⭐ ভালো লাগলে Repository-তে একটি <b>Star</b> দিতে পারেন।

<br>

<b>Boot • Crash • Repeat</b>

</p>

---

## 📜 License

এই প্রজেক্টটি শিক্ষামূলক ও ব্যক্তিগত ব্যবহারের জন্য তৈরি।

শেখার উদ্দেশ্যে Code ব্যবহার, পরিবর্তন ও নিজের Project-এ প্রয়োগ করা যাবে।

---

# 👨‍💻 Author

<p align="center">

<b>RAHAT-UL-FARDOUSH RAZ</b><br>
Computer Engineer | Robotics • Python • C++ • Intelligent Systems

</p>

<p align="center">
  <a href="https://github.com/raz-rahat">
    GitHub
  </a>
  &nbsp;•&nbsp;
  <a href="https://www.youtube.com/@razfriday">
    Tech Raz Friday
  </a>
</p>
