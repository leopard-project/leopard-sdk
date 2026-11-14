# XJ380 SDK
本仓库用于存储 XJ380 SDK 的源代码，与 XXCC/XACT 最新版同步，编译好的二进制文件请前往本仓库 Release 页面或 XXCC-suite 仓库或官网获取。

## 构建运行时 / CRT / xact-crt 包

```bash
git submodule update --init third_party/liballoc
python3 tools/gen_ninja.py
ninja            # out/xapi + out/runtime + out/crt
ninja pack       # out/xact-crt-2026v5_1.x86_64.xbps
```

产物：

`out/xapi/`：运行时对象 + `libxact.a` + `libxgui.a` + `liballoc.a`

`out/runtime/`：`include/` + 启动对象 + `libxact.a` + `libxgui.a`

`out/crt/`：完整 XACT CRT

```
include/
lib/x86_64/
    crt0.o
    start.o
    constart.o
    libxact.a
    libxgui.a
    liballoc.a
```

`liballoc` 来自 `third_party/liballoc` submodule（`plos-clan/liballoc`，MIT）。
有 `rustup` + Rust nightly + rust-src 时从源码构建；否则使用
`vendor/liballoc-x86_64.a` 或 OpenXJ380 中的同一归档，并做 SHA-256 校验。
`ninja pack` 生成 `xact-crt` xbps；需要 `xbps-create` 在 PATH 中。

本仓库只包含核心/TUI 运行时源码；GUI/音频/网络对象依赖发行包中的
third_party 代码，不在本次源码构建范围内。
