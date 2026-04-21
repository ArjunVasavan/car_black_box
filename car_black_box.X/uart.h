#ifndef UART_H
#define UART_H

void init_uart();
void put_char(unsigned char ch);
void put_str(unsigned char* data);
unsigned char getch();

#endif // !UART_H
