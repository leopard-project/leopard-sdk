#include "./include/conio.h"
#include "./include/xtuiapi.h"
#include "./include/unistd.h"

/*
 * conio compatibility over the XAPI terminal helpers. getch() shares the
 * xapi_Getch() console-fd/termios path so there is only one raw-mode
 * implementation in the runtime.
 */

extern "C" int getch(void)
{
    return (int)(unsigned char)xapi_Getch();
}

extern "C" int getche(void)
{
    unsigned char ch = (unsigned char)xapi_Getch();
    if (ch != 0) write(1, &ch, 1);
    return (int)ch;
}
