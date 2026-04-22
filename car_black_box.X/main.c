#include "black_box.h"
#include "clcd.h"
#include "timer.h"
#include "matrix_keypad.h"
#include "adc.h"
#include "i2c.h"
#include "ds1307.h"
#include "uart.h"

State_t state;

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

