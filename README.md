# XJ380 SDK
本仓库用于存储 XJ380 SDK 的源代码，与 XXCC/XACT 最新版同步，编译好的二进制文件请前往本仓库 Release 页面或 XXCC-suite 仓库或官网获取。

## 构建

```bash
python3 gen_ninja.py
ninja
```

产物：

- `obj-tui/`：官方 XAPI 运行时对象 + `liballoc-x86_64.a`
- `obj-extra/start.cpp.o`：官方 `start.cpp` 入口（`environ → main → exit`）

Leopard 组件直接引用 `src/include` 与上述产物；不使用会 fork/exec
`/apps/system/shell/shell.elf` 的官方 `constart.cpp`。
