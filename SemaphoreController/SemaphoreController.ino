//Includes
#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPTPIN PCINT1       //this is PB1 per the schematic
/*
 * Alias for the ISR: "PCINT_VECTOR" (Note: There is only one PCINT ISR, 
 * PCINT0 in the name maybe be misleading, hence the Alias, this how the 
 * datasheet refers to it)
 */
#define PCINT_VECTOR PCINT0_vect
#define DATADIRECTIONPIN DDB1     //Page 64 of data sheet
#define PORTPIN PB1               //Page 64 of data sheet
#define READPIN PINB1             //Page 64 of data sheet

#define RED_PIN PB4
#define YELLOW_PIN PB3
#define GREEN_PIN PB0

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))  //OR
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) //AND

static volatile byte LEDState = 0; //variable used within ISR must be declared Volatile.

void setup() {
    cli(); //disable interrupts during setup
    
    pinMode(RED_PIN, OUTPUT);
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    
    LEDState = 0;
    
    PCMSK |= (1 << INTERRUPTPIN);   //sbi(PCMSK,INTERRUPTPIN) also works; tell pin change mask to listen to pin2/pb3 //SBI
    GIMSK |= (1 << PCIE);           //enable PCINT interrupt in the general interrupt mask //SBI

    DDRB &= ~(1 << DATADIRECTIONPIN); //cbi(DDRB, DATADIRECTIONPIN); set up as input; pin2 clear bit; set to zero
    PORTB |= (1<< PORTPIN);           //cbi(PORTB, PORTPIN); disable pull-up; hook up pulldown resistor; set to zero
    sei();                            //enable interrupts after setup

}

void loop() {
    switch(LEDState) {
      case 0:
      //Off
        digitalWrite(RED_PIN,LOW);
        digitalWrite(YELLOW_PIN,LOW);
        digitalWrite(GREEN_PIN,LOW);
        break;
      case 1:
      //Red
        digitalWrite(RED_PIN,HIGH);
        digitalWrite(YELLOW_PIN,LOW);
        digitalWrite(GREEN_PIN,LOW);
        break;
      case 2:
      //Yellow
        digitalWrite(RED_PIN,LOW);
        digitalWrite(YELLOW_PIN,HIGH);
        digitalWrite(GREEN_PIN,LOW);
        break;
      case 3:
      //Green
        digitalWrite(RED_PIN,LOW);
        digitalWrite(YELLOW_PIN,LOW);
        digitalWrite(GREEN_PIN,HIGH);
        break;
      default:
        break;
    }
}


ISR(PCINT_VECTOR)
{
  //Since the PCINTn triggers on both rising and falling edge let's just looks for rising edge
  //i.e. pin goes to 5v
  
  byte pinState;
  pinState = (PINB >> READPIN)& 1;  //PINB is the register to read the state of the pins
  if (pinState > 0) {               //look at the pin state on the pin PINB register- returns 1 if high
    LEDState = LEDState + 1;
  }

  if (LEDState > 3) {
    LEDState = 0;
  }
}
