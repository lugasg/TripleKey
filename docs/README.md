# TripleKey 项目文档

## 文档结构
```
docs/
├── README.md                 # 本文档：主索引
├── overview/                 # 概述文档
│   ├── project_structure.md  # 项目结构说明
│   └── software_arch.md     # 软件架构说明
├── core/                    # 核心模块文档
│   ├── app_modules.md      # 应用模块说明
│   ├── manager_module.md   # 管理器模块说明
│   └── pages_module.md     # 页面模块说明
└── guides/                 # 开发指南（待添加）
    ├── dev_setup.md       # 开发环境设置
    ├── coding_style.md    # 代码风格指南
    └── contribution.md    # 贡献指南
```

## 快速开始

1. [项目结构概述](overview/project_structure.md)
2. [软件架构说明](overview/software_arch.md)
3. [开发环境设置](guides/dev_setup.md)

## 核心模块

### [应用模块](core/app_modules.md)
- 按键控制
- 系统设置
- 天气功能
- 定时器
- 音频处理
- 其他功能模块

### [管理器模块](core/manager_module.md)
- 页面生命周期管理
- 状态管理
- 事件处理

### [页面模块](core/pages_module.md)
- 时钟页面
- 天气页面
- 股票页面
- 设置页面
- 其他功能页面

## 开发指南

### 环境设置
- PlatformIO 开发环境
- 依赖库安装
- 硬件准备

### 代码规范
- 命名规范
- 注释规范
- 代码组织

### 贡献指南
- 提交规范
- 分支管理
- 测试要求
