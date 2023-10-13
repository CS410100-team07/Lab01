#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define INC_BUTTON PC1
#define DEC_BUTTON PC0
#define DEBOUNCE 50
volatile uint8_t cnt = 0;

ISR(PCINT1_vect)
{

  _delay_ms(DEBOUNCE);

  // increment button
  if (!(PINC & (1 << INC_BUTTON))) 
  {
    cnt = (cnt + 1) & 0xFF;
  }

  // decrement button
  if (!(PINC & (1 << DEC_BUTTON))) 
  {
    cnt = (cnt - 1) & 0xFF;
  }


  PORTB = (cnt & 0xF8) >> 3;  //PB43210 for upper 5 bits
  PORTD = (cnt & 0x07) << 5;  //PD765 for lower 3 bits

}

void setup(){

    //setup PC1 and PC0 as input
    DDRC &= ~(1 << INC_BUTTON) & ~(1 << DEC_BUTTON);

    // enable PC1 and PC1 pin change interrupts
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);

    // Set up LED pins as outputs
    DDRB |= 0x1F; //PB01234 as upper bits
    DDRD |= 0xE0; //PD567 as lower bits

    // Turn off all LEDs initially
    PORTB &= 0x00;  
    PORTD &= 0x00;

    // enable interrupts
    sei();

}

int main(void){
    setup();

    while(1){

    }
}