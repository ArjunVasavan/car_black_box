#include <xc.h>
#include "clcd.h"

void clcd_write(unsigned char bit_values, unsigned char control_bit) {

    CLCD_RS = control_bit;
    CLCD_PORT = bit_values;

    CLCD_EN = HI;
    CLCD_EN = LO;

    PORT_DIR = INPUT;
    CLCD_RW = HI;
    CLCD_RS = INSTRUCTION_COMMAND;

    do {

        CLCD_EN = HI;
        CLCD_EN = LO;

    } while (CLCD_BUSY);

    CLCD_RW = LO;
    PORT_DIR = OUTPUT;

}

