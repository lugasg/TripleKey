# TripleKey 项目结构说明

## 项目概述
TripleKey是一个完整的硬件+软件项目，包含了从软件开发、硬件设计到3D打印外壳的全套解决方案。

## 目录结构

### 1. Software 目录
主要的软件开发目录，基于PlatformIO平台开发。
- `src/` - 源代码目录，包含主要的程序代码
- `include/` - 头文件目录
- `data/` - 数据文件目录
- `test/` - 测试代码目录
- `platformio.ini` - PlatformIO配置文件
- `custom.csv`, `custom_16MB.csv`, `custom_8MB.csv` - 分区表配置文件
- `partition_custom.csv` - 自定义分区配置
- `VERSION` - 版本信息文件

### 2. Hardware 目录
硬件设计相关文件，包含PCB设计文件。
- `Gerber_【主板】TripleKey主板_2024-05-09.zip` - 主板PCB制造文件
- `Gerber_【定位板】按键定位板-屏幕排线板-拓展IO板_2024-05-09.zip` - 定位板和IO扩展板文件
- `Gerber_【拓展方案2】右侧拓展板_2024-05-09.zip` - 右侧扩展板方案
- `Gerber_【拓展方案2】米家及人在传感器拓展_2024-05-09.zip` - 米家传感器扩展方案2
- `Gerber_【拓展方案3】米家及人在传感器拓展_贴_2024-06-05.zip` - 米家传感器扩展方案3
- `Panel_透明亚克力_1mm_底面打印_带胶_2024-05-09.epanm` - 亚克力面板设计文件

### 3. Firmware 目录
包含不同型号和容量的固件文件。
- `firmware_GC9107_16MB.bin` - GC9107型号16MB固件
- `firmware_GC9107_8MB.bin` - GC9107型号8MB固件
- `firmware_NV3023_16MB.bin` - NV3023型号16MB固件
- `firmware_NV3023_8MB.bin` - NV3023型号8MB固件

### 4. docs 目录
文档和物料清单（BOM）。
- `BOM_【主板】TripleKey主板_2024-05-09.xlsx` - 主板物料清单
- `BOM_【拓展方案1】米家及人在传感器拓展_2024-05-09.xlsx` - 拓展方案1物料清单
- `BOM_【拓展方案2】右侧拓展板_2024-05-09.xlsx` - 拓展方案2物料清单
- `BOM_【拓展方案2】米家及人在传感器拓展_2024-05-09.xlsx` - 米家传感器拓展方案2物料清单
- `BOM_【拓展方案3】米家及人在传感器拓展_贴_2024-06-05.xlsx` - 米家传感器拓展方案3物料清单
- `额外BOM.xlsx` - 额外物料清单

### 5. 3DPrinting 目录
包含3D打印相关文件，可能包含外壳等3D模型设计文件。

### 6. 其他文件
- `.git/` - Git版本控制目录
- `.gitignore` - Git忽略文件配置
- `LICENSE` - 项目许可证文件
- `README.md` - 项目主要说明文档
- `README.assets/` - README文档的资源文件目录
