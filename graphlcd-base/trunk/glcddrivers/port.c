/*
 * GraphLCD driver library
 *
 * port.c  -  parallel port class with low level routines
 *
 * This file is released under the GNU General Public License. Refer
 * to the COPYING file distributed with this package.
 *
 * (c) 2004 Andreas Regel <andreas.regel AT powarman.de>
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/io.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>

#include "port.h"

namespace GLCD
{

static inline int port_in(int port)
{
	unsigned char value;
	__asm__ volatile ("inb %1,%0"
	                  : "=a" (value)
	                  : "d" ((unsigned short) port));
	return value;
}

static inline void port_out(unsigned short int port, unsigned char val)
{
	__asm__ volatile ("outb %0,%1\n"
	                  :
	                  : "a" (val), "d" (port));
}

cParallelPort::cParallelPort()
:	fd(-1),
	port(0),
	usePPDev(false)
{
}

cParallelPort::~cParallelPort()
{
}

int cParallelPort::Open(int portIO)
{
	usePPDev = false;
	port = portIO;

	if (port < 0x400)
	{
		if (ioperm(port, 3, 255) == -1)
		{
			syslog(LOG_ERR, "glcd drivers: ERROR ioperm(0x%X) failed! Err:%s (cParallelPort::Open)\n",
					port, strerror(errno));
			return -1;
		}
	}
	else
	{
		if (iopl(3) == -1)
		{
			syslog(LOG_ERR, "glcd drivers: ERROR iopl failed! Err:%s (cParallelPort::Init)\n",
					strerror(errno));
			return -1;
		}
	}
	return 0;
}

int cParallelPort::Open(const char * device)
{
	usePPDev = true;

	fd = open(device, O_RDWR);
	if (fd == -1)
	{
		syslog(LOG_ERR, "glcd drivers: ERROR cannot open %s. Err:%s (cParallelPort::Init)\n",
				device, strerror(errno));
		return -1;
	}

	if (ioctl(fd, PPCLAIM, NULL) == -1)
	{
		syslog(LOG_ERR, "glcd drivers: ERROR cannot claim %s. Err:%s (cParallelPort::Init)\n",
				device, strerror(errno));
		close(fd);
		return -1;
	}

	int mode = PARPORT_MODE_PCSPP;
	if (ioctl(fd, PPSETMODE, &mode) == -1)
	{
		syslog(LOG_ERR, "glcd drivers: ERROR cannot setmode %s. Err:%s (cParallelPort::Init)\n",
				device, strerror(errno));
		close(fd);
		return -1;
	}

	return 0;
}

int cParallelPort::Close()
{
	if (usePPDev)
	{
		if (fd != -1)
		{
			ioctl(fd, PPRELEASE);
			close(fd);
			fd = -1;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		if (port < 0x400)
		{
			if (ioperm(port, 3, 0) == -1)
			{
				return -1;
			}
		}
		else
		{
			if (iopl(0) == -1)
			{
				return -1;
			}
		}
	}
	return 0;
}

void cParallelPort::Claim()
{
	if (usePPDev)
		ioctl(fd, PPCLAIM);
}

void cParallelPort::Release()
{
	if (usePPDev)
		ioctl(fd, PPRELEASE);
}

void cParallelPort::SetDirection(int direction)
{
	if (usePPDev)
	{
		if (ioctl(fd, PPDATADIR, &direction) == -1)
		{
			perror("ioctl(PPDATADIR)");
			//exit(1);
		}
	}
	else
	{
		if (direction == kForward)
			port_out(port + 2, port_in(port + 2) & 0xdf);
		else
			port_out(port + 2, port_in(port + 2) | 0x20);
	}
}

unsigned char cParallelPort::ReadControl()
{
	unsigned char value;

	if (usePPDev)
	{
		if (ioctl(fd, PPRCONTROL, &value) == -1)
		{
			perror("ioctl(PPRCONTROL)");
			//exit(1);
		}
	}
	else
	{
		value = port_in(port + 2);
	}

	return value;
}

void cParallelPort::WriteControl(unsigned char value)
{
	if (usePPDev)
	{
		if (ioctl(fd, PPWCONTROL, &value) == -1)
		{
			perror("ioctl(PPWCONTROL)");
			//exit(1);
		}
	}
	else
	{
		port_out(port + 2, value);
	}
}

unsigned char cParallelPort::ReadStatus()
{
	unsigned char value;

	if (usePPDev)
	{
		if (ioctl(fd, PPRSTATUS, &value) == -1)
		{
			perror("ioctl(PPRSTATUS)");
			//exit(1);
		}
	}
	else
	{
		value = port_in(port + 1);
	}

	return value;
}

unsigned char cParallelPort::ReadData()
{
	unsigned char data;

	if (usePPDev)
	{
		if (ioctl(fd, PPRDATA, &data) == -1)
		{
			perror("ioctl(PPRDATA)");
			//exit(1);
		}
	}
	else
	{
		data = port_in(port);
	}

	return data;
}

void cParallelPort::WriteData(unsigned char data)
{
	if (usePPDev)
	{
		if (ioctl(fd, PPWDATA, &data) == -1)
		{
			perror("ioctl(PPWDATA)");
			//exit(1);
		}
	}
	else
	{
		port_out(port, data);
	}
}

} // end of namespace
