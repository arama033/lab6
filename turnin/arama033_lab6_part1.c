/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Begin, Init, Move} state;
volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M=1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn()
{
TCCR1B=0x0B;

OCR1A=125;
TIMSK1=0x02;
TCNT1=0;
_avr_timer_cntcurr = _avr_timer_M;
SREG |= 0x80;
}

void TimerOff(){
TCCR1B=0x00;	
}

void TimerISR()
{
TimerFlag=1;

}


ISR(TIMER1_COMPA_vect)
{
_avr_timer_cntcurr--;
if(_avr_timer_cntcurr==0){
	TimerISR();
	_avr_timer_cntcurr=_avr_timer_M;
}

}


void TimerSet(unsigned long M)
{
_avr_timer_M=M;
_avr_timer_cntcurr=_avr_timer_M;
}


void Tick()
{
switch(state){
	case Begin:
	state = Init;
	break;
	
	case Init:
	state = Move;
	break;

	case Move:
	state = Move;
	break;

	default:
	state=Begin;
	break;

}

	switch(state)
{
	case Begin:
	break;
	
	case Init:
	PORTB=0x01;
	break;

	case Move:

	

	if(PORTB==0x04)
	{
	PORTB=0x01;
	}
	else
	{
	PORTB=PORTB<<1;	
	}
	break;

	default:
	break;

}

}
void main() {
    /* Insert DDR and PORT initializations */
DDRB=0xFF;
PORTB=0x00;
TimerSet(1000);
TimerOn();

    while (1) {
Tick();
while(!TimerFlag);
TimerFlag=0;


    }
    
}
