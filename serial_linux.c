#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define __USE_MISC

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

#include "serial.h"

int serial_port = -1;

int serial_open(char* port_name)
{
	serial_port = open(port_name, O_RDWR);
	
	if (serial_port < 0)
	{
		printf("Error %i from open: %s\n", errno, strerror(errno));
		return -1;
	}
	
	struct termios tty;
	
	if (tcgetattr(serial_port, &tty) != 0)
	{
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return -2;
	}
	
	// Disable parity, 1 stop bit, clear size bits
	tty.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
	
	// 8 bits per byte, something else
	tty.c_cflag |= CS8 | CREAD | CLOCAL;
	
	// Disable canonical mode, and echo, and signal chars
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);
	
	// Disable software flow controll and special handling
	tty.c_iflag &= ~(IXON | IXOFF | IXANY | IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
	
	// Disable output char interpretation
	tty.c_oflag &= ~(OPOST | ONLCR);
	
	// Set up for polling approach THIS MAY CAUSE PROBLEMS
	tty.c_cc[VTIME] = 0;
	tty.c_cc[VMIN] = 0;
	
	// Set baud rate
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
	{
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return -3;
	}
	
	return 0;
}

int serial_is_open()
{
	return serial_port >= 0;
}

int serial_close()
{
	close(serial_port);
	serial_port = -1;
	return 0;
}

int line_end = 0;
char line_buffer[1024] = {0};

int serial_read_line(char* res)
{
	int n = 0;
	do
	{
		char buffer[1] = {0};
		n = read(serial_port, &buffer, sizeof(buffer));

		for (int i = 0; i < n; i++)
		{
			if (i + line_end >= sizeof(line_buffer)) return -1; // Line buffer overflow
			
			line_buffer[i+line_end] = buffer[i];
			if (buffer[i] == '\n') 
			{
				line_buffer[i] = '\0';
				memcpy(res, line_buffer, i+1);
				line_end = 0;
				return 1;
			}
			line_end++;
		}
	}
	while (n);
	
	return 0;
}

// To be implemented!!!
int serial_write(char c)
{
}

