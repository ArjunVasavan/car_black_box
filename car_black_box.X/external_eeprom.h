#ifndef EEPROM_H
#define EEPROM_H

#define EEPROM_SLAVE_WRITE 0xA0 // EEPROM write address
#define EEPROM_SLAVE_READ 0xA1 // EEPROM read address

 // here the address is different compared to DS1307 so they dont conflict on bus

void write_external_eeprom(unsigned char address, unsigned char data);
unsigned char read_external_eeprom(unsigned char address);
void save_log();

#endif // !EEPROM_H
