#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Begin, Init, L1, L2, L3, Hold} state;
unsigned char dflag = 0;
unsigned char Flag1 = 0;
unsigned char Flag2 = 0;
unsigned char Flag3 = 0;
unsigned char issue1flag = 0;
unsigned char issue3flag = 0;



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
  state = L1;
  break;

case L1:
{
  issue1flag=1;
if((~PINA & 0x01) == 0x01)
  {
  dflag=0;
  Flag1 = 1;  
  state=Hold; 
  }
  else
  {
    dflag=0;
    state = L2;
  }

 } 
 break;

  case L2:
    issue1flag=0;
    issue3flag=0;

  if((~PINA & 0x01) == 0x01)
  {
    Flag2 = 1;
  state=Hold; 
  }
  else
  {
if(dflag == 0 )
{
state = L3;
}
if(dflag == 1)
{
  state = L1;
}
  }
  break;

case L3:
  issue3flag=1;
  if((~PINA & 0x01) == 0x01)
  {
    Flag3=1;
    dflag = 1;
  state=Hold; 
  }
  else
  {
  dflag = 1;
  state = L2;
 
  }
  break;

  


  case Hold:
 if((~PINA & 0x01) == 0x01)
 {
 state = Hold;
 }
 else
 {
   
if(Flag1==1)
{
  Flag1 = 0;
state = L2;
}

if(Flag2==1)
{
  Flag2=0;
if(dflag == 0 )
{
state = L3;
}
if(dflag == 1)
{
  state = L1;
}

}

if(Flag3==1)
{
  Flag3=0;

state = L2 ;

}

 }
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

  case L1:
  if(issue3flag==0)
  {
  PORTB=0x01;
  }
  break;

  case L2:
  PORTB=0x02;
  break;

  case L3:
  if(issue1flag==0)
  {
  PORTB=0x04;
  }
  break;

  
case Hold:
break;

default:
break;

}

}

void main() {
    /* Insert DDR and PORT initializations */
DDRB=0xFF;
PORTB=0x00;
DDRA=0x00;
PORTA=0xFF;
TimerSet(300);
TimerOn();

    while (1) {
Tick();
while(!TimerFlag);
TimerFlag=0;


    }
    
}
