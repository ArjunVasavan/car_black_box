#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "external_eeprom.h"
#include "uart.h"
#include "ds1307.h"

static unsigned char first_entry = 1;

void display_main_menu(void) {

    static unsigned char menu_index = 0;
    static unsigned char cursor = 0;
    static unsigned char selected = 0;

    if ( first_entry ) {
        menu_index = 0;
        cursor = 0;
        selected = 0;
        first_entry = 0;
        CLEAR_DISP_SCREEN;
    }

    const char* menu[] = {
        "VIEW LOG",
        "DOWNLOAD",
        "CLEAR LOG",
        "SET TIME",
    };

    unsigned char key = read_switches(STATE_CHANGE);

    if ( key == MK_SW1 && selected < 3 ) {
        selected += 1;
        menu_index = (selected > 0) ? selected - 1 : 0;
        cursor     = (selected == 0) ? 0 : 1;
        CLEAR_DISP_SCREEN;
    } else if ( key == MK_SW2 && selected > 0 ) {
        selected -= 1;
        menu_index = (selected > 0) ? selected - 1 : 0;
        cursor     = (selected == 0) ? 0 : 1;
        CLEAR_DISP_SCREEN;
    } else if ( key == MK_SW11 ) {
        first_entry = 1;
        if ( selected == 0 ) state = e_view_log;
        if ( selected == 1 ) state = e_download_log;
        if ( selected == 2 ) state = e_clear_log;
        if ( selected == 3 ) state = e_set_time;
    } else if ( key == MK_SW12 ) {
        menu_index = 0;
        cursor = 0;
        selected = 0;
        state = e_dashboard;
        CLEAR_DISP_SCREEN;
    }

    if ( cursor == 0 ) {
        clcd_print((unsigned char*)"->", LINE1(0));
        clcd_print((unsigned char*)"  ", LINE2(0));
    } else {
        clcd_print((unsigned char*)"  ", LINE1(0));
        clcd_print((unsigned char*)"->", LINE2(0));
    }

    clcd_print((unsigned char*)menu[menu_index],   LINE1(3));
    clcd_print((unsigned char*)menu[menu_index+1], LINE2(3));
}

void view_log(void) {

    static unsigned char read_index = 0;
    static unsigned char log_count = 0;

    // check no logs FIRST
    if ( write_flag == 0 && write_index == 0 ) {
        first_entry = 1;
        read_index = 0;
        log_count = 0;
        CLEAR_DISP_SCREEN;
        clcd_print((const unsigned char*)"NO LOGS TO", LINE1(3));
        clcd_print((const unsigned char*)"DISPLAY", LINE2(4));
        __delay_ms(2000);
        state = e_main_menu;
        return;
    }

    if ( first_entry ) {
        first_entry = 0;
        read_index = 0;
        log_count = 0;
        CLEAR_DISP_SCREEN;
        clcd_print((const unsigned char*)"SN SP  GR TIME", LINE1(0));
        clcd_print((const unsigned char*)"PRESS SW1/SW2", LINE2(0));
    }

    unsigned char key = read_switches(STATE_CHANGE);

    if ( key == MK_SW1 || key == MK_SW2 ) {

        if ( key == MK_SW1 ) {
            if ( write_flag == 0 ) {
                if ( ( read_index + 5 ) < write_index ) {
                    read_index += 5;
                    log_count += 1;
                }
            } else {
                if ( log_count < 9 ) {
                    read_index += 5;
                    log_count += 1;
                    if ( read_index >= 50 ) {
                        read_index = 0;
                    }
                }
            }
        } else if ( key == MK_SW2 && log_count > 0 ) {
            if ( read_index < 5 ) {
                read_index = 45;
            } else {
                read_index -= 5;
            }
            log_count -= 1;
        }

        unsigned char spd = read_external_eeprom(read_index + SPEED_ADDR);
        unsigned char gr  = read_external_eeprom(read_index + GEAR_ADDR);
        unsigned char hr  = read_external_eeprom(read_index + HOURS_ADDR);
        unsigned char mn  = read_external_eeprom(read_index + MINS_ADDR);
        unsigned char sc  = read_external_eeprom(read_index + SECS_ADDR);

        unsigned char spd_str[4];
        convert_speed_to_string(spd_str, spd);

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

        const char* serial_no[] = {
            "01","02","03","04","05",
            "06","07","08","09","10"
        };

        CLEAR_DISP_SCREEN;
        clcd_print((const unsigned char*)"SN SP  GR TIME", LINE1(0));
        clcd_print((const unsigned char*)serial_no[log_count], LINE2(0));
        clcd_print((const unsigned char*)spd_str, LINE2(3));

        if ( gr == 0 ) clcd_putch('N', LINE2(7));
        else if ( gr <= 5 ) clcd_putch(gr + '0', LINE2(7));
        else if ( gr == 6 ) clcd_putch('R', LINE2(7));
        else if ( gr == 7 ) clcd_putch('C', LINE2(7));

        clcd_print(time_str, LINE2(8));

    } else if ( key == MK_SW12 ) {
        first_entry = 1;
        read_index = 0;
        log_count = 0;
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }
}

void set_time(void) {

    CLEAR_DISP_SCREEN;

    unsigned char hr = 0, min = 0, sec = 0;
    unsigned char field = 1, blink_flag = 0;
    unsigned long int delay = 0;

    clcd_print((const unsigned char*)"HH:MM:SS",LINE1(3));

    while (1) {
        unsigned char key = read_switches(STATE_CHANGE);

        if ( key == MK_SW1 ) {
            if ( field == 1  ) {
                sec+=1;
                if ( sec == 60 ) {
                    sec = 0;
                }
            } else if ( field == 2 ) {
                min+=1;
                if ( min == 60 ) {
                    min = 0;
                }
            } else if ( field == 3 ) {
                hr+=1;
                if ( hr == 24 ) {
                    hr = 0;
                }
            }
        } else if ( key == MK_SW2 ) {

            if ( field == 1  ) {
                sec-=1;
                if ( sec == 255 ) {
                    sec = 59;
                }
            } else if ( field == 2 ) {
                min-=1;
                if ( min == 255 ) {
                    min = 59;
                }
            } else if ( field == 3 ) {
                hr-=1;
                if ( hr == 255 ) {
                    hr = 23;
                }
            }

        } else if ( key == MK_SW4 ) {
            field+=1;
            if ( field == 4 ) field = 1;

        } else if (key == MK_SW11 ) { // save time

            CLEAR_DISP_SCREEN;

            unsigned char bcd_hr = ( ( hr / 10 ) << 4 ) | ( hr % 10 );
            unsigned char bcd_min = ( ( min / 10 ) << 4 ) | ( min % 10 );
            unsigned char bcd_sec = ( ( sec / 10 ) << 4 ) | ( sec % 10 );

            write_ds1307(HOUR_ADDR, bcd_hr); 
            write_ds1307(MIN_ADDR,bcd_min); 
            write_ds1307(SEC_ADDR,bcd_sec); 

            clcd_print((const unsigned char*)"TIME SAVED!", LINE1(3));
            __delay_ms(2000);
            break;

        }

        // blinking logic 

        if ( delay++ >= 500 ) {
            delay = 0;
            blink_flag = !blink_flag;
        }

        // displaying time with blinking

        if ( field == 3 && blink_flag ) {
            clcd_print((const unsigned char*)"  ",LINE2(3));
        } else {
            clcd_putch(hr/10 + '0',LINE2(3));
            clcd_putch(hr%10 + '0',LINE2(4));
        }

        clcd_putch(':', LINE2(5));

        if ( field == 2 && blink_flag ) {
            clcd_print((const unsigned char*)"  ",LINE2(6));
        } else {
            clcd_putch(min/10 + '0',LINE2(6));
            clcd_putch(min%10 + '0',LINE2(7));
        }

        clcd_putch(':', LINE2(8));

        if ( field == 1 && blink_flag  ) {
            clcd_print((const unsigned char*)"  ",LINE2(9));
        } else {
            clcd_putch(sec/10 + '0',LINE2(9));
            clcd_putch(sec%10 + '0',LINE2(10));
        }
    }

    state = e_main_menu;
}

void download_log(void) {

    unsigned char read_index = 0;
    unsigned char count = 0;

    // no log condition
    if ( write_flag == 0 && write_index == 0 ) {
        put_str((unsigned char*)"NO LOGS TO PRINT\n\r");
        state  = e_main_menu;
        return;
    }

    // print header
    put_str((unsigned char*)"SN  SPD  GEAR  TIME\n\r");
    //serial numbers
    const char* serial_no[] = {
        "01","02","03","04","05",
        "06","07","08","09","10"
    };
    // sting from oldest log
    if ( write_flag == 1 ) {
        read_index = write_index;
    } else {
        read_index = 0;
    }

    for ( int i = 0 ; i <= 9 ; i++ ) {
        // stopping if buffer is not full and reached write_index
        if ( write_flag == 0 && read_index >= write_index ) break;

        // read log

        unsigned char spd = read_external_eeprom(read_index+SPEED_ADDR);
        unsigned char gr = read_external_eeprom(read_index+GEAR_ADDR);
        unsigned char hr = read_external_eeprom(read_index+HOURS_ADDR);
        unsigned char mn = read_external_eeprom(read_index+MINS_ADDR);
        unsigned char sc = read_external_eeprom(read_index+SECS_ADDR);

        // converting speed
        unsigned char spd_str[4];
        convert_speed_to_string(spd_str, spd);

        // converting time
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

        // send serial number
        put_str((unsigned char*)serial_no[count]);
        put_str((unsigned char *)" ");

        //send speed
        put_str(spd_str);
        put_str((unsigned char *)" ");

        // send gear
        if      ( gr == 0 ) put_char('N');
        else if ( gr <= 5 ) put_char(gr + '0');
        else if ( gr == 6 ) put_char('R');
        else if ( gr == 7 ) put_char('C');

        put_str((unsigned char*)"     ");

        // send time
        put_str(time_str);
        put_str((unsigned char*)"\n\r");

        count++;
        read_index += 5;
        if ( read_index >= 50 ) read_index = 0;
    }

    first_entry = 1;
    state = e_main_menu;
}

void clear_log(void) {
    CLEAR_DISP_SCREEN;
    clcd_print((const unsigned char*)"CLEARING", LINE1(4));
    clcd_print((const unsigned char*)"THE LOGS", LINE2(4));

    write_index = 0;
    write_flag = 0;

    write_external_eeprom(WRITE_INDEX_ADDR, write_index);
    write_external_eeprom(WRITE_FLAG_ADDR, write_flag);

    __delay_ms(2000);

    first_entry = 1;
    state = e_main_menu;
    CLEAR_DISP_SCREEN;
}

