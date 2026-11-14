#!/usr/bin/env python3
"""XJ380-SDK 构建：编译官方 XAPI 运行时，供各 Leopard 组件链接。

产物（均在本仓库内，可被 ../<component>/gen_ninja.py 直接引用）：
    obj-tui/<name>.cpp.o      官方运行时对象
    obj-tui/liballoc-x86_64.a 官方预编译分配器（见 lib/README.md）
    obj-extra/start.cpp.o     官方 start.cpp：environ -> main -> exit

说明：
- 头文件直接用 src/include，不再复制一份，避免与 SDK 源码漂移。
- 不使用官方 constart.cpp：它会 fork 后 exec /apps/system/shell/shell.elf
  建立 terminal-app-mode，Leopard 没有该宿主；start.cpp 是官方提供的
  无包装入口，功能等价于原 Leopard CRT。
"""
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parent
SRC = ROOT / "src"
INC = SRC / "include"
OBJ = ROOT / "obj-tui"
EXTRA = ROOT / "obj-extra"
LIBALLOC_SRC = ROOT / "lib" / "liballoc-x86_64.a"
LIBALLOC_DST = OBJ / "liballoc-x86_64.a"
CRT_SRC = SRC / "start.cpp"
CRT_OBJ = EXTRA / "start.cpp.o"

# 各组件当前实际链接的运行时集合。需要新 API 时在这里追加源文件即可。
RUNTIME = [
    "libsys.cpp", "xtuiapi.cpp", "krlibc.cpp", "string.cpp", "stdlib.cpp",
    "stdio.cpp", "unistd.cpp", "errno.cpp", "posix_ret.cpp", "ctype.cpp",
    "permissions.cpp", "posix_fs.cpp", "posix_time.cpp", "posix_ioctl.cpp",
    "xcrypto_random.cpp",
]
CRT0 = "arch/x86_64/crt0.S"


def rel(p):
    return str(Path(p).resolve().relative_to(ROOT))


def esc(p):
    return rel(p).replace("$", "$$").replace(":", "$:").replace(" ", "$ ")


def main():
    cc = os.environ.get("CC", "clang")
    cxx = os.environ.get("CXX", "clang++")
    ld = os.environ.get("LD", "ld")
    opt = "-O" + os.environ.get("OPT_LEVEL", "2")
    col = os.environ.get("DIAGNOSTICS_COLOR", "-fdiagnostics-color=always")

    common = (f"{col} {opt} -Wall -g -ffreestanding -fno-builtin -m64 -mstackrealign "
              "-fno-stack-protector -fno-exceptions -fno-strict-aliasing -fshort-wchar -nostdinc")
    cxx_flags = f"{common} -std=c++11 -I {rel(INC)} -Wno-write-strings -MMD -MP"
    asm_flags = f"{col} -g -m64 -nostdinc"

    L = ["# XJ380-SDK 官方运行时构建", "ninja_required_version = 1.10",
         f"cxx = {cxx}", f"cc = {cc}", f"ld = {ld}",
         f"cxx_flags = {cxx_flags}", f"asm_flags = {asm_flags}", ""]
    L += [
        "rule cxx",
        "  command = mkdir -p $$(dirname $out) && $cxx $cxx_flags -MF $out.d -c $in -o $out",
        "  depfile = $out.d", "  deps = gcc", "",
        "rule asm",
        "  command = mkdir -p $$(dirname $out) && $cc $asm_flags -c $in -o $out", "",
        "rule copy",
        "  command = mkdir -p $$(dirname $out) && cp $in $out", "",
    ]

    objects = []
    crt0 = OBJ / "arch" / "x86_64" / "crt0.S.o"
    L += [f"build {esc(crt0)}: asm {esc(SRC / CRT0)}", ""]
    objects.append(crt0)

    for name in RUNTIME:
        obj = OBJ / (name + ".o")
        L += [f"build {esc(obj)}: cxx {esc(SRC / name)}", ""]
        objects.append(obj)

    # liballoc 是官方发布的 Rust no_std 预编译归档，源码不在本仓库。
    if not LIBALLOC_SRC.is_file():
        raise SystemExit(f"缺少 {LIBALLOC_SRC}；请从 XACT 发行包或 OpenXJ380 复制 liballoc-x86_64.a")
    L += [f"build {esc(LIBALLOC_DST)}: copy {esc(LIBALLOC_SRC)}", ""]
    objects.append(LIBALLOC_DST)

    L += [f"build {esc(CRT_OBJ)}: cxx {esc(CRT_SRC)}", ""]
    objects.append(CRT_OBJ)

    L += ["build all: phony " + " ".join(esc(o) for o in objects), "default all", ""]
    L += ["rule clean", "  command = rm -rf obj-tui obj-extra", "build clean: clean"]

    (ROOT / "build.ninja").write_text("\n".join(L) + "\n", encoding="utf-8")
    print(f"已生成 build.ninja（XJ380-SDK，{len(objects)} 个产物）。构建：ninja")


if __name__ == "__main__":
    main()
