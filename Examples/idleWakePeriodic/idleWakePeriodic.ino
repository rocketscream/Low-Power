// **** INCLUDES *****
#include "LowPower.h"

void setup()
{
  // Setup the peripherals to be turned off while in sleep
  // Each microcontroller comes with different number of peripherals

  // ATmega328P, ATmega168
  LowPower.setup(ADC_SWITCH, TIMER2_SWITCH, TIMER1_SWITCH, TIMER0_SWITCH,
                SPI_SWITCH, USART0_SWITCH, TWI_SWITCH);

  // ATmega32U4
  //LowPower.setup(ADC_SWITCH, TIMER4_SWITCH, TIMER3_SWITCH, TIMER1_SWITCH,
  //		  TIMER0_SWITCH, SPI_SWITCH, USART1_SWITCH, TWI_SWITCH, USB_SWITCH);

  // ATmega2560
  //LowPower.setup(ADC_SWITCH, TIMER5_SWITCH, TIMER4_SWITCH, TIMER3_SWITCH,
  //		  TIMER2_SWITCH, TIMER1_SWITCH, TIMER0_SWITCH, SPI_SWITCH, USART3_SWITCH,
  //		  USART2_SWITCH, USART1_SWITCH, USART0_SWITCH, TWI_SWITCH);

  // ATmega256RFR2
  //LowPower.setup(ADC_SWITCH, TIMER5_SWITCH, TIMER4_SWITCH, TIMER3_SWITCH,
  //      TIMER2_SWITCH, TIMER1_SWITCH, TIMER0_SWITCH, SPI_SWITCH,
  //      USART1_SWITCH, USART0_SWITCH, TWI_SWITCH);

}

void loop()
{
  // Enter idle state for 8 s with the rest of peripherals turned off
  // Each microcontroller comes with different number of peripherals

  LowPower.idle(SLEEP_8S);

  // Do something here
  // Example: Read sensor, data logging, data transmission.
}
