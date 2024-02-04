#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>
#include "common.h"
#include <stdlib.h>
#include <unistd.h>

#define ENTER_KEYCODE 0x24
#define HIEGHT 600
#define WIDTH 600

static struct struct_alert_box {
    Display *display;
    Window main_window;
    GC graphic_ctx;
} alert_box;

int randr(uint upper){
    return rand() % upper+1;
}

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
    int status;
    uint64_t ret = SUCCESS;
    alert_box.main_window = XCreateSimpleWindow(alert_box.display, DefaultRootWindow(alert_box.display), 300, 300, HIEGHT, WIDTH, 2, 0, 0xFFFFFF);

    status = XSelectInput(alert_box.display, alert_box.main_window, StructureNotifyMask | ExposureMask | KeyPressMask);
    if(status == BadWindow){
        ret = FAILURE;
        goto out;
    }


    alert_box.graphic_ctx = XCreateGC(alert_box.display, alert_box.main_window, 0, NULL);

    XMapWindow(alert_box.display, alert_box.main_window);
out:
    return ret;
}

void handle_events(){
    XEvent e;
    XKeyEvent k;
    KeySym ksym;
    for(;;){
        XNextEvent(alert_box.display, &e);
        switch(e.type){
            case KeyPress:
                k = e.xkey;
                ksym = XLookupKeysym(&k, 0);
                LOG_DEBUG("KEY PRESSED: %x\n", k.keycode);
                LOG_DEBUG("KEYSYM: %x\n", (uint32_t)ksym);
                if(ksym == '\a')
                    return;
            case Expose:
                XSetForeground(alert_box.display, alert_box.graphic_ctx, randr(255) << 16 | randr(255) << 8 | randr(255));
                XDrawLine(alert_box.display, alert_box.main_window, alert_box.graphic_ctx, randr(600), randr(600), randr(600), randr(600));
                XFlush(alert_box.display);
                break;
            default:
                break;
        }
    }
    return;
}

void draw_alert(){
    LOG_DEBUG("%s\n", "drawing started");
    handle_events();
    LOG_DEBUG("%s\n", "drawing done");
}

uint64_t close_window(Window w){
    int status;
    uint64_t ret;
    status = XUnmapWindow(alert_box.display, w);
    XFreeGC(alert_box.display, alert_box.graphic_ctx);
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
    XCloseDisplay(alert_box.display);
    //XFlush(alert_box.display);
    //XFree(alert_box.display);
    return SUCCESS;
}
