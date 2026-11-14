# XJ380 SDK
本仓库用于存储 XJ380 SDK 的源代码，与 XXCC/XACT 最新版同步，编译好的二进制文件请前往本仓库 Release 页面或 XXCC-suite 仓库或官网获取。

## 构建运行时

```bash
python3 tools/gen_ninja.py
ninja
```

产物：

`out/xapi/`（与官方 XAPI 目标文件布局一致）：

- `arch/x86_64/crt0.S.o`：`_start -> xapi_start`
- `start.cpp.o`：通用入口（`environ -> main -> exit`）
- `constart.cpp.o`：终端入口（`fork + shell.elf` terminal-app-mode）
- `*.cpp.o`：libc / POSIX / XAPI 运行时对象
- `libxact.a`：核心/TUI 运行时静态库
- `libxgui.a`：GUI 客户端 ABI（`xguiapi.cpp`），TUI 程序不会拉入

`out/runtime/`（供下游 XACT CRT 打包的运行时布局）：

```
include/
lib/x86_64/
    crt0.o
    start.o
    constart.o
    libxact.a
    libxgui.a
```

**liballoc 不属于本仓库**：`XJ380-SDK` 只提供运行时，`liballoc` 由下游
`XACT-CRT` 打包层与 `libxact.a` / `libxgui.a` 组合成完整 CRT，避免上游
SDK 反向依赖 XACT 产物。

本仓库只包含核心/TUI 运行时源码；GUI/音频/网络对象依赖发行包中的
third_party 代码，不在本次源码构建范围内。
