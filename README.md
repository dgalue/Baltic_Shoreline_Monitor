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

## Building the Host Integration Test
The integration test in `tests/integration_test.cpp` can be compiled and run on a
desktop machine. The original instructions used `/tmp/integration_test` as the
output binary, which works on Unix-like systems but fails on Windows because the
`/tmp` directory is not defined. Use a repository-local build directory instead:

1. Create an output directory (run once):
   - macOS/Linux: `mkdir -p build`
   - Windows (PowerShell or Command Prompt): `mkdir build`
2. Compile the test:
   - macOS/Linux: `g++ -std=c++17 tests/integration_test.cpp -Isrc -o build/integration_test`
   - Windows (MinGW): `g++ -std=c++17 tests/integration_test.cpp -Isrc -o build\integration_test.exe`
3. Run the resulting executable from the `build` directory.

If you prefer another location, make sure the directory exists and update the
`-o` argument accordingly.

## Detailed Guides
- [ESP32-S3 Comprehensive Guide](./ESP32-S3_Comprehensive_Guide.md)
- [Meshtastic firmware README](./meshtastic-firmware-2.6.11.60ec05e/README.md)
- [Meshtastic documentation](https://meshtastic.org/docs/)
- [FreeRTOS API reference](https://freertos.org)

## Code Overview
- [GPS Task](src/tasks/Task_GPS.cpp) – reads GPS data and queues events
- [Vision Task](src/tasks/Task_Vision.cpp) – performs camera inference

docs/assets/architecture.jpg
