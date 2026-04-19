#include "black_box.h"
#include "clcd.h"

unsigned char line1[17] = " TIME    EV  SP ";
unsigned char time[9]   = "00:00:00";
unsigned char event[3]  = "00";
unsigned char speed[3]  = "00";

void view_dashboard() {
    clcd_print(line1,  LINE1(0));
    clcd_print(time,   LINE2(0));   // col 0  → 00:00:00
    clcd_print(event,  LINE2(9));   // col 9  → under EV
    clcd_print(speed,  LINE2(13));  // col 13 → under SP
}

void display_main_menu(void) {

}

void view_log(void) {

}

void set_time(void) {

}

void download_log(void) {

}

void clear_log(void) {

}
