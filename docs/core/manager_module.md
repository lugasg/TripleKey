# TripleKey 管理器模块说明

## 概述
管理器模块(`manager`)是TripleKey项目的核心控制模块，负责管理和调度所有页面的显示和交互。它采用了状态机的设计模式，通过回调函数机制实现页面的生命周期管理。

## 核心数据结构

### 页面结构 (page_t)
```c
typedef struct page {
    init_cb_t init;    // 初始化回调
    enter_cb_t enter;  // 进入页面回调
    exit_cb_t exit;    // 退出页面回调
    loop_cb_t loop;    // 循环执行回调

    char *title_en;    // 英文标题
    char *title_cn;    // 中文标题

    const uint8_t *icon;       // 图标数据
    uint16_t icon_width;       // 图标宽度
    uint16_t icon_height;      // 图标高度

    bool sleep_enable;             // 睡眠使能标志
    bool wakeup_btn_effect_enable; // 唤醒按钮效果使能
    bool acc_enable;               // 加速度计使能
}
```

### 管理器结构 (manager_t)
```c
typedef struct manager {
    uint8_t index;     // 当前页面索引
    uint8_t count;     // 总页面数量
    page_t *cur;       // 当前页面指针
    page_t *next;      // 下一个页面指针
    bool busy;         // 忙状态标志
}
```

## 主要功能

### 1. 初始化 (manager_init)
- 初始化管理器状态
- 设置初始页面（索引1）
- 调用当前页面的初始化和进入回调

### 2. 主循环 (manager_loop)
- 非忙状态时执行当前页面的循环回调
- 包含10ms的延时以防止过快刷新
- 忙状态时只执行延时

### 3. 页面切换
提供多种页面切换方式：
- `manager_switchToNext`: 切换到下一个页面
- `manager_switchToChild`: 切换到子页面
- `manager_switchToParent`: 切换到父页面
- `manager_switchToPage`: 切换到指定页面

### 4. 状态管理
- `manager_setBusy`: 设置忙状态
- `manager_getBusy`: 获取当前忙状态

## 页面生命周期

1. **初始化阶段**
   - 调用 `init` 回调
   - 初始化页面相关变量和资源

2. **进入阶段**
   - 调用 `enter` 回调
   - 执行页面进入时的准备工作

3. **运行阶段**
   - 循环调用 `loop` 回调
   - 处理页面的主要逻辑

4. **退出阶段**
   - 调用 `exit` 回调
   - 清理页面资源

## 页面特性

每个页面可以配置以下特性：
1. **标题显示**
   - 支持中英文双语标题
   - 可配置页面图标

2. **电源管理**
   - 可配置是否允许睡眠
   - 可配置唤醒按钮行为

3. **传感器集成**
   - 可配置是否使用加速度计

## 使用示例

```cpp
// 定义页面回调函数
void page_init(void *data) {
    // 初始化代码
}

void page_enter(void *data) {
    // 进入页面代码
}

void page_exit(void *data) {
    // 退出页面代码
}

void page_loop(void *data) {
    // 循环执行代码
}

// 创建页面结构
page_t my_page = {
    .init = page_init,
    .enter = page_enter,
    .exit = page_exit,
    .loop = page_loop,
    .title_en = "My Page",
    .title_cn = "我的页面",
    .sleep_enable = true,
    .wakeup_btn_effect_enable = true,
    .acc_enable = false
};
```

## 注意事项

1. 页面切换时的资源管理
   - 确保在exit回调中释放所有资源
   - 避免在loop回调中执行耗时操作

2. 状态管理
   - 使用busy状态来防止页面切换时的重入
   - 注意管理页面切换的时序

3. 内存管理
   - 页面资源应在适当的生命周期阶段分配和释放
   - 避免内存泄漏
