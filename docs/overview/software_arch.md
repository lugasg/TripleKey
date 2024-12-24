# TripleKey 软件架构说明

## 开发环境
项目基于PlatformIO开发环境，使用ESP32-S3作为主控制器。

### 主要硬件配置
- MCU: ESP32-S3
- CPU频率: 160MHz
- Flash大小: 16MB
- 通信速率: 115200 baud

## 项目结构

### 1. 源代码目录 (`src/`)
- `main.cpp` - 程序入口文件
- `board_def.h/cpp` - 硬件定义和初始化
- `app/` - 应用层代码
- `manager/` - 管理器代码
- `pages/` - 界面相关代码
- `fonts/` - 字体资源文件

### 2. 主要依赖库
```ini
- AceButton (v1.9.2) - 按键处理
- PNGdec (v1.0.1) - PNG图片解码
- ESP32 BLE Keyboard (v0.3.2) - 蓝牙键盘功能
- NimBLE-Arduino - 蓝牙协议栈
- Encoder (v1.4.2) - 编码器处理
- U8g2 (v2.33.15) - 显示驱动
- ArduinoJson (v7.0.4) - JSON解析
- ESPAsyncWebServer - 异步Web服务器
- ESP8266Audio (v1.9.7) - 音频处理
- JPEGDEC (v1.4.2) - JPEG图片解码
```

### 3. 主要功能模块

#### 硬件抽象层
- 按键定义 (BUTTON1_PIN ~ BUTTON4_PIN)
- 编码器接口 (PIN_EC_A, PIN_EC_B)
- 显示屏控制 (PIN_RST, PIN_MOSI, PIN_SCLK, PIN_DC, PIN_CS1, PIN_CS2)

#### 通信模块
- WiFi管理 (WiFiManager)
- 蓝牙键盘功能 (BleKeyboard)
- Web服务器 (ESPAsyncWebServer)

#### 应用模块
- 按键应用 (app_key)
- LED控制 (app_led)
- 人体传感器 (app_humansensor)
- 系统设置 (app_settings)
- 图形绘制 (app_drawpicture)

#### 显示模块
- 多种字体支持
  - MiSans系列
  - DreamHanSerifCN系列
  - 其他特殊字体

#### 存储模块
- LittleFS文件系统
- JSON配置文件处理

### 4. 初始化流程
1. 串口初始化 (115200波特率)
2. 板级硬件初始化 (board_init)
3. 按键应用初始化 (app_key_init)
4. 管理器初始化 (manager_init)

### 5. 主循环
- manager_loop() 处理主要的任务调度

## 编译标志
```ini
- ARDUINO_USB_MODE=1
- ARDUINO_USB_CDC_ON_BOOT=1
- BOARD_HAS_PSRAM
- ELEGANTOTA_USE_ASYNC_WEBSERVER=1
- USE_NIMBLE
```

## 版本信息
- 硬件版本: TRIPLEKEY_V2
- 软件版本: 1.1.0

## 时间同步
使用阿里云NTP服务器进行时间同步：
- ntp1.aliyun.com
- ntp2.aliyun.com
- ntp3.aliyun.com
