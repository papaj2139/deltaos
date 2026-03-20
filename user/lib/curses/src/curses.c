#include <io.h>
#include <keyboard.h>

int curses_init() {
    int err = kbd_init();
    if (err < 0) return err;
}