#include "./include/xposix/termios.h"
#include "./include/xposix/errno.h"
#include "./include/sys/ioctl.h"
#include "./include/unistd.h"

/*
 * Terminal attribute wrappers. The kernel only stores the termios block for
 * PTYs and the default tty; line discipline and echoing stay in the kernel,
 * while all text helpers above this layer are plain libc code.
 */

extern "C" int tcgetattr(int fd, struct termios *termios_p)
{
    if (termios_p == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (ioctl(fd, TCGETS, termios_p) < 0) return -1;
    return 0;
}

extern "C" int tcsetattr(int fd, int optional_actions, const struct termios *termios_p)
{
    if (termios_p == NULL) {
        errno = EINVAL;
        return -1;
    }

    unsigned long request;
    switch (optional_actions)
    {
    case TCSANOW:   request = TCSETS;   break;
    case TCSADRAIN: request = TCSETSW;  break;
    case TCSAFLUSH: request = TCSETSF;  break;
    default:
        errno = EINVAL;
        return -1;
    }

    if (ioctl(fd, request, (void *)termios_p) < 0) return -1;
    return 0;
}

extern "C" int isatty(int fd)
{
    // A real tty is exactly a file descriptor that answers TCGETS.
    struct termios attributes;
    if (fd < 0) {
        errno = EBADF;
        return 0;
    }
    if (tcgetattr(fd, &attributes) == 0) return 1;
    if (errno != EBADF) errno = ENOTTY;
    return 0;
}
