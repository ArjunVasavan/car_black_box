#ifndef DS1307_H
#define	DS1307_H

#define SLAVE_WRITE		0xD0 // Write Mode
#define SLAVE_READ		0xD1 // Read Mode

#define SEC_ADDR		0x00 // Seconds
#define MIN_ADDR		0x01 // Minutes
#define HOUR_ADDR		0x02 // Hours
#define DAY_ADDR		0x03 // Day
#define DATE_ADDR		0x04 // Date
#define MONTH_ADDR		0x05 // Month
#define YEAR_ADDR		0x06 // Year

#define CNTL_ADDR		0x07 // Control Register

void init_ds1307(void);
void write_ds1307(unsigned char address,  unsigned char data);
unsigned char read_ds1307(unsigned char address);

#endif

