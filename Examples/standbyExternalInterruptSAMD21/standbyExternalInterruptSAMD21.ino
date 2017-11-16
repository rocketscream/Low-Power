// **** INCLUDES *****
#include "LowPower.h"

// External interrupt on pin 0 (use pin 0 to 24, except pin 4 on Arduino Zero)
const int pin = 0;
unsigned char count = 10;

void setup()
{
	// Wait for serial USB port to open
	while(!SerialUSB);
	SerialUSB.println("***** ATSAMD21 Standby Mode Example *****");

	// ***** IMPORTANT *****
	// Delay is required to allow the USB interface to be active during
	// sketch upload process
	SerialUSB.println("Entering standby mode in:");
	for (count; count > 0; count--)
	{
	  SerialUSB.print(count);
	  SerialUSB.println(" s");
	  delay(1000);
  }
  // *********************

  // External interrupt on pin (example: press of an active low button)
  // A pullup resistor is used to hold the signal high when no button press
  attachInterrupt(pin, blink, LOW);

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
	SerialUSB.println("Entering standby mode.");
	SerialUSB.println("Apply low signal to wake the processor.");
	SerialUSB.println("Zzzz...");
	// Detach USB interface
	USBDevice.detach();
  // Enter standby mode
  LowPower.standby();
  // Attach USB interface
  USBDevice.attach();
  // Wait for serial USB port to open
  while(!SerialUSB);
  // Serial USB is blazing fast, you might miss the messages
  delay(1000);
  SerialUSB.println("Awake!");
  SerialUSB.println("Send any character to enter standby mode again");
  // Wait for user response
  while(!SerialUSB.available());
  while(SerialUSB.available() > 0)
  {
		SerialUSB.read();
	}
}

void blink(void)
{

}
