#include "include/common.h"
#include "include/alert_box.h"
#include <unistd.h>

#define TEST
static void tests();

int main(void){

    if(init_display()) LOG_ERR("DISPLAY INIT FAILED");

#ifdef TEST
    tests();
#else
    create_main_window();
#endif
    cleanup();
    return 0;
}

static void tests(){
    LOG_DEBUG("%s\n", "Create Window");
    create_main_window();
    sleep(1);
    LOG_DEBUG("%s\n", "Close Window");
    close_main_window();
    sleep(1);
    LOG_DEBUG("%s\n", "Create Window");
    create_main_window();
    sleep(3);
    return;
}
