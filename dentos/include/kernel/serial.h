#ifndef DENTOS_SERIAL_H
#define DENTOS_SERIAL_H

#include <stddef.h>

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

void serial_configure_baud_rate(unsigned short, unsigned short);
void serial_configure_line(unsigned short);
void serial_configure_buffer(unsigned short);
void serial_configure_modem(unsigned short);
int serial_is_transmit_fifo_empty(unsigned int);
void serial_write(unsigned short, const char*, size_t);
int printk(const char* restrict format, ...);
int putsk(const char*);

#endif //DENTOS_SERIAL_H
