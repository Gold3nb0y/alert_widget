#include <stdint.h>
#include <sys/types.h>

uint64_t init_display();
uint64_t cleanup();
void close_main_window();
void draw_alert(char* bg_img_filename);
