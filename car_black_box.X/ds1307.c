#include <xc.h>
#include "ds1307.h"
#include "i2c.h"

/* NOTE: DS1307 (Real Time Clock Chip)
 * its a dedicated clock chip that keeps track of seconds, minutes, hours, date
 * it has its own battery so it keeps running even when battery is off 
 * DS1307 is an slave on I2C bus
 * DS1307 stores values in BCD format ( Binary Coded Decimal )
 * Normal binary:
 * 45 seconds = 0010 1101
 * BCD:
 * 45 seconds = 0100 0101
 *              ↑↑↑↑ ↑↑↑↑
 *               4    5
 */

void init_ds1307() {
    /* NOTE: SEC_ADDR
     * Bit:  7    6    5    4    3    2    1    0
     *       CH   --seconds in BCD-----------
     * CH -> clock halt bit
     * CH = 1 -> clock stopped 
     * CH = 0 -> clock running
     * dummy | 0x80 -> sets the CH = 1 then we are writin it on RTC which halts the clock
     * The reason for this halting is to prevent garbage values to get written when setting things up
     */
    unsigned char dummy;
    dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, dummy | 0x80 );

    /* NOTE: HOUR_ADDR
     * Bit:  7    6    5    4    3    2    1    0
     *       0   12/24  --hours in BCD--------
     * if Bit 6 = 1  -> 12 hour format
     * if Bit 6 = 0  -> 24 hour format
     * dummy | 0x40 here we are forcing to 12hr format
     */
    dummy = read_ds1307(HOUR_ADDR);
    write_ds1307(HOUR_ADDR, dummy | 0x40 );

    /* NOTE: CNTL_ADDR ( control register )
     * Bit:  7    6    5    4    3    2    1    0
     i   *      OUT   0   OSF  SQWE  0    0   RS1  RS0
     * 0x93 = 1001 0011
     * OUT = 1  -> Output level high
     * OSF = 0  -> oscillator running
     * SQWE = 1 -> square wave Output enabled
     * RS1 = 1  and RS0 = 1 -> together = 11 => 32.768 KHz square wave output
     */
    write_ds1307(CNTL_ADDR, 0x93);

    /* This is to start clock again
     * 0x7F      = 0111 1111
     * dummy     = 1101 0110  (current seconds with CH=1)
     * AND result= 0101 0110  → CH bit forced to 0 → clock running
     */
    dummy = read_ds1307(SEC_ADDR);
    write_ds1307(SEC_ADDR, dummy & 0x7F );
}

void write_ds1307(unsigned char address, unsigned char data) {
    i2c_start(); // taking control of bus
    i2c_write(SLAVE_WRITE); // writing DS1307 in write mode
    i2c_write(address); // go to this particular register
    i2c_write(data); // store value there
    i2c_stop(); // stopped and releasing bus

    /* START → 0xD0 → ACK → 0x00 → ACK → 0x45 → ACK → STOP
     *   ↑              ↑            ↑
     *   DS1307        SEC_ADDR      seconds=45
     *  write mode
     */
}


