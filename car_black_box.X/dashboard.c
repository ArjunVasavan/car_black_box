#include "black_box.h"
#include "clcd.h"
#include "adc.h"
#include "matrix_keypad.h"
#include "ds1307.h"
#include "external_eeprom.h"


static const char* gear_display[] = {"N","1","2","3","4","5","R"};

static unsigned char line1[17] = " TIME    EV  SP ";
static unsigned char time[9]   = "00:00:00";
static unsigned char event[3]  = "ON";
static unsigned char speed[4]  = "00";

void display_labels() {
    clcd_print(line1,LINE1(0));
}

void save_log() {
    write_external_eeprom(SPEED_ADDR+write_index,speed_val);
    write_external_eeprom(GEAR_ADDR+write_index, gear);
    write_external_eeprom(HOURS_ADDR+write_index,hours);
    write_external_eeprom(MINS_ADDR+write_index,minutes);
    write_external_eeprom(SECS_ADDR+write_index,seconds);

    write_index+=5;

    if ( write_index >= 50 ) {
        write_index = 0;
        write_flag = 1;
        write_external_eeprom(WRITE_FLAG_ADDR,write_flag);
    }

    write_external_eeprom(WRITE_INDEX_ADDR,write_index);
}


void read_real_time_clock() {

    /* NOTE: BCD formats
     * Second register: ( it has Clock Halt register )
     *                  bit:  7   6 5 4   3 2 1 0
     *                        CH  tens    ones
     * Minute register: ( it doesnt have clock halt )
     *                  bit:  7   6 5 4   3 2 1 0
     *                        0   tens    ones
     * Hour register : ( we only need one bit from tens as its hour ) 
     *                  bit:  7 6   5   4   3 2 1 0
     *                        0 0   0   tens   ones
     */

    // reading raw BCD format
    unsigned char raw_sec = read_ds1307(SEC_ADDR);
    unsigned char raw_min = read_ds1307(MIN_ADDR);
    unsigned char raw_hr = read_ds1307(HOUR_ADDR);
     // converting BCD format

    seconds = ( ( ( ( raw_sec >> 4 ) & 0x07 ) * 10 ) + ( raw_sec & 0x0F) );
    minutes = ( ( ( ( raw_min >> 4 ) & 0x0F ) * 10 ) + ( raw_min & 0x0F) );
    hours = ( ( ( ( raw_hr >> 4 ) & 0x01 ) * 10 ) + ( raw_hr & 0x0F) );
}

void convert_speed_to_string(unsigned char* speed, unsigned char spd ) {
    speed[0] = ( spd / 100 ) % 10 + '0';
    speed[1] = ( spd / 10 )  % 10 + '0';
    speed[2] = ( spd )       % 10 + '0';
    speed[3] = '\0';
}

void read_speed_adc() {
    unsigned short adc_val = read_adc(CHANNEL4); // CHANNEL4 is potentiometer
    unsigned char spd = adc_val/ 10.23; // scale 0 - 1023 to 0 - 100
    convert_speed_to_string(speed,spd);
    speed_val = adc_val / 10.23;
}


void update_time() {
        time[0] = ( hours / 10 ) + '0';
        time[1] = ( hours % 10 ) + '0';
        time[2] = ':';
        time[3] = ( minutes / 10 ) + '0';
        time[4] = ( minutes % 10 ) + '0';
        time[5] = ':';
        time[6] = ( seconds / 10 ) + '0';
        time[7] = ( seconds % 10 ) + '0';
        time[8] = '\0';
}

void show_collision() {
    CLEAR_DISP_SCREEN;
    clcd_print((const unsigned char*)"COLLISION",LINE1(3));
    clcd_print((const unsigned char*)"HAPPEN",LINE2(4));

    while (1) {
        unsigned char key = read_switches(STATE_CHANGE);
        if ( key != ALL_RELEASED ) {
            break;
        }
    }
}

void view_dashboard() {
    read_real_time_clock();
    read_speed_adc();
    update_time();
    display_labels();
    clcd_print(time,   LINE2(0));   // col 0  → 00:00:00
    clcd_print((const unsigned char*)gear_display[gear],LINE2(9));
    clcd_print(speed,  LINE2(13));  // col 13 → under SP
    unsigned char key = read_switches(STATE_CHANGE);
    if ( key == MK_SW11 ) {
        CLEAR_DISP_SCREEN;
        state = e_main_menu;
    } else if ( key == MK_SW1 && gear < 6 ) {
        gear+=1;
        save_log();
    } else if ( key == MK_SW2 &&  gear > 0 ) {
        gear-=1;
        save_log();
    } else if ( key == MK_SW3 ) {
        show_collision();
        gear = 0;
    }
}

