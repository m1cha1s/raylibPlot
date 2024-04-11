#ifndef _SERIAL_H
#define _SERIAL_H

int serial_open(char* port_name);
int serial_is_open();
int serial_close();
int serial_read_line(char* res);
int serial_write(char c);

#endif
