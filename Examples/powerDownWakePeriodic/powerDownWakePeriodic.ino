// **** INCLUDES *****
#include "LowPower.h"

void setup()
{
    // No setup is required for this library

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
    // Enter power down state for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S);

    // Do something here
    // Example: Read sensor, data logging, data transmission.
}
