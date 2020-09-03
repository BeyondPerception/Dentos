#ifndef DENTOS_SERIAL_H
#define DENTOS_SERIAL_H

#include <stddef.h>

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

void serial_configure_baud_rate(unsigned short com, unsigned short divisor);
void serial_configure_line(unsigned short com);
void serial_configure_buffer(unsigned short com);
void serial_configure_modem(unsigned short com);
int serial_is_transmit_fifo_empty(unsigned int com);
void serial_write(unsigned short com, const char* data, size_t len);
int printk(const char* restrict format, ...);
int putsk(const char* string);

#endif //DENTOS_SERIAL_H
