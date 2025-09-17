# Hardware Component Validation Guide

This guide explains how to bring up each peripheral connected to the Seeed XIAO
ESP32S3 individually. Every test runs on actual hardware and relies on
manufacturer example sketches or widely available Arduino/PlatformIO libraries.
Run the tests one at a time before integrating the components into the full
Baltic Shoreline Monitor firmware.

> **Note**
> The commands below require physical access to the development board and
> sensors. They cannot be exercised in CI; capture the observed output in the
> "Result" field for each test once you complete it on hardware.

## Common Setup

1. Install PlatformIO (`pip install platformio`) if you have not already.
2. From the repository root run `pio project init --board seeed_xiao_esp32s3`
   inside a temporary folder such as `hardware-bringup/`.
3. Install the required libraries listed in each section using
   `pio lib install <name>`.
4. Copy the corresponding `main.cpp` snippet into the PlatformIO project's
   `src/` directory, build with `pio run`, then flash with
   `pio run -t upload`.
5. Open a serial monitor at 115200 bps (`pio device monitor`) to watch the
   output.
6. Record the observed behavior in the result table.

Power each peripheral from the 3V3 and GND pins on the XIAO ESP32S3 and connect
signal pins as described below. Disconnect other peripherals while testing to
avoid bus contention.

## Air530 GPS (UART)

- **Library**: none required (use the built-in `HardwareSerial` interface)
- **Wiring**: connect GPS `TX` to XIAO pin `GPIO44 (RX)`, GPS `RX` to
  `GPIO43 (TX)`, and `5V`/`GND` to the matching pins.

```cpp
#include <Arduino.h>

HardwareSerial GPS(1);

constexpr int GPS_RX = 44;  // XIAO pin receiving GPS TX
constexpr int GPS_TX = 43;  // XIAO pin driving GPS RX

void setup() {
  Serial.begin(115200);
  GPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
}

void loop() {
  while (GPS.available()) {
    Serial.write(GPS.read());
  }

  while (Serial.available()) {
    GPS.write(Serial.read());
  }
}
```

**Expected output**: NMEA sentences (e.g., `$GPGGA`, `$GPRMC`) streaming over
USB once the module powers up. You can copy the output into a GPS viewer such as
https://www.gpsvisualizer.com/ to decode location and fix data. Because this sketch
simply bridges UART-to-USB, the text should match the raw sentences described in
Seeed's Air530 documentation (<https://wiki.seeedstudio.com/Grove-GPS-Air530/>).

**Result**:

| Date | Firmware Commit | Observed Behavior |
| --- | --- | --- |
| _pending_ | | |

## Grove Vision AI V2 (I²C)

- **Library**: `Seeed_Arduino_SSCMA`
- **Wiring**: connect SDA to `GPIO9`, SCL to `GPIO8`, plus 3V3 and GND.

```cpp
#include <Arduino.h>
#include <Seeed_Arduino_SSCMA.h>

SSCMA ai;

void setup() {
  Serial.begin(115200);
  if (!ai.begin()) {
    Serial.println("Camera init failed");
    while (true) {
      delay(1000);
    }
  }
  ai.setDefaultModel();
}

void loop() {
  if (ai.invoke()) {
    SSCMA::object_detection_t result;
    if (ai.getResult(result)) {
      Serial.print("Detections: ");
      Serial.println(result.object_count);
      for (uint8_t i = 0; i < result.object_count; ++i) {
        Serial.print("Object ");
        Serial.print(i);
        Serial.print(" score ");
        Serial.println(result.objects[i].score);
      }
    }
  }
  delay(1000);
}
```

**Expected output**: After initialization the camera reports the number of
objects detected each second along with confidence scores. If no objects are in
view, the count should be zero.

**Result**:

| Date | Firmware Commit | Observed Behavior |
| --- | --- | --- |
| _pending_ | | |

## MicroSD Card (SPI)

- **Library**: built-in `SD` from ESP-IDF/Arduino core
- **Wiring**: connect `CS` to `GPIO1`, `MOSI` to `GPIO10`, `MISO` to `GPIO11`,
  `SCK` to `GPIO12`, and supply 3V3/GND.

```cpp
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

constexpr uint8_t SD_CS = 1;

void setup() {
  Serial.begin(115200);
  if (!SD.begin(SD_CS)) {
    Serial.println("SD init failed");
    while (true) {
      delay(1000);
    }
  }
  File f = SD.open("test.txt", FILE_WRITE);
  if (f) {
    f.println("SD card write test");
    f.close();
    Serial.println("Write OK");
  } else {
    Serial.println("Write failed");
  }

  f = SD.open("test.txt", FILE_READ);
  if (f) {
    Serial.print("Read back: ");
    while (f.available()) {
      Serial.write(f.read());
    }
    f.close();
  } else {
    Serial.println("Read failed");
  }
}

void loop() {
  delay(1000);
}
```

**Expected output**: `Write OK` followed by the text `SD card write test`
indicates the card can be mounted, written, and read back successfully.

**Result**:

| Date | Firmware Commit | Observed Behavior |
| --- | --- | --- |
| _pending_ | | |

## SSD1315 OLED (I²C)

- **Library**: `Adafruit_SSD1306`
- **Wiring**: SDA to `GPIO9`, SCL to `GPIO8`, plus 3V3 and GND.

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (true) {
      delay(1000);
    }
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Baltic");
  display.println("Monitor");
  display.display();
}

void loop() {
  delay(1000);
}
```

**Expected output**: The OLED displays "Baltic" on the first line and
"Monitor" on the second line with the Serial console remaining silent after
initialization.

**Result**:

| Date | Firmware Commit | Observed Behavior |
| --- | --- | --- |
| _pending_ | | |

## X1262 LoRa Radio

- **Library**: `RadioLib`
- **Wiring**: The Seeed XIAO ESP32S3 LoRa variant has the SX1262 connected
  internally—no additional wiring is required.

```cpp
#include <Arduino.h>
#include <RadioLib.h>

SX1262 radio = new Module(5, 7, 6, 3);

void setup() {
  Serial.begin(115200);
  int state = radio.begin(915.0);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("Radio init failed: ");
    Serial.println(state);
    while (true) {
      delay(1000);
    }
  }
  Serial.println("Radio ready, sending test packet");
  state = radio.transmit("hello baltic shoreline");
  Serial.print("Transmit status: ");
  Serial.println(state);
}

void loop() {
  delay(5000);
  int state = radio.transmit("ping");
  Serial.print("Periodic transmit status: ");
  Serial.println(state);
}
```

**Expected output**: Successful initialization prints `Radio ready, sending test
packet`. Transmit status values of `0` indicate the packet was queued
successfully. To fully verify RF performance, monitor the transmissions with a
second LoRa receiver tuned to the same frequency.

**Result**:

| Date | Firmware Commit | Observed Behavior |
| --- | --- | --- |
| _pending_ | | |

## Test Log

After completing each bring-up run, update the tables above and summarize the
outcome here for quick reference.

| Component | Pass/Fail | Notes |
| --- | --- | --- |
| Air530 GPS | _pending_ | |
| Grove Vision AI V2 | _pending_ | |
| MicroSD | _pending_ | |
| SSD1315 OLED | _pending_ | |
| X1262 LoRa | _pending_ | |

