/*
 *
 *      XINGJI XJ380 Application Compile Tools (XACT)
 *      XACT C\C++ Header
 *      XJ380API POSIX Edition - terminal attributes
 *      Copyright(C) XINGJI Interactive Software 2017 - 2026 All rights reserved.
 *
 *      A XINGJI Interactive Software Production
 *
 */

#pragma once

#include "../stdint.h"

/*
 * This header is the userland mirror of OpenXSKernel include/tty.h. The ioctl
 * requests and the termios layout are part of the user/kernel ABI, so the
 * field order, c_line type and NCCS count must stay in sync with the kernel.
 */

#define NCCS 20

#define VINTR    0
#define VQUIT    1
#define VERASE   2
#define VKILL    3
#define VEOF     4
#define VTIME    5
#define VMIN     6
#define VSWTC    7
#define VSTART   8
#define VSTOP    9
#define VSUSP    10
#define VEOL     11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE  14
#define VLNEXT   15
#define VEOL2    16

#define IGNBRK  0000001
#define BRKINT  0000002
#define IGNPAR  0000004
#define PARMRK  0000010
#define INPCK   0000020
#define ISTRIP  0000040
#define INLCR   0000100
#define IGNCR   0000200
#define ICRNL   0000400
#define IUCLC   0001000
#define IXON    0002000
#define IXANY   0004000
#define IXOFF   0010000
#define IMAXBEL 0020000
#define IUTF8   0040000

#define OPOST 0000001

#define CSIZE  0000060
#define CS5    0000000
#define CS6    0000020
#define CS7    0000040
#define CS8    0000060
#define CSTOPB 0000100
#define CREAD  0000200
#define PARENB 0000400
#define PARODD 0001000
#define HUPCL  0002000
#define CLOCAL 0004000

#define ISIG   0000001
#define ICANON 0000002
#define ECHO   0000010
#define ECHOE  0000020
#define ECHOK  0000040
#define ECHONL 0000100
#define NOFLSH 0000200
#define TOSTOP 0000400
#define ECHOCTL 0001000
#define IEXTEN 0100000

/* tcsetattr optional_actions */
#define TCSANOW   0
#define TCSADRAIN 1
#define TCSAFLUSH 2

/* ioctl requests implemented by the kernel tty/pty layer */
#define TCGETS  0x5401
#define TCSETS  0x5402
#define TCSETSW 0x5403
#define TCSETSF 0x5404

struct termios
{
    uint32_t c_iflag;
    uint32_t c_oflag;
    uint32_t c_cflag;
    uint32_t c_lflag;
    uint8_t  c_line;
    uint8_t  c_cc[NCCS];
};

#ifdef __cplusplus
extern "C" {
#endif

int tcgetattr(int fd, struct termios *termios_p);
int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);

#ifdef __cplusplus
}
#endif
