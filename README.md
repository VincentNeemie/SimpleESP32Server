# Simple ESP32 Web Server for ESP32-WROOM-32

![ESP32](https://m.media-amazon.com/images/I/51FzgrVl-cL._UF350,350_QL80_.jpg)

Small, friendly ESP32 web server that serves a responsive web UI from SPIFFS, lets you control the onboard LED, view system info, and scan nearby Wi‑Fi networks.

<!-- Badges -->
[![PlatformIO](https://img.shields.io/badge/PlatformIO-esp32-blue.svg)](https://platformio.org)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

## Key Features

- Wi‑Fi connectivity and web server hosted on the ESP32
- Responsive web UI served from SPIFFS (`/data/index.html`)
- LED control (on/off/blink)
- System information endpoint and uptime counter
- Wi‑Fi network scanner

## Hardware

- ESP32‑WROOM‑32 module (pins pre‑soldered recommended)
- Micro‑USB cable
- Computer with USB port (Windows/macOS/Linux)

## Software

- Visual Studio Code
- PlatformIO extension
- ESP32 Arduino framework (PlatformIO will install it)

## Quick start (safe for sharing)

1. Clone this repository:

```bash
git clone <your-repo-url>
cd SimpleESP32Server
```

2. Open the project in VS Code and install the PlatformIO extension if needed.

3. IMPORTANT: set your Wi‑Fi credentials in `src/main.cpp` before uploading:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

4. Build & upload the firmware (PlatformIO)

5. Build & upload the filesystem image (Project tasks → Platform → Build Filesystem Image → Upload Filesystem Image)

6. Open the Serial Monitor (115200 baud) to see the ESP32 IP address and server logs.

7. Open the IP address shown in the Serial Monitor in your browser.

## One‑click upload (Windows)

There is a helper `upload.bat` (Windows) to run build/upload/uploadfs and open the monitor. See the repo root.

## Security & sharing

- Do NOT commit real Wi‑Fi credentials or secrets. `src/main.cpp` uses placeholders.
- If you add API keys or other secrets in the future, keep them out of the repo (use environment variables or a separate `secrets.h` excluded by `.gitignore`).

## Project layout

- `platformio.ini` — PlatformIO config
- `src/main.cpp` — Main firmware
- `data/` — Files served from SPIFFS (including `index.html`)
- `SETUP_GUIDE.md` — Extended setup instructions

## Troubleshooting

- Serial Monitor stuck? Close and reopen it.
- If you can't upload, check the COM port and drivers (CH340/CP210x).
- For SPIFFS issues, rebuild filesystem image and upload.

## Contributing

Fixes and improvements welcome — open a PR. If adding features that require secrets, add instructions to load them from local config ignored by `.gitignore`.

## License

This project is licensed under the MIT License — see `LICENSE`.
