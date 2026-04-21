#include <xc.h>
#include "uart.h"

/* NOTE: UART
 * its a way to send data serially ( one bit at a time ) between PIC and a PC / terminal
 * it has two wires only TX ( transmit ) and RX ( receive )
 */
void init_uart() {
    TRISC6 = 0; // TX pin setted as output
    TRISC7 = 1; // RX pin setted as input

    TXEN = 1; // Transmit Enable - for activating the transmitter
    SYNC = 0; // 0 -> Asynchronous mode ( UART )
    BRGH = 1; // Setting Baud rate as high speed mode, high speed gives better accuracy

    SPEN = 1; // Serial port enable -> turns ON the whole UART module 
    CREN = 1; // Continous receive enable -> keeps receiver always listening
              // if CREN = 1 -> we have to manually trigger each receive
    ABDEN = 0; // Auto Baud rate detect is disabled
    WUE = 0; // Wake up enable = disabled, used to wake PIC from sleep on RX activity
    BRG16 = 0; // 0 -> 8 bit Baud rate register ( just SPBRG )
    SPBRG = 129; // This for controlling how fast bit is send both side must agree on same speed
    /* 
       The formula with BRGH=1, BRG16=0:
       SPBRG = (Fosc / (16 × BaudRate)) - 1
       = (20,000,000 / (16 × 9600)) - 1
       = (20,000,000 / 153,600) - 1
       = 130.2 - 1
       = 129.2 ≈ 129
       */
} 
