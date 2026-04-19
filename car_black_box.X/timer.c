#include <xc.h>
#include "timer0.h"

/* NOTE: TIMER0
 * Its an Hardware counter inside the PIC 
 * it counts up automatically and when it overflows it triggers an interrupt
 */

void init_timer0(void) {
    T08BIT = 1; // Set to 8 bit mode i.e. count from 0 to 255 the overflow
    T0CS = 0; // 0 --> internal clock source
    TMR0ON = 1; // Turns timer0 ON
    PSA = 1; // Prescalar is not assigned so timer0 runs at full speed with no Prescalar
    TMR0 = 6; // PRELOAD value : insted of 0 - 256 start from 6 - 256 i.e. 250 steps
              // the reason for 250 steps is : at 20 MHz instruction cycle is 0.2us
              // 250 x 0.2 = 50 us per overflow
    TMR0IF = 0; // clearing timer0 interrupt flag
    TMR0IE = 1; // allowing timer0 to trigger interrupts
}
