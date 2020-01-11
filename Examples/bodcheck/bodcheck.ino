/* This program checks whether BOD can be disabled by software when sleeping.
 *
 * The output is via Serial (9600 baud), when a serial port is defined.
 * In addition the builtin LED (or D4 if there is no builtin) is permanently switched on
 * if BOD disable is possible. 
 */

#include <avr/sleep.h>
#include <Arduino.h>

#ifdef LEDBUILTIN
#define LED LED_BUILTIN
#else
#define LED 4
#endif


#if defined(BODS) && defined(BODSE)
bool bodcheck(void) {
  uint8_t res;
  asm volatile("in %[res], %[mcucr] \n\t" 
	       "ori %[res], %[bods_bodse] \n\t" 
	       "out %[mcucr], %[res] \n\t"
	       "andi %[res], %[not_bodse] \n\t" 
	       "out %[mcucr], %[res] \n\t"
	       "in %[res], %[mcucr] \n\t"
	       "andi %[res], %[bods]  \n\t"
	       : [res] "+r" (res)
	       : [mcucr] "I" _SFR_IO_ADDR(MCUCR),
		 [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)),
		 [not_bodse] "i" (~_BV(BODSE)),
		 [bods] "i" (_BV(BODS)));
    return (res != 0);	       
}
#else
#warning "This MCU type does not support software disabling of BOD"
#endif

void setup (void) {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(500);

#if defined(SERIAL_PORT_MONITOR)
  Serial.begin(9600);
#if defined(BODS) && defined(BODSE)
  if (bodcheck())
    Serial.println("\nBOD can be disabled by software on this MCU");
  else
    Serial.println("\nBOD CANNOT be disabled by software on this MCU");
#else
  Serial.println("\nBOD CANNOT be disabled by software on this MCU TYPE");
#endif
#endif

#if defined(BODS) && defined(BODSE)
  if  (bodcheck()) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);
#else
  digitalWrite(LED, LOW);
#endif
  while (1);
}

void loop (void) {
}
