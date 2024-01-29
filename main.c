#include "include/common.h"
#include "include/alert_box.h"

int main(void){
    LOG_DEBUG("%s\n", "test");
    LOG_INFO("info");

    if(init_display()) LOG_ERR("DISPLAY INIT FAILED");


done:
    destroy_display();
    return 0;
}
