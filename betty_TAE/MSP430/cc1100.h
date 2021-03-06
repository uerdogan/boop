/*
    cc1100.h
    Copyright (C) 2009  <telekatz@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef cc1100_H
#define cc1100_H

#define WRITE			0x00
#define BURST			0x40
#define READ			0x80
#define TX_fifo			0x7F
#define RX_fifo			0xff

#define SRES			0x30
#define SFSTXON			0x31
#define SXOFF			0x32
#define SCAL			0x33
#define SRX				0x34
#define STX 			0x35
#define SIDLE			0x36
#define SWOR			0x38
#define SPWD			0x39
#define SFRX			0x3A
#define SFTX			0x3B
#define SWORRST			0x3C
#define SNOP			0x3D
#define PTABLE			0x3E

#define cc1100_chan		0x01
#define cc1100_addr		0x02

const unsigned char conf[0x2F];

void cc1100_init(void);
unsigned char cc1100_write(unsigned char addr, unsigned char* dat, unsigned char lenght);
unsigned char cc1100_write1(unsigned char addr,unsigned char dat);
unsigned char cc1100_read(unsigned char addr, unsigned char* dat, unsigned char lenght);
unsigned char cc1100_read1(unsigned char addr);
unsigned char cc1100_strobe(unsigned char cmd);
unsigned char spi_rw(unsigned char write);


#endif
