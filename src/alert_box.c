#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <png.h>
#include <pngconf.h>
#include <stdint.h>
#include "common.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ENTER_KEYCODE 0x24
#define HIEGHT 1000
#define WIDTH 1200
#define ENTER 0xFF0D
#define HDR_SZ 8

static struct struct_alert_box {
    Display *display;
    Window main_window;
    GC graphic_ctx;
} alert_box;

XImage* waifu;

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
    alert_box.main_window = XCreateSimpleWindow(alert_box.display, DefaultRootWindow(alert_box.display), 300, 300, WIDTH, HIEGHT, 2, 0, 0xFFFFFF);

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

//refrence https://copyprogramming.com/howto/displaying-png-file-using-xputimage-does-not-work
//https://heiwais25.github.io/c/image%20process/2017/12/28/how-to-use-libpng-library/
void load_png(FILE *imgp){
    size_t size = 0;
    png_structp png = NULL;
    int read_flag = PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND;
    png_infop info = NULL, end = NULL;
    int depth = 0, colortype = 0, interlace = 0;
    png_bytepp row_pointers;
    uint8_t header[HDR_SZ+1];
    uint8_t *data;
    uint32_t width, height;
    uint32_t row_bytes;

    memset(header, 0, HDR_SZ+1);
    fread(header, 1, HDR_SZ, imgp);
    if(png_sig_cmp(header, 0, HDR_SZ)) LOG_ERR("invalid png file");

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) LOG_ERR("png read struct");

    info = png_create_info_struct(png);
    if(!info){
        png_destroy_read_struct(&png, NULL, NULL);
        LOG_ERR("info struct");
    }

    end = png_create_info_struct(png);
    if(!end){
        png_destroy_read_struct(&png, NULL, NULL);
        LOG_ERR("end info struct");
    }

    png_init_io(png, imgp);
    png_set_sig_bytes(png, HDR_SZ);
    png_read_png(png, info, read_flag, NULL);

    LOG_DEBUG("%s\n", "parsing IHDR");
    png_get_IHDR(png, info, &width, &height, 
            &depth, &colortype, &interlace, NULL, NULL);
    LOG_DEBUG("width: %d height %d, depth: %d, colortype: %d, interlace: %d\n",
            width, height, depth, colortype, interlace);

    LOG_DEBUG("%s\n", "get rowbytes");
    row_bytes = png_get_rowbytes(png, info);
    //https://copyprogramming.com/howto/displaying-png-file-using-xputimage-does-not-work
    size = height * row_bytes;

    LOG_DEBUG("%s\n", "create data and pointers");
    data = malloc(sizeof(png_byte) * size);
    row_pointers = png_get_rows(png, info);
    LOG_DEBUG("row_pointers %p\n", row_pointers);

    LOG_DEBUG("%s\n", "read image");
    for(uint i = 0; i < height; i++){
        //LOG_DEBUG("data: %p index: %d row_bytes: %d row_pointer: %p\n", *data, i, *row_bytes, row_pointers[i]);
        memcpy(data + (i * (row_bytes)), row_pointers[i], row_bytes);
    }

    LOG_DEBUG("Display Depth: %d\n", DefaultDepth(alert_box.display, DefaultScreen(alert_box.display)));
    waifu = XCreateImage(alert_box.display, 
            DefaultVisual(alert_box.display, DefaultScreen(alert_box.display)),
            DefaultDepth(alert_box.display, DefaultScreen(alert_box.display)),
            ZPixmap, 0, (char*)data, 
            width, height, 32, row_bytes);

    LOG_DEBUG("%s\n", "destroy");
    //png_read_end(png, end);
    png_destroy_info_struct(png, &info);
    png_destroy_read_struct(&png, &info, NULL);
}

uint64_t draw_image(){
    uint64_t ret = SUCCESS;
    FILE* imgp = fopen("aqua.png", "rb");
    if(!imgp) LOG_ERR("failed to open file");

    load_png(imgp);

    if(waifu){
        LOG_DEBUG("%s\n", "drawing waifu");
        XPutImage(alert_box.display, alert_box.main_window, alert_box.graphic_ctx, 
                waifu, 0, 0, 0, 0, 860, 1160);
    } else {
        LOG_DEBUG("%s\n", "Failed to create waifu :(");
        ret = FAILURE;
    }

    //send the events
    XFlush(alert_box.display);
    XSync(alert_box.display, False);

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
                if(ksym == ENTER)
                    return;
            case Expose:
                XSetForeground(alert_box.display, alert_box.graphic_ctx, 
                        randr(255) << 16 | randr(255) << 8 | randr(255));
                XDrawLine(alert_box.display, alert_box.main_window, 
                        alert_box.graphic_ctx, randr(600), randr(600), randr(600), randr(600));
                XFlush(alert_box.display);
                break;
            default:
                break;
        }
    }
    return;
}

void draw_alert(){
    LOG_DEBUG("%s\n", "aqua");
    draw_image();
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
