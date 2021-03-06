/*
    timerfuncs.c - timer control functions
    Copyright (C) 2007  Ch. Klippel <ck@mamalala.net>

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

#include "lpc2220.h"
#include "timerfuncs.h"
#include "irq.h"
#include "keyboard.h"

extern struct CB callbacks[MAX_CB];
extern unsigned long* timeouts[MAX_TO];

void startTimerIRQ(void)
{
	int i;
	struct CB *cur_cb;

	for(i=0; i<MAX_CB; i++)
	{
		cur_cb = &callbacks[i];
		cur_cb->state	= SLOT_FREE;
		cur_cb->exec	= 0;
	}
	
	for(i=0; i<MAX_TO; i++)
		timeouts[i] = 0;
	
	T0TCR = 0x02;	// reset timer
	T0TC = 1870;
	T0PR = 0x0e; 	// 15.000.000 Hz / 15 = 1.000.000 Hz --> PR = 15 - 1 = 0x0e
	T0MR0 = 250;	//  1.000.000 Hz / 250 =  4000 Hz = 0,25msec intervall time
	T0MCR = 0x03;	// reset and issue IRQ on TC == MR0
	T0TCR = 0x01;	// enable timer
	
	T0overflow = 0;
	
	VICVectAddr15 = (unsigned long)&(timerIRQ);
	VICVectCntl15 = VIC_SLOT_EN | INT_SRC_TIMER0;
	VICIntEnable = INT_TIMER0;
}

void wait5ms(void) {
	
	unsigned long x;
	
	x=T0TC;
	while(x==T0TC);
	while(x!=T0TC);
}

void waitus(unsigned long d) {
	
	unsigned long x,y;
	
	x=(T0TC+d)%5000;
	y= T0overflow;
	
	if(x<T0TC)
		while(y==T0overflow);
	while(x>T0TC);
}

unsigned int addTimerCB(cb_fn cb, unsigned short intervall)
{
	int i;
	struct CB *cur_cb;

	for(i=0; i<MAX_CB;)
	{
		cur_cb = &callbacks[i];
		if(cur_cb->state == SLOT_FREE)
		{
			cur_cb->exec		= cb;
			cur_cb->intervall	= intervall;
			cur_cb->cur_intervall	= 0;
			cur_cb->state		= SLOT_HALT | SLOT_USED;
			return i;
		}
		i++;
	}
	return 0xFF;
}

unsigned int removeTimerCB(unsigned int cb)
{
	if(cb < MAX_CB)
	{
		callbacks[cb].state	= SLOT_FREE;
		return MAX_CB;
	}
	return 0xFF;
}

void startCB(unsigned int cb)
{
	if(cb < MAX_CB)
	{
			callbacks[cb].state &= ~SLOT_HALT;
	}
}

void stopCB(unsigned int cb)
{
	if(cb < MAX_CB)
	{
			callbacks[cb].state |= SLOT_HALT;
	}
}

void setCBIntervall(unsigned int cb, unsigned short intervall)
{
	if(cb < MAX_CB)
	{
			callbacks[cb].intervall = intervall;
			callbacks[cb].cur_intervall = 0;
	}
}

unsigned char addTimeout(volatile unsigned long* counter, unsigned long value) {

	int i;

	for(i=0; i<MAX_TO; i++) {
		if(timeouts[i] == counter) {
			*timeouts[i] = value;
			timeouts[i] = (unsigned long*)counter;
			return(1);
		}
	}
	
	for(i=0; i<MAX_TO; i++) {
		if(timeouts[i] == 0) {
			timeouts[i] = (unsigned long*)counter;
			*timeouts[i] = value;
			return(1);
		}
	}
	
	*counter = 0;
	return(0);
}

unsigned char removeTimeout(volatile unsigned long* counter) {

	int i;

	for(i=0; i<MAX_TO; i++) {
		if(timeouts[i] == counter) {
			timeouts[i] = 0;
			return(1);
		}
	}
	
	return(0);
}
