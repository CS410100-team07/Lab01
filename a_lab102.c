#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define INC_BUTTON PC1
#define DEC_BUTTON PC0

#define DEBOUNCE_DELAY_MS 50 // debounce delay in milliseconds
volatile uint8_t count = 0;

ISR(PCINT1_vect)
{
  static uint8_t inc_button_pressed = 0;
  static uint8_t dec_button_pressed = 0;

  // Check for increment button press
  if (!(PINC & (1 << INC_BUTTON))) {
    if (!inc_button_pressed) { // debounce increment button
      _delay_ms(DEBOUNCE_DELAY_MS);
      if (!(PINC & (1 << INC_BUTTON))) {
        count = (count + 1) % 256;
        inc_button_pressed = 1;
      }
    }
  } else {
    inc_button_pressed = 0;
  }

  // Check for decrement button press
  if (!(PINC & (1 << DEC_BUTTON))) {
    if (!dec_button_pressed) { // debounce decrement button
      _delay_ms(DEBOUNCE_DELAY_MS);
      if (!(PINC & (1 << DEC_BUTTON))) {
        count = (count - 1) % 256;
        dec_button_pressed = 1;
      }
    }
  } else {
    dec_button_pressed = 0;
  }

  // Display the count value on the LEDs
  PORTB = ((count & 0xF8) >> 3) ;
  PORTD = ((count & 0x07) << 5);  // PB0~PB5 indicate lower 6 bits

}

void setup(){

    //setup PC1 and PC0 as input
    DDRC &= ~(1 << INC_BUTTON) & ~(1 << DEC_BUTTON);
    PORTC |= (1 << INC_BUTTON) | (1 << DEC_BUTTON);
    // enable PC1 and PC1 pin change interrupts
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);

    // Set up LED pins as outputs
    DDRB = 0x1F; //PB01234 as upper bits
    DDRD = 0xE0; //PD567 as lower bits

    // Turn off all LEDs initially
    PORTB = 0x00;  
    PORTD = 0x00;

    // enable interrupts
    sei();

}

void loop()
{

}



int main(void){
    setup();

    while(1){
        loop();
    }
}