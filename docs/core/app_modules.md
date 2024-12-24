# TripleKey 应用模块说明

## 核心功能模块

### 1. 按键控制模块 (app_key)
负责处理设备的按键输入和编码器输入。
- 支持4个物理按键
- 每个按键支持以下事件：
  - 按下 (DOWN)
  - 释放 (UP)
  - 短按 (SHORT)
  - 长按 (LONG)
  - 双击 (DOUBLE)
- 支持编码器输入：
  - 正向旋转 (ENC_NEXT)
  - 反向旋转 (ENC_PREV)

### 2. 系统设置模块 (app_settings)
提供Web配置界面和系统设置功能。
- 配置文件管理：
  - 读取配置 (readConfig)
  - 保存配置 (saveConfig)
  - HTML页面读取 (readHTML)
- Web接口处理：
  - 主页 (handleRoot)
  - 配置提交 (handleConfigPost)
  - 米家设备配置 (handleMijiaPost)
  - 天气配置 (handleWeather)
  - 文件上传 (handleUpload)
  - 固件更新 (handleFirmware)
- 功能配置：
  - 股票配置分析 (AnalyzeStocksConfig)
  - 米家配置分析 (AnalyzeMijiaConfig)
  - Web配置分析 (AnalyzeWebConfig)
  - 时钟配置分析 (AnalyzeClockConfig)
  - 编码器配置分析 (AnalyzeEncoderConfig)
  - 照片配置分析 (AnalyzePhotoConfig)

### 3. 天气功能模块 (app_weather)
提供天气信息获取和显示功能。
- 数据结构：
  - 实时天气 (NowWeather)
    - 温度、天气状况、风向、风力等级
    - 湿度、空气质量指数、空气质量等级
  - 三天预报 (DayWeather)
    - 最高/最低温度
    - 日出/日落时间
    - 白天/夜间天气状况
  - 天气预警 (Warning)
    - 预警标题、类型、颜色
    - 开始时间
- API功能：
  - 城市信息获取 (getCity)
  - 实时天气获取 (getNowWeather)
  - 空气质量获取 (getNowAir)
  - 三天预报获取 (getDay3Weather)
  - 预警信息获取 (getWarning)

### 4. 定时器模块 (SoftwareTimer)
提供软件定时器功能，用于定时任务处理。

### 5. 音频模块 (app_audio)
处理设备的音频播放功能。

### 6. 图片绘制模块 (app_drawpicture)
处理设备的图片显示和绘制功能。

### 7. 人体传感器模块 (app_humansensor)
处理人体传感器的输入和响应。

### 8. LED控制模块 (app_led)
控制设备的LED指示灯。

### 9. 米家模块 (app_mijia)
处理与米家智能设备的集成。

### 10. PC控制模块 (app_pc)
处理与PC的通信和控制功能。

### 11. 股票模块 (app_stocks)
提供股票信息的获取和显示功能。

### 12. 超时处理模块 (app_timeout)
处理各种超时事件。

## 模块间的关系

1. **核心控制流**：
   - app_key 处理用户输入
   - app_settings 管理系统配置
   - manager 模块统筹各个功能模块

2. **显示相关**：
   - app_drawpicture 负责基础绘制
   - app_weather、app_stocks 等提供具体内容

3. **外设控制**：
   - app_led 控制指示灯
   - app_humansensor 处理传感器
   - app_audio 处理声音

4. **通信功能**：
   - app_mijia 处理智能家居
   - app_pc 处理电脑控制
   - app_settings 提供Web配置界面

## 配置管理
所有模块的配置都通过 app_settings 模块统一管理，支持：
- Web界面配置
- 文件系统存储
- 运行时更新
