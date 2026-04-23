#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"


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

    static unsigned char read_index = 0;
    static unsigned char log_count = 0;

    if ( write_flag == 0 && write_index == 0 ) {
        CLEAR_DISP_SCREEN;
        clcd_print((const unsigned char*)"NO LOGS TO ",LINE1(3));
        clcd_print((const unsigned char*)"DISPLAY",LINE2(4));
        __delay_ms(2000);
        state = e_main_menu;
        return;
    }

    unsigned char key = read_switches(STATE_CHANGE);

    if ( key == MK_SW1 ) {
        if ( write_flag == 0 ) { // buffer is not full as flag is 0 

            if ( ( read_index + 5 ) < write_index ) {
                read_index+=5;
                log_count+=1;
            }
        } else {// buffer is full and we can go through that 10 logs
            if ( log_count < 9 ) {
                read_index+=5;
                log_count+=1;
            }
            if ( read_index >= 50 ) {
                read_index = 0;
            }
        }

    } else if (key == MK_SW2 && log_count > 0 ) { // previous log

        if ( read_index < 5 ) {
            read_index = 45;
        } else {
            read_index-=5;
        }
        log_count-=1;
    } else if ( key == MK_SW12 ) {    // back to main menu

        read_index = 0;
        log_count = 0;
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
        return;
    }

    // read log from EEPROM

    unsigned char spd = read_external_eeprom(read_index+SPEED_ADDR);
    unsigned char gr = read_external_eeprom(read_index+GEAR_ADDR);
    unsigned char hr = read_external_eeprom(read_index+HOURS_ADDR);
    unsigned char mn = read_external_eeprom(read_index+MINS_ADDR);
    unsigned char sc = read_external_eeprom(read_index+SECS_ADDR);

    unsigned char spd_str[4];

    convert_speed_to_string(spd_str,spd);

    unsigned char time_str[9];
    time_str[0] = ( hr / 10 ) + '0';
    time_str[1] = ( hr % 10 ) + '0';
    time_str[2] = ':';
    time_str[3] = ( mn / 10 ) + '0';
    time_str[4] = ( mn % 10 ) + '0';
    time_str[5] = ':';
    time_str[6] = ( sc / 10 ) + '0';
    time_str[7] = ( sc % 10 ) + '0';
    time_str[8] = '\0';

    // serial number string:

    const char* serial_no[] = {
        "01","02","03","04","05",
        "06","07","08","09","10"
    };

    // displaying header
    clcd_print((const unsigned char*)"SN SP  GR TIME",LINE1(0));

    // displaying log
    clcd_print((const unsigned char*)serial_no[log_count],LINE2(0));
    clcd_print((const unsigned char*)spd_str,LINE2(3));

    // displaying gear
    if ( gr == 0 ) clcd_putch('N',LINE2(7));
    else if ( gr <= 5 ) clcd_putch(gr+'0',LINE2(7));
    else if ( gr == 6 ) clcd_putch('R',LINE2(7));

    // printing time
    clcd_print(time_str,LINE2(9));
}

void set_time(void) {

}

void download_log(void) {

}

void clear_log(void) {

}

