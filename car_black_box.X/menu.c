#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"


void display_main_menu(void) {

    static unsigned char menu_index = 0;
    static unsigned char cursor = 0;
    static unsigned char selected = 0;

    const char* menu[] = {
        "VIEW LOG",
        "DOWNLOAD",
        "SET TIME",
        "CLEAR LOG"
    };

    unsigned char key = read_switches(STATE_CHANGE);

    if ( key == MK_SW1 && selected < 3 ) {
        selected+=1;
        if ( cursor == 0 ) {
            cursor = 1;
        } else {
            cursor = 0;
            menu_index+=1;
        }
        CLEAR_DISP_SCREEN;
    } else if ( key == MK_SW2 && selected > 0 ) {
        selected--;
        if ( cursor == 1 ) {
            cursor = 0;
        } else {
            cursor = 1;
            menu_index-=1;
        }
        CLEAR_DISP_SCREEN;
    } else if (key == MK_SW11) {
        if ( selected == 0 ) state = e_view_log;
        if ( selected == 1 ) state = e_download_log;
        if ( selected == 2 ) state = e_set_time;
        if ( selected == 3 ) state = e_clear_log;
    } else if ( key == MK_SW12 ) {
        // reset menu state before leaving
        menu_index = 0;
        cursor = 0;
        selected = 0;
        state = e_dashboard;
        CLEAR_DISP_SCREEN;
    }

    //display

    if ( cursor == 0 ) {
        clcd_print((unsigned char*)"->",LINE1(0));
        clcd_print((unsigned char*)"  ",LINE2(0));
    } else {
        clcd_print((unsigned char*)"  ",LINE1(0));
        clcd_print((unsigned char*)"->",LINE2(0));
    }

    clcd_print((unsigned char*)menu[menu_index],LINE1(3));
    clcd_print((unsigned char*)menu[menu_index+1],LINE2(3));

}

void view_log(void) {

}

void set_time(void) {

}

void download_log(void) {

}

void clear_log(void) {

}

