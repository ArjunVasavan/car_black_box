#include "black_box.h"
#include "clcd.h"
#include "timer.h"
#include "adc.h"

static unsigned char line1[17] = " TIME    EV  SP ";
static unsigned char time[9]   = "00:00:00";
static unsigned char event[3]  = "ON";
static unsigned char speed[3]  = "00";
static unsigned char seconds = 0;
static unsigned char minutes = 0;
static unsigned char hours = 0;

void display_labels() {
    clcd_print(line1,LINE1(0));
}

void update_time() {
    if ( timer_count == 0 ) { // this means timer_count just reset i.e. 1 seconds
        seconds++;
        if ( seconds >= 60 ) { 
            seconds = 0;
            minutes++;
        }
        if ( minutes >= 60 ) {
            minutes = 0;
            hours++;
        }
        if ( hours >= 24 ) {
            hours = 0;
        }

        time[0] = ( hours / 10 ) + '0';
        time[1] = ( hours % 10 ) + '0';
        time[2] = ':';
        time[3] = ( minutes / 10 ) + '0';
        time[4] = ( minutes % 10 ) + '0';
        time[5] = ':';
        time[6] = ( seconds / 10 ) + '0';
        time[7] = ( seconds % 10 ) + '0';
    }
}


void view_dashboard() {
    update_time();
    display_labels();
    clcd_print(time,   LINE2(0));   // col 0  → 00:00:00
    clcd_print(event,  LINE2(9));   // col 9  → under EV
    clcd_print(speed,  LINE2(13));  // col 13 → under SP
}

