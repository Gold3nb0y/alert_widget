#include "include/common.h"
#include "include/alert_box.h"
#include <unistd.h>

#define TESTS
static void tests();

int main(void){
#ifdef TEST
    tests();
#endif

    if(init_display()) LOG_ERR("DISPLAY INIT FAILED");

    if(create_main_window()){
        LOG_INFO("Failed to init window");
        goto out;
    }
    draw_alert();

out:
    cleanup();
    return 0;
}

static void tests(){
    LOG_DEBUG("[TEST] %s\n", "init display");
    if(init_display()) LOG_ERR("failed to initialize display");
    LOG_DEBUG("[TEST] %s\n", "Create Window");
    if(create_main_window()) LOG_ERR("failed to create window");
    sleep(1);
    LOG_DEBUG("[TEST] %s\n", "Close Window");
    close_main_window();
    sleep(1);
    LOG_DEBUG("[TEST] %s\n", "cleanup");
    cleanup();
    return;
}
