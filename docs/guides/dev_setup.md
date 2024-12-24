# TripleKey 开发环境设置指南

## 开发环境要求

### 软件要求
- Visual Studio Code
- PlatformIO IDE 插件
- Git

### 硬件要求
- ESP32-S3开发板
- USB数据线
- 显示屏
- 按键模块
- 其他外设（根据需要）

## 环境设置步骤

### 1. 安装开发工具
1. 安装Visual Studio Code
2. 安装PlatformIO IDE插件
3. 安装Git

### 2. 获取代码
```bash
git clone [repository-url]
cd TripleKey
```

### 3. 安装依赖
通过platformio.ini文件自动安装所有依赖库：
```ini
lib_deps = 
    bxparks/AceButton@^1.9.2
    bitbank2/PNGdec@^1.0.1
    t-vk/ESP32 BLE Keyboard@^0.3.2
    NimBLE-Arduino
    paulstoffregen/Encoder@^1.4.2
    olikraus/U8g2@^2.33.15
    bblanchon/ArduinoJson@^7.0.4
    ottowinter/ESPAsyncWebServer-esphome@^3.1.0
    earlephilhower/ESP8266Audio@^1.9.7
    bitbank2/JPEGDEC@^1.4.2
```

### 4. 硬件连接
- 按照硬件文档进行接线
- 检查所有连接

### 5. 编译和上传
1. 打开PlatformIO项目
2. 选择正确的开发板
3. 编译代码
4. 上传固件

## 常见问题

### 编译问题
- 依赖库版本冲突解决
- 编译错误处理

### 上传问题
- 串口权限设置
- 上传模式选择

### 调试技巧
- 串口监视器使用
- 调试日志设置
