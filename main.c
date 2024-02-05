#include "include/common.h"
#include "include/alert_box.h"
#include <unistd.h>

#define TESTS
static void tests();

int main(int argc, char* argv[]){
#ifdef TEST
    tests();
#endif
    if(argc < 2){
        puts("usage: ./alert_widget <image.png>") ;
        exit(0);
    }

    if(init_display()) LOG_ERR("DISPLAY INIT FAILED");

    draw_alert(argv[1]);

    cleanup();
    return 0;
}

static void tests(){
    LOG_DEBUG("[TEST] %s\n", "init display");
    if(init_display()) LOG_ERR("failed to initialize display");
    LOG_DEBUG("[TEST] %s\n", "Create Window");
    //if(init_display()) LOG_ERR("failed to create window");
    //sleep(1);
    LOG_DEBUG("[TEST] %s\n", "Close Window");
    close_main_window();
    sleep(1);
    LOG_DEBUG("[TEST] %s\n", "cleanup");
    cleanup();
    return;
}
