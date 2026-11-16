/*
 *
 *      XINGJI XJ380 Application Compile Tools (XACT)
 *      XACT C\C++ Header
 *      XJ380API conio-style console input
 *      Copyright(C) XINGJI Interactive Software 2017 - 2026 All rights reserved.
 *
 *      A XINGJI Interactive Software Production
 *
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Reads one byte from the controlling terminal without waiting for Enter.
int getch(void);
// Same as getch(), but echoes the byte to stdout.
int getche(void);

#ifdef __cplusplus
}
#endif
