#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define INC_BUTTON PC1
#define DEC_BUTTON PC0
#define LED PB0
#define DEBOUNCE 50

volatile uint16_t blinkInterval = 500; // default time blinkInterval

ISR(PCINT1_vect)
{
  _delay_ms(DEBOUNCE);
  
  if(!(PINC & (1 << INC_BUTTON)))
  {
    // increase time blinkInterval by 100ms
    blinkInterval += 100;
    if (blinkInterval > 2000) blinkInterval = 3000;
  }
  
  if(!(PINC & (1 << DEC_BUTTON)))
  {
    // decrease time blinkInterval by 100ms
    blinkInterval -= 100;
    if (blinkInterval < 100) blinkInterval = 100;
  }
}

void setup(){
    // set PB0 as output
    DDRB |= (1 << LED);
    //setup PC1 and PC0 as input
    DDRC &= ~(1 << INC_BUTTON) & ~(1 << DEC_BUTTON);

    // enable PC0 and PC1 pin change interrupts
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
  
    // enable interrupts
    sei();

}

int main(void)
{
  setup();

    while(1){
      // set PB5 high
      PORTB |= (1 << LED);
      _delay_ms(blinkInterval);

      // set PB5 low
      PORTB &= ~(1 << LED);
      _delay_ms(blinkInterval);
    }
  return 0;
}
