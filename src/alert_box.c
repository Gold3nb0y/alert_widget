#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include "common.h"

static Display *display;

uint64_t init_display(){
    uint32_t screen_num;
    Screen* def_screen;
    display = XOpenDisplay(NULL);
    if(!display) LOG_ERR("failed to open display");

    screen_num = DefaultScreen(display);
    def_screen = ScreenOfDisplay(display, screen_num);
    if(!def_screen) LOG_ERR("ScreenOfDisplay");

    LOG_VER("display name: %s\n", DisplayString(display));
    LOG_VER("screen number: %d\n", screen_num);
    return SUCCESS;
}

uint64_t destroy_display(){
    XCloseDisplay(display);
    return SUCCESS;
}
