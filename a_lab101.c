#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define INC_BUTTON PC1
#define DEC_BUTTON PC0
#define LED PB0

volatile uint16_t interval = 500; // default time interval

ISR(PCINT1_vect)
{
  // debounce
  _delay_ms(50);
  
  // check if PD6 is low (falling edge)
  if(!(PINC & (1 << INC_BUTTON)))
  {
    // increase time interval by 100ms
    interval += 100;
    if (interval > 2000) interval = 3000;
  }
  
  // check if PD7 is low (falling edge)
  if(!(PINC & (1 << DEC_BUTTON)))
  {
    // decrease time interval by 100ms
    interval -= 100;
    if (interval < 100) interval = 100;
  }
}

void setup(){
    // set PB5 as output
    DDRB |= (1 << LED);
    //setup PC1 and PC0 as input
    DDRC &= ~(1 << INC_BUTTON) & ~(1 << DEC_BUTTON);
    // enable PC1 and PC1 pin change interrupts
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
  
    // enable interrupts
    sei();

}




int main(void){
    setup();

    while(1){
        // set PB5 high
    PORTB |= (1 << LED);
    _delay_ms(interval);

    // set PB5 low
    PORTB &= ~(1 << LED);
    _delay_ms(interval);
    }
}