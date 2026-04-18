#ifndef CLCD_H
#define CLCD_H

#define CLCD_PORT PORTD
#define CLCD_EN RC2
#define CLCD_RS RC1
#define CLCD_RW RC0
#define CLCD_BUSY RD7
#define PORT_DIR TRISF7

#define HI 1
#define LO 0

#define INPUT 0xFF
#define OUTPUT 0x00

#define DATA_COMMAND 1
#define INSTRUCTION_COMMAND	0

/*
 * NOTE: _XTAL_FREQ
 * This tells the compiler the PIC's clock speed so delay function works correctly
 */

#define _XTAL_FREQ  20000000 // 20 MHz crystal used for __delay_ms()



#define LINE1(x) (0x80 + (x)) // position on row 1 column x
#define LINE2(x) (0xC0 + (x)) // position on row 2 column x


/*
 * NOTE: TWO_LINE_5x8_MATRIX_8_BIT
 * 0x38 in bits
 * Bit:   7   6   5   4   3   2   1   0
          0   0   1   1   1   0   0   0
               ↑   ↑   ↑
               |   |   |
               |   |   └── F  = 0 → 5x8 dot font
               |   └────── N  = 1 → 2 line display
               └────────── DL = 1 → 8-bit data mode
 */

#define TWO_LINE_5x8_MATRIX_8_BIT clcd_write(0x38, INSTRUCTION_COMMAND)
#define CLEAR_DISP_SCREEN clcd_write(0x01, INSTRUCTION_COMMAND)
#define CURSOR_HOME	clcd_write(0x02, INSTRUCTION_COMMAND)
#define DISP_ON_AND_CURSOR_OFF clcd_write(0x0C, INSTRUCTION_COMMAND)
#define EIGHT_BIT_MODE 0x33


void init_clcd(void);
void clcd_print(const unsigned char *data, unsigned char addr);
void clcd_putch(const unsigned char data, unsigned char addr);
void clcd_write(unsigned char bit_values, unsigned char control_bit);

#endif // !CLCD_H
