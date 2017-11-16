// **** INCLUDES *****
#include "LowPower.h"

// Use pin 2 as wake up pin
const int wakeUpPin = 2;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup()
{
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);

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
    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp, LOW);

    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER);

    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0);

    // Do something here
    // Example: Read sensor, data logging, data transmission.
}
