#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include "common.h"
#include <unistd.h>

static struct struct_alert_box {
    Display *display;
    Window main_window;
} alert_box;

uint64_t init_display(){
    uint32_t screen_num;
    Screen* def_screen;
    alert_box.display = XOpenDisplay(NULL);
    if(!alert_box.display) LOG_ERR("failed to open alert_box.display");

    screen_num = DefaultScreen(alert_box.display);
    def_screen = ScreenOfDisplay(alert_box.display, screen_num);
    if(!def_screen) LOG_ERR("ScreenOfDisplay");

    LOG_VER("alert_box.display name: %s\n", DisplayString(alert_box.display));
    LOG_VER("screen number: %d\n", screen_num);
    return SUCCESS;
}

uint64_t create_main_window(){
    alert_box.main_window = XCreateSimpleWindow(alert_box.display, DefaultRootWindow(alert_box.display), 50, 50, 250, 250, 2, 0, 0xFFFFFF);
    XMapWindow(alert_box.display, alert_box.main_window);
    XFlush(alert_box.display);
    return SUCCESS;
}

uint64_t close_window(Window w){
    int status;
    uint64_t ret;
    status = XUnmapWindow(alert_box.display, w);
    if(status == BadWindow){
        ret = FAILURE;
    } else {
        ret = SUCCESS;
        XFlush(alert_box.display);
    }
    return ret;
}

void close_main_window(){
    if(close_window(alert_box.main_window)) LOG_ERR("BadWindow it close main window");
}

uint64_t cleanup(){
    close_main_window();
    XCloseDisplay(alert_box.display);
    //XFlush(alert_box.display);
    //XFree(alert_box.display);
    return SUCCESS;
}
