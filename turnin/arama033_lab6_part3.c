#include <avr/io.h>
//DEMO LINK
//https://youtu.be/HRbvNKsdvFk
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum States {Start, Reset, Plus, Minus, Wait} state;
unsigned char buttoninput;
unsigned char buttheld;

volatile unsigned char TimerFlag = 0;
void TimerISR() { TimerFlag = 1;}
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}
void TimerOff() {
  TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
  _avr_timer_cntcurr--;
  if (_avr_timer_cntcurr == 0) {
    TimerISR();
    _avr_timer_cntcurr = _avr_timer_M;
  }
}
void TimerSet (unsigned long M) {
  _avr_timer_M = M;
  _avr_timer_cntcurr = _avr_timer_M;
}

void Tick() {
  switch(state) {
    case Start:
    if(buttoninput==0x01)
    {
      if(PORTB < 9)
      {
        PORTB++;
      }
    state = Plus;

    }
    if(buttoninput==0x02)
    {
    if(PORTB > 0)
      {
        PORTB--;
      }
    state = Minus;

    }
    if(buttoninput==0x03)
    {
      state = Reset;
    }

    break;



    case Reset:
  if(buttoninput==0x01)
    {
      if(PORTB < 9)
      {
        PORTB++;
      }
    state = Plus;

    }
    if(buttoninput==0x02)
    {
    if(PORTB > 0)
      {
        PORTB--;
      }
    state = Minus;

    }
    else if(buttoninput==0x03)
    {
      state = Reset;
    }
    else
    {
      state = Start;
    }
    break;


    case Plus:
  if(buttoninput==0x02)
  {
    if(PORTB > 0)
      {
        PORTB--;
      }
    buttheld=0;
    state = Minus;

  }
  else if (buttoninput==0x03)
  {
    state = Reset;
    buttheld=0;
  }
  else if(buttoninput==0x00)
  {
  state = Wait;
  }
  else if((buttoninput==0x01)&&(buttheld==10))
  {
    if(PORTB < 9)
      {
        PORTB++;
      }
    state = Plus;
    buttheld=0;

  }
  else
  buttheld++;

    break;


    case Minus:
    if(buttoninput==0x01)
  {
    if(PORTB < 9)
      {
        PORTB++;
      }
    buttheld=0;
    state = Plus;

  }
  else if (buttoninput==0x03)
  {
    state = Reset;
    buttheld=0;
  }
  else if(buttoninput==0x00)
  {
  state = Wait;
  }
  else if((buttoninput==0x02)&&(buttheld==10))
  {
    if(PORTB > 0)
      {
        PORTB--;
      }
    state = Minus;
    buttheld=0;

  }
  else
  buttheld++;
    break;


    case Wait:
    if(buttoninput==0x01)
    {
      if(PORTB < 9)
      {
        PORTB++;
      }
    state = Plus;

    }
    if(buttoninput==0x02)
    {
    if(PORTB > 0)
      {
        PORTB--;
      }
    state = Minus;

    }
    else if(buttoninput==0x03)
    {
      state = Reset;
    }
    else
    {

      state = Wait;
    }
    break;

    default:
    break;
      
}


switch(state) {
    case Start:
    PORTB = 0x07;
    break;
    case Reset:
    PORTB = 0x00;
    break;
    case Plus:
    break;
    case Minus:
    break;
    case Wait:
    break;

    default:
    break;
}

}

void main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(100);
    TimerOn();
    state = Start;
    while (1) {
buttoninput = ~PINA & 0x03;

  Tick();
  while(!TimerFlag) {};
  TimerFlag = 0;
    }
}
