#ifndef UART_H
#define UART_H

void init_uart();
void put_char(unsigned char ch);
void put_str(unsigned char* data);
unsigned char get_ch();

#endif // !UART_H
