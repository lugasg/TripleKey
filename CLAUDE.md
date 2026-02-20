# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

TripleKey is an ESP32-S3-based Bluetooth multi-function keypad with time display, weather forecast, stock information, macro keyboard, Bluetooth keyboard, countdown timer, human presence detection, and Mijia smart home control. The project is derived from MakerM0/3Plus_v2 with significant enhancements.

**Hardware:** ESP32-S3-WROOM-1 (N16R2 variant confirmed), with three 0.85" screens, rotary encoder, and optional Mijia/human sensor expansion.

## Confirmed Device Info

| Item | Value |
|------|-------|
| Chip | ESP32-S3 (rev v0.2) |
| Flash | 16MB (N16R2) |
| Screen | 0.85" 12PIN, driver **NV3023** |
| MAC | 30:30:f9:4e:ea:40 |
| Serial port | /dev/cu.usbmodem2101 |
| Device IP | 192.168.1.185 (home WiFi) |
| Active config | SCREEN_NV3023 + custom_16MB.csv |

## Build Commands

Working directory: `Software/`

```bash
# Build firmware
pio run

# Build and upload to device (BOOT mode only)
pio run --target upload

# Serial monitor (115200 baud)
pio device monitor

# Clean build
pio run --target clean

# Upload filesystem (LittleFS)
pio run --target uploadfs
```

## Development Setup

- **IDE:** VS Code with PlatformIO extension
- **Framework:** Arduino
- **Platform:** espressif32
- **Monitor speed:** 115200 baud

## Hardware Variants

The firmware supports two screen drivers and two flash sizes. Configure in `platformio.ini`:

```ini
# Screen driver (choose one):
-D SCREEN_NV3023    # Socket connection (confirmed for this device)
# -D SCREEN_GC9107  # Solder connection

# Partition table:
board_build.partitions = custom_16MB.csv   # N16R2 variant (confirmed)
# board_build.partitions = custom_8MB.csv  # N8R2 variant
```

## Firmware Upload Workflows

### Daily OTA Upload (normal operation)

Device runs OTA server continuously — no button press needed:

```bash
cd /Users/luzhongjie/clawd/projects/TripleKey/Software
# Build first
pio run
# Upload via OTA
curl -X POST http://192.168.1.185/update -F "firmware=@.pio/build/esp32s3/firmware.bin"
```

### BOOT Mode Full Flash (first time or OTA failure)

1. Hold BOOT key (BUTTON3 / IO0), plug in USB
2. Run:

```bash
cd /Users/luzhongjie/clawd/projects/TripleKey/Software
pio run --target upload
pio run --target uploadfs
```

### Remote Restart

```bash
curl -X GET http://192.168.1.185/restart
```

### Full Erase (if needed)

```bash
/Users/luzhongjie/.platformio/penv/bin/python \
  /Users/luzhongjie/.platformio/packages/tool-esptoolpy/esptool.py \
  --port /dev/cu.usbmodem2101 --baud 921600 erase_flash
```

## Known Issues & Fixes Applied

| Issue | Fix |
|-------|-----|
| NimBLE-Arduino new API incompatible | Pinned to `^1.4.1` in platformio.ini |
| PNGdec callback signature mismatch | Changed return type from `void` to `int` |
| `dispDate()` duplicate definition | Made `static` in stocks.cpp |
| OTA trigger on BUTTON3 (IO0 = BOOT pin) conflicts | Changed OTA trigger to BUTTON1 (IO1) |
| Auto theme switching every minute | Set `autotheme = false`, fixed `clocktheme: false` in config |
| Settings page not accessible in normal mode | Added `/`, `/settings`, `/config` routes to `handleRoot()` |

## Current Behavior (as of last flash)

- Time display: fixed **NeonWhite** theme (white neon digits), no auto-switching
- Rotary encoder theme switching: disabled
- OTA server: always on in normal mode (`http://192.168.1.185/update`)
- Settings page: accessible at `http://192.168.1.185/`
- Remote restart: `GET http://192.168.1.185/restart`

## WiFi Configuration

WiFi credentials are **hardcoded** in `board_def.cpp` (pre-connect before WiFiManager fallback). Do NOT commit credentials — they are covered by `.gitignore`.

## Architecture

### Page-Based State Machine

The UI is built around a page manager pattern. Each page implements four callbacks:

```cpp
typedef struct page {
    init_cb_t init;    // Initialize variables (called once)
    enter_cb_t enter;  // Called when entering page
    exit_cb_t exit;    // Called when leaving page
    loop_cb_t loop;    // Main loop while page is active

    char *title_en, *title_cn;  // Bilingual titles
    const uint8_t *icon;        // Page icon

    bool sleep_enable;           // Allow sleep mode
    bool wakeup_btn_effect_enable;
    bool acc_enable;
} page_t;
```

### Key Directories

```
Software/src/
├── main.cpp           # Entry point (setup/loop)
├── board_def.h/cpp    # Hardware init, pin definitions
├── app/               # Application modules (independent functionality)
│   ├── app_key.*      # Input handling (buttons + encoder)
│   ├── app_settings.* # Web configuration & settings
│   ├── app_weather.*  # Weather API integration
│   ├── app_stocks.*   # Stock data
│   ├── app_mijia.*    # Mijia smart home
│   ├── app_led.*      # RGB LED control
│   ├── app_audio.*    # Audio playback
│   ├── app_pc.*       # Bluetooth keyboard
│   └── ...
├── pages/             # UI page implementations
├── manager/           # Page lifecycle management
└── fonts/             # Custom fonts (MiSans, DreamHanSerifCN)
```

### Initialization Flow

```cpp
setup() {
    Serial.begin(115200);
    board_init();      // Hardware: display, WiFi, Bluetooth, buttons
    app_key_init();    // Input system
    manager_init();    // Page manager
}

loop() {
    manager_loop();    // Delegates to current page's loop callback
}
```

### Configuration System

- Runtime configs stored in LittleFS as JSON files (`data/*.json`)
- Web-based configuration via ESPAsyncWebServer (access through settings page)
- Key configs: weather.json, stocks.json, mijia.json, encoder.json
- **Note:** `data/config_*.json` and `data/config_*.txt` are gitignored (contain user data)

## Code Style

- **Naming:** snake_case for functions/variables, UPPER_CASE for constants, _t suffix for types
- **Functions:** Verb prefix (e.g., `init_display`, `get_weather_data`)
- **Comments:** Use Doxygen format for file/function headers
- **Indentation:** 4 spaces or tabs, be consistent
- **Braces:** Opening brace on same line

## Page Implementation Pattern

When adding a new page:

1. Create `page_xxx.cpp` in `src/pages/`
2. Implement init/enter/exit/loop callbacks
3. Add icon resource if needed
4. Register in `pages.cpp` page array
5. Define bilingual titles

## Key Dependencies

| Library | Purpose |
|---------|---------|
| NimBLE-Arduino `^1.4.1` | BLE stack (pinned — newer versions break API) |
| ESP32 BLE Keyboard | Bluetooth keyboard |
| AceButton | Button handling |
| Encoder | Rotary encoder |
| ArduinoJson | JSON parsing |
| ESPAsyncWebServer | Web configuration |
| Arduino_GFX | Display driver |
| U8g2 | Font rendering |
| ElegantOTA | OTA updates |

## Boot Modes

- **Normal boot:** Standard operation (OTA always available)
- **WiFi config:** Hold middle button during boot
- **BOOT/flash mode:** Hold BOOT key (IO0) during USB plug-in

## License

CC BY-NC-SA 4.0 (Attribution-NonCommercial-ShareAlike 4.0 International)
