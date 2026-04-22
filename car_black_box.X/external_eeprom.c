#include <xc.h>
#include "external_eeprom.h"
#include "i2c.h"

/* NOTE: External EEPROM
 * ELECTRICALLY ERASABLE PROGRAMMABLE READ ONLY MEMORY
 * It saves Data even when power is OFF
 * It can be written and read many times
 * Used to Store logs in this project
 * Its an Slave of I2C bus
 */


void write_external_eeprom(unsigned char address, unsigned char data) {
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
    /* NOTE: Write Cycle Time
     * EEPROM is slower than DS1307. After writing it need to physically save the
     * data internally. this is called write cycle time
     * without waiting if we write an byte on EEPROM and immediatly trying to 
     * read it back EEPROM is still saving there thus we can get an garbage
     * with wait EEPROM can finish the writing at that time
     */
    for ( unsigned int wait = 3000; wait--; );
}

unsigned char read_external_eeprom(unsigned char address) {
    i2c_start();
    i2c_write(EEPROM_SLAVE_WRITE);
    i2c_write(address);
    i2c_rep_start();
    i2c_write(EEPROM_SLAVE_READ);
    unsigned char data = i2c_read();
    i2c_stop();
    return data;
}

