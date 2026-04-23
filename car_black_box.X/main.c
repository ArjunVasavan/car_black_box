#include "black_box.h"
#include "clcd.h"
#include "timer.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"
#include "external_eeprom.h"

State_t state;

unsigned char write_index = 0;
unsigned char write_flag  = 0;
unsigned char gear        = 0;
unsigned char speed_val   = 0;
unsigned char hours       = 0;
unsigned char minutes     = 0;
unsigned char seconds     = 0;

void init_config() {
    state = e_dashboard;
    init_clcd();
    init_timer0();
    init_matrix_keypad();
    init_adc();
    init_i2c();
    init_ds1307();
    init_uart();
}

int main(void) {
    init_config();

    // checking if EEPROM is used before

    if ( read_external_eeprom(INIT_FLAG_ADDR) == INIT_FLAG_VAL ) {
        // This means EEPROM is saved before so load saved values
        write_flag = read_external_eeprom(WRITE_FLAG_ADDR);
        write_index = read_external_eeprom(WRITE_INDEX_ADDR);
    } else {
        // starting from zero
        write_flag = 0;
        write_index = 0;
    }

    // now marking EEPROM as used
    write_external_eeprom(INIT_FLAG_ADDR,INIT_FLAG_VAL);

    while(1) {
        // Detect key press
        switch (state) {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;

            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;

            case e_view_log:
                // Display dashboard
                view_log();
                break;

            case e_download_log:
                download_log();
                break;

            case e_clear_log:
                clear_log();
                break;


            case e_set_time:
                set_time();
                break;

        }

    }

}
