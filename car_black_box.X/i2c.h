#ifndef I2C_H
#define I2C_H

void init_i2c();
void i2c_start();
void i2c_rep_start();
void i2c_stop();
void i2c_write(unsigned char data);
unsigned char i2c_read();

#endif // !I2C_H
