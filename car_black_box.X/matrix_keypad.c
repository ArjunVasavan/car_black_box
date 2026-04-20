#include <xc.h>
#include "matrix_keypad.h"

void init_matrix_keypad() {
    ADCON1 = 0x0F; // Making all pins as digital
                   
    /* NOTE: TRISB = 0x1E
* Bit:   7    6    5    4    3    2    1    0
              0    0    0    1    1    1    1    0
             RB7  RB6  RB5  RB4  RB3  RB2  RB1  RB0
             OUT  OUT  OUT  IN   IN   IN   IN   ---
             ROW3 ROW2 ROW1 COL4 COL3 COL2 COL1
     */
    TRISB = 0x1E; // RB5, RB6, RB7 -> output & RB1, RB2, RB3, RB4 -> input

    RBPU = 0; // RortB PullUP, RBPU = 0 Means PullUP is Enabled

    MATRIX_KEYPAD_PORT = MATRIX_KEYPAD_PORT | 0xE0;
    /* NOTE: MATRIX_KEYPAD_PORT
     * Bit:   7    6    5    4    3    2    1    0
              1    1    1    0    0    0    0    0
              RB7  RB6  RB5
              ROW3 ROW2 ROW1  → all set HIGH
     */
}


unsigned char scan_key() {

    ROW1 = LO;
    ROW2 = HI;
    ROW3 = HI;

    if ( COL1 == LO ) {
        return 1;
    } else if ( COL2 == LO ) {
        return 4;
    } else if ( COL3 == LO ) {
        return 7;
    } else if ( COL4 == LO ) {
        return 10;
    }

    ROW1 = HI;
    ROW2 = LO;
    ROW3 = HI;

    if ( COL1 == LO ) {
        return 2;
    } else if ( COL2 == LO ) {
        return 5;
    } else if ( COL3 == LO ) {
        return 8;
    } else if ( COL4 == LO ) {
        return 11;
    }

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = LO;

    if ( COL1 == LO ) {
        return 3;
    } else if ( COL2 == LO ) {
        return 6;
    } else if ( COL3 == LO ) {
        return 9;
    } else if ( COL4 == LO ) {
        return 12;
    }

    ROW1 = HI;
    ROW2 = HI;
    ROW3 = HI;

    return 0xFF;
}

unsigned char read_switches(unsigned char detection_type) {
    static unsigned char once = 1;
    static unsigned char key;

    if ( detection_type == STATE_CHANGE ) {
        key = scan_key();
        if ( key != 0xFF && once ) {
            once = 0;
            __delay_ms(20); // debounce delay
            return key;
        } else if ( key == 0xFF ) {
            once = 1;
        }
    } else if ( detection_type == LEVEL_CHANGE ) {
        return scan_key();
    }
    return 0xFF;
}
