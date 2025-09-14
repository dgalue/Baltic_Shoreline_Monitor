# Baltic Shoreline Monitor

## Project Goal
A low-power, solar-powered buoy uses vision and acoustic sensing to spot shoreline pollution such as oil films, abnormal noise or microplastic clusters. TinyML models running on an ESP32-S3 summarize the data and send compact events over a long‑range LoRa mesh.

## Hardware List
- **Seeed XIAO ESP32S3** with SX1262 LoRa radio
- **Solar panel + LiPo battery** for autonomous power
- **Grove Vision AI V2** camera module (I²C)
- **Hydrophone + preamp** for underwater audio
- **Air530 GPS** (UART)
- **SSD1315 OLED** display (I²C)
- **MicroSD card** for local logging

## Hardware Wiring Diagram
![Wiring diagram showing sensor and radio connections](docs/assets/architecture.jpg)
*Sensors and peripherals wired to the Seeed XIAO ESP32-S3.*

## Meshtastic + FreeRTOS Architecture
Meshtastic handles the SX1262 radio, routing and BLE/Wi‑Fi services on **core 0** of the ESP32‑S3. Custom FreeRTOS tasks run on **core 1** for audio capture, AI inference, vision polling, GPS and logging. Shared buses (I²C/SPI) use mutexes and only one task writes to the SD card. This keeps radio timing deterministic while application logic scales with additional sensors.

## Build and Flash Steps
This repository vendors Meshtastic firmware release **meshtastic-firmware-2.6.11.60ec05e**. To compile it for the Seeed XIAO ESP32S3:

1. Install PlatformIO: `pip install platformio`
2. `cd meshtastic-firmware-2.6.11.60ec05e`
3. Build the firmware: `pio run -e seeed-xiao-s3`
4. Connect the board over USB and flash: `pio run -e seeed-xiao-s3 -t upload`

## Detailed Guides
- [ESP32-S3 Comprehensive Guide](./ESP32-S3_Comprehensive_Guide.md)
- [Meshtastic firmware README](./meshtastic-firmware-2.6.11.60ec05e/README.md)
- [Meshtastic documentation](https://meshtastic.org/docs/)
- [FreeRTOS API reference](https://freertos.org)

## Code Overview
- [GPS Task](src/tasks/Task_GPS.cpp) – reads GPS data and queues events
- [Vision Task](src/tasks/Task_Vision.cpp) – performs camera inference

docs/assets/architecture.jpg
