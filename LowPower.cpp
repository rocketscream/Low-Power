/*******************************************************************************
* LowPower Library
* Version: 1.70
* Date: 16-11-2017
* Author: Lim Phang Moh
* Company: Rocket Scream Electronics
* Website: www.rocketscream.com
*
* This is a lightweight low power library for Arduino.
*
* This library is licensed under Creative Commons Attribution-ShareAlike 3.0
* Unported License.
*
* Revision	Description
* ========	===========
* 1.70		Addded support for ALWAYSON or ALWAYSOFF settings.
* 1.60		Added support for ATmega256RFR2. Contributed by Rodmg.
* 1.50		Fixed compiler optimization (Arduino IDE 1.6.x branch) on BOD enable
*			function that causes the function to be over optimized.
* 1.40		Added support for ATSAMD21G18A.
* 		Library format compliant with Arduino IDE 1.5.x.
* 1.30		Added support for ATMega168, ATMega2560, ATMega1280 & ATMega32U4.
*			Tested to work with Arduino IDE 1.0.1 - 1.0.4.
* 1.20		Remove typo error in idle method for checking whether Timer 0 was
*			turned off.
*			Remove dependecy on WProgram.h which is not required.
*			Tested to work with Arduino IDE 1.0.
* 1.10		Added #ifndef for sleep_bod_disable() for compatibility with future
*			Arduino IDE release.
* 1.00		Initial public release.
*******************************************************************************/
#if defined (__AVR__)
	#include <avr/sleep.h>
	#include <avr/wdt.h>
	#include <avr/power.h>
	#include <avr/interrupt.h>
#elif defined (__arm__)

#else
	#error "Processor architecture is not supported."
#endif

#include "LowPower.h"

#if defined (__AVR__)
// Only Pico Power devices can change BOD settings through software
#if defined __AVR_ATmega328P__
#ifndef sleep_bod_disable
#define sleep_bod_disable() 										\
do { 																\
	unsigned char tempreg; 													\
	__asm__ __volatile__("in %[tempreg], %[mcucr]" "\n\t" 			\
											 "ori %[tempreg], %[bods_bodse]" "\n\t" 		\
											 "out %[mcucr], %[tempreg]" "\n\t" 			\
											 "andi %[tempreg], %[not_bodse]" "\n\t" 		\
											 "out %[mcucr], %[tempreg]" 					\
											 : [tempreg] "=&d" (tempreg) 					\
											 : [mcucr] "I" _SFR_IO_ADDR(MCUCR), 			\
												 [bods_bodse] "i" (_BV(BODS) | _BV(BODSE)), \
												 [not_bodse] "i" (~_BV(BODSE))); 			\
} while (0)
#endif
#endif

// Some macros is still missing from AVR GCC distribution for ATmega32U4
#if defined __AVR_ATmega32U4__
	// Timer 4 PRR bit is currently not defined in iom32u4.h
	#ifndef PRTIM4
		#define PRTIM4 4
	#endif

	// Timer 4 power reduction macro is not defined currently in power.h
	#ifndef power_timer4_disable
		#define power_timer4_disable()	(PRR1 |= (uint8_t)(1 << PRTIM4))
	#endif

	#ifndef power_timer4_enable
		#define power_timer4_enable()		(PRR1 &= (uint8_t)~(1 << PRTIM4))
	#endif
#endif



#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__)
void LowPowerClass::switch_bod(bool on)
{
	if (on)
	{
	}
	else
	{
		sleep_bod_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_adc(bool on)
{
	if (on)
	{
		power_adc_enable();
		ADCSRA |= (1 << ADEN);
	}
	else
	{
		ADCSRA &= ~(1 << ADEN);
		power_adc_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
void LowPowerClass::switch_timer5(bool on)
{
	if (on)
	{
		power_timer5_enable();
	}
	else
	{
		power_timer5_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
void LowPowerClass::switch_timer4(bool on)
{
	if (on)
	{
		power_timer4_enable();
	}
	else
	{
		power_timer4_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
void LowPowerClass::switch_timer3(bool on)
{
	if (on)
	{
		power_timer3_enable();
	}
	else
	{
		power_timer3_disable();
	}
}
#endif
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
void LowPowerClass::switch_timer2(bool on)
{
	if (on)
	{
		if (LowPower.clockSource & CS22) TCCR2B |= (1 << CS22);
		if (LowPower.clockSource & CS21) TCCR2B |= (1 << CS21);
		if (LowPower.clockSource & CS20) TCCR2B |= (1 << CS20);

		power_timer2_enable();
	}
	else
	{
		if (TCCR2B & CS22) LowPower.clockSource |= (1 << CS22);
		if (TCCR2B & CS21) LowPower.clockSource |= (1 << CS21);
		if (TCCR2B & CS20) LowPower.clockSource |= (1 << CS20);
		TCCR2B &= ~(1 << CS22);
		TCCR2B &= ~(1 << CS21);
		TCCR2B &= ~(1 << CS20);
		power_timer2_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_timer1(bool on)
{
	if (on)
	{
		power_timer1_enable();
	}
	else
	{
		power_timer1_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_timer0(bool on)
{
	if (on)
	{
		power_timer0_enable();
	}
	else
	{
		power_timer0_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_spi(bool on)
{
	if (on)
	{
		power_spi_enable();
	}
	else
	{
		power_spi_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__)
void LowPowerClass::switch_usart3(bool on)
{
	if (on)
	{
		power_usart3_enable();
	}
	else
	{
		power_usart3_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__)
void LowPowerClass::switch_usart2(bool on)
{
	if (on)
	{
		power_usart2_enable();
	}
	else
	{
		power_usart2_disable();
	}
}
#endif
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
void LowPowerClass::switch_usart1(bool on)
{
	if (on)
	{
		power_usart1_enable();
	}
	else
	{
		power_usart1_disable();
	}
}
#endif
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
void LowPowerClass::switch_usart0(bool on)
{
	if (on)
	{
		power_usart0_enable();
	}
	else
	{
		power_usart0_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_twi(bool on)
{
	if (on)
	{
		power_twi_enable();
	}
	else
	{
		power_twi_disable();
	}
}
#endif
#if defined (__AVR__)
void LowPowerClass::switch_usb(bool on)
{
	if (on)
	{
		power_usb_enable();
	}
	else
	{
		power_usb_disable();
	}
}
#endif

/*******************************************************************************
* Name: setup
\* Description: Setup ATmega328P/168. Please make sure you
*							 understand the implication and result of disabling module.
*
* Argument		Description
* =========		===========
* 1. bod		BOD module disable control:
*				(a) BOD_SWITCH - Turn off BOD module during sleep
*				(c) BOD_ALWAYSON - Always turn on BOD module
*				(d) BOD_DEFAULT - Leave BOD module in its default state
*
* 2. adc		ADC module disable control:
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
* 4. timer1		Timer 1 module disable control:
*				(a) TIMER1_SWITCH - Turn off Timer 1 module during sleep
*				(b) TIMER1_ALWAYSOFF - Always turn off Timer 1 module
*				(c) TIMER1_ALWAYSON - Always turn on Timer 1 module
*				(d) TIMER1_DEFAULT - Leave Timer 1 module in its default state
*
* 5. timer0		Timer 0 module disable control:
*				(a) TIMER0_SWITCH - Turn off Timer 0 module during sleep
*				(b) TIMER0_ALWAYSOFF - Always turn off Timer 0 module
*				(c) TIMER0_ALWAYSON - Always turn on Timer 0 module
*				(d) TIMER0_DEFAULT - Leave Timer 0 module in its default state
*
* 6. spi		SPI module disable control:
*				(a) SPI_SWITCH - Turn off SPI module during sleep
*				(b) SPI_ALWAYSOFF - Always turn off SPI module
*				(c) SPI_ALWAYSON - Always turn on SPI module
*				(d) SPI_DEFAULT - Leave SPI module in its default state
*
* 7. usart0		USART0 module disable control:
*				(a) USART0_SWITCH - Turn off USART0	module during sleep
*				(b) USART0_ALWAYSOFF - Always turn off USART0	module
*				(c) USART0_ALWAYSON - Always turn on USART0	module
*				(d) USART0_DEFAULT - Leave USART0 module in its default state
*
* 8. twi		TWI module disable control:
*				(a) TWI_SWITCH - Turn off TWI module during sleep
*				(b) TWI_ALWAYSOFF - Always turn off TWI module
*				(c) TWI_ALWAYSON - Always turn on TWI module
*				(d) TWI_DEFAULT - Leave TWI module in its default state
*
*******************************************************************************/
#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__)
void	LowPowerClass::setup(bod_t bod, adc_t adc, timer2_t timer2,
							timer1_t timer1, timer0_t timer0,
							spi_t spi, usart0_t usart0,	twi_t twi)
{
	this->bod = bod;
	this->adc = adc;
	this->timer2 = timer2;
	this->timer1 = timer1;
	this->timer0 = timer0;
	this->spi = spi;
	this->usart0 = usart0;
	this->twi = twi;
	if (LowPower.bod == BOD_ALWAYSON) LowPower.switch_bod(true);
	if (LowPower.adc == ADC_ALWAYSOFF) LowPower.switch_adc(false);
	else if (LowPower.adc == ADC_ALWAYSON) LowPower.switch_adc(true);
	if (LowPower.timer2 == TIMER2_ALWAYSOFF) LowPower.switch_timer2(false);
	else if (LowPower.timer2 == TIMER2_ALWAYSON) LowPower.switch_timer2(true);
	if (LowPower.timer1 == TIMER1_ALWAYSOFF) LowPower.switch_timer1(false);
	else if (LowPower.timer1 == TIMER1_ALWAYSON) LowPower.switch_timer1(true);
	if (LowPower.timer0 == TIMER0_ALWAYSOFF) LowPower.switch_timer0(false);
	else if (LowPower.timer0 == TIMER0_ALWAYSON) LowPower.switch_timer0(true);
	if (LowPower.spi == SPI_ALWAYSOFF) LowPower.switch_spi(false);
	else if (LowPower.spi == SPI_ALWAYSON) LowPower.switch_spi(true);
	if (LowPower.usart0 == USART0_ALWAYSOFF) LowPower.switch_usart0(false);
	else if (LowPower.usart0 == USART0_ALWAYSON) LowPower.switch_usart0(true);
	if (LowPower.twi == TWI_ALWAYSOFF) LowPower.switch_twi(false);
	else if (LowPower.twi == TWI_ALWAYSON) LowPower.switch_twi(true);
}
#endif

/*******************************************************************************
* Name: setup
\* Description: Setup ATmega32U4. Please make sure you
*				 understand the implication and result of disabling module.
*				 Take note that Timer 2 is not available and USART0 is replaced
* 				 with USART1 on ATmega32U4.
*
* Argument		Description
* =========		===========
* 1. adc		ADC module disable control:
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 2. timer4		Timer 4 module disable control:
*				(a) TIMER4_SWITCH - Turn off Timer 4 module during sleep
*				(b) TIMER4_ALWAYSOFF - Always turn off Timer 4 module
*				(c) TIMER4_ALWAYSON - Always turn on Timer 4 module
*				(d) TIMER4_DEFAULT - Leave Timer 4 module in its default state
*
* 3. timer3		Timer 3 module disable control:
*				(a) TIMER3_SWITCH - Turn off Timer 3 module during sleep
*				(b) TIMER3_ALWAYSOFF - Always turn off Timer 3 module
*				(c) TIMER3_ALWAYSON - Always turn on Timer 3 module
*				(d) TIMER3_DEFAULT - Leave Timer 3 module in its default state
*
* 4. timer1		Timer 1 module disable control:
*				(a) TIMER1_SWITCH - Turn off Timer 1 module during sleep
*				(b) TIMER1_ALWAYSOFF - Always turn off Timer 1 module
*				(c) TIMER1_ALWAYSON - Always turn on Timer 1 module
*				(d) TIMER1_DEFAULT - Leave Timer 1 module in its default state
*
* 5. timer0		Timer 0 module disable control:
*				(a) TIMER0_SWITCH - Turn off Timer 0 module during sleep
*				(b) TIMER0_ALWAYSOFF - Always turn off Timer 0 module
*				(c) TIMER0_ALWAYSON - Always turn on Timer 0 module
*				(d) TIMER0_DEFAULT - Leave Timer 0 module in its default state
*
* 6. spi		SPI module disable control:
*				(a) SPI_SWITCH - Turn off SPI module during sleep
*				(b) SPI_ALWAYSOFF - Always turn off SPI module
*				(c) SPI_ALWAYSON - Always turn on SPI module
*				(d) SPI_DEFAULT - Leave SPI module in its default state
*
* 7. usart1		USART1 module disable control:
*				(a) USART1_SWITCH - Turn off USART1	module during sleep
*				(b) USART1_ALWAYSOFF - Always turn off USART1	module
*				(c) USART1_ALWAYSON - Always turn on USART1	module
*				(d) USART1_DEFAULT - Leave USART1 module in its default state
*
* 8. twi		TWI module disable control:
*				(a) TWI_SWITCH - Turn off TWI module during sleep
*				(b) TWI_ALWAYSOFF - Always turn off TWI module
*				(c) TWI_ALWAYSON - Always turn on TWI module
*				(d) TWI_DEFAULT - Leave TWI module in its default state
*
* 9.usb		USB module disable control:
*				(a) USB_SWITCH - Turn off USB module during sleep
*				(b) USB_ALWAYSOFF - Always turn off USB module
*				(c) USB_ALWAYSON - Always turn on USB module
*				(d) USB_DEFAULT - Leave USB module in its default state
*******************************************************************************/
#if defined __AVR_ATmega32U4__
void	LowPowerClass::setup(adc_t adc,
							timer4_t timer4, timer3_t timer3,
							timer1_t timer1, timer0_t timer0,
							spi_t spi, usart1_t usart1,	twi_t twi, usb_t usb)
{
	this->adc = adc;
	this->timer4 = timer4;
	this->timer3 = timer3;
	this->timer1 = timer1;
	this->timer0 = timer0;
	this->spi = spi;
	this->usart1 = usart1;
	this->twi = twi;
	this->usb = usb;
  if (LowPower.adc == ADC_ALWAYSOFF) LowPower.switch_adc(false);
  else if (LowPower.adc == ADC_ALWAYSON) LowPower.switch_adc(true);
  if (LowPower.timer4 == TIMER4_ALWAYSOFF) LowPower.switch_timer4(false);
  else if (LowPower.timer4 == TIMER4_ALWAYSON) LowPower.switch_timer4(true);
  if (LowPower.timer3 == TIMER3_ALWAYSOFF) LowPower.switch_timer3(false);
  else if (LowPower.timer3 == TIMER3_ALWAYSON) LowPower.switch_timer3(true);
  if (LowPower.timer1 == TIMER1_ALWAYSOFF) LowPower.switch_timer1(false);
  else if (LowPower.timer1 == TIMER1_ALWAYSON) LowPower.switch_timer1(true);
  if (LowPower.timer0 == TIMER0_ALWAYSOFF) LowPower.switch_timer0(false);
  else if (LowPower.timer0 == TIMER0_ALWAYSON) LowPower.switch_timer0(true);
  if (LowPower.spi == SPI_ALWAYSOFF) LowPower.switch_spi(false);
  else if (LowPower.spi == SPI_ALWAYSON) LowPower.switch_spi(true);
  if (LowPower.usart1 == USART1_ALWAYSOFF) LowPower.switch_usart1(false);
  else if (LowPower.usart1 == USART1_ALWAYSON) LowPower.switch_usart1(true);
  if (LowPower.twi == TWI_ALWAYSOFF) LowPower.switch_twi(false);
  else if (LowPower.twi == TWI_ALWAYSON) LowPower.switch_twi(true);
  if (LowPower.usb == USB_ALWAYSOFF) LowPower.switch_usb(false);
  else if (LowPower.usb == USB_ALWAYSON) LowPower.switch_usb(true);
}
#endif

/*******************************************************************************
* Name: setup
\* Description: Setup ATmega2560 & ATmega1280. Please make sure
*				 you understand the implication and result of disabling module.
*				 Take note that extra Timer 5, 4, 3 compared to an ATmega328P/168.
*				 Also take note that extra USART 3, 2, 1 compared to an
*				 ATmega328P/168.
*
* Argument		Description
* =========		===========
* 1. adc		ADC module disable control:
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 2. timer5		Timer 5 module disable control:
*				(a) TIMER5_SWITCH - Turn off Timer 5 module during sleep
*				(b) TIMER5_ALWAYSOFF - Always turn off Timer 5 module
*				(c) TIMER5_ALWAYSON - Always turn on Timer 5 module
*				(d) TIMER5_DEFAULT - Leave Timer 5 module in its default state
*
* 3. timer4		Timer 4 module disable control:
*				(a) TIMER4_SWITCH - Turn off Timer 4 module during sleep
*				(b) TIMER4_ALWAYSOFF - Always turn off Timer 4 module
*				(c) TIMER4_ALWAYSON - Always turn on Timer 4 module
*				(d) TIMER4_DEFAULT - Leave Timer 4 module in its default state
*
* 4. timer3		Timer 3 module disable control:
*				(a) TIMER3_SWITCH - Turn off Timer 3 module during sleep
*				(b) TIMER3_ALWAYSOFF - Always turn off Timer 3 module
*				(c) TIMER3_ALWAYSON - Always turn on Timer 3 module
*				(d) TIMER3_DEFAULT - Leave Timer 3 module in its default state
*
* 5. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
* 6. timer1		Timer 1 module disable control:
*				(a) TIMER1_SWITCH - Turn off Timer 1 module during sleep
*				(b) TIMER1_ALWAYSOFF - Always turn off Timer 1 module
*				(c) TIMER1_ALWAYSON - Always turn on Timer 1 module
*				(d) TIMER1_DEFAULT - Leave Timer 1 module in its default state
*
* 7. timer0		Timer 0 module disable control:
*				(a) TIMER0_SWITCH - Turn off Timer 0 module during sleep
*				(b) TIMER0_ALWAYSOFF - Always turn off Timer 0 module
*				(c) TIMER0_ALWAYSON - Always turn on Timer 0 module
*				(d) TIMER0_DEFAULT - Leave Timer 0 module in its default state
*
* 8. spi		SPI module disable control:
*				(a) SPI_SWITCH - Turn off SPI module during sleep
*				(b) SPI_ALWAYSOFF - Always turn off SPI module
*				(c) SPI_ALWAYSON - Always turn on SPI module
*				(d) SPI_DEFAULT - Leave SPI module in its default state
*
* 9.usart3		USART3 module disable control:
*				(a) USART3_SWITCH - Turn off USART3	module during sleep
*				(b) USART3_ALWAYSOFF - Always turn off USART3	module
*				(c) USART3_ALWAYSON - Always turn on USART3	module
*				(d) USART3_DEFAULT - Leave USART3 module in its default state
*
* 10.usart2		USART2 module disable control:
*				(a) USART2_SWITCH - Turn off USART2	module during sleep
*				(b) USART2_ALWAYSOFF - Always turn off USART2	module
*				(c) USART2_ALWAYSON - Always turn on USART2	module
*				(d) USART2_DEFAULT - Leave USART2 module in its default state
*
* 11.usart1		USART1 module disable control:
*				(a) USART1_SWITCH - Turn off USART1	module during sleep
*				(b) USART1_ALWAYSOFF - Always turn off USART1	module
*				(c) USART1_ALWAYSON - Always turn on USART1	module
*				(d) USART1_DEFAULT - Leave USART1 module in its default state
*
* 12.usart0		USART0 module disable control:
*				(a) USART0_SWITCH - Turn off USART0	module during sleep
*				(b) USART0_ALWAYSOFF - Always turn off USART0	module
*				(c) USART0_ALWAYSON - Always turn on USART0	module
*				(d) USART0_DEFAULT - Leave USART0 module in its default state
*
* 13.twi		TWI module disable control:
*				(a) TWI_SWITCH - Turn off TWI module during sleep
*				(b) TWI_ALWAYSOFF - Always turn off TWI module
*				(c) TWI_ALWAYSON - Always turn on TWI module
*				(d) TWI_DEFAULT - Leave TWI module in its default state
*
*******************************************************************************/
#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega1280__)
void	LowPowerClass::setup(adc_t adc, timer5_t timer5,
									timer4_t timer4, timer3_t timer3, timer2_t timer2,
									timer1_t timer1, timer0_t timer0, spi_t spi,
									usart3_t usart3, usart2_t usart2, usart1_t usart1,
									usart0_t usart0, twi_t twi)
{
	this->adc = adc;
	this->timer5 = timer5;
	this->timer4 = timer4;
	this->timer3 = timer3;
	this->timer2 = timer2;
	this->timer1 = timer1;
	this->timer0 = timer0;
	this->spi = spi;
	this->usart3 = usart3;
	this->usart2 = usart2;
	this->usart1 = usart1;
	this->usart0 = usart0;
	this->twi = twi;
	if (LowPower.adc == ADC_ALWAYSOFF) LowPower.switch_adc(false);
	else if (LowPower.adc == ADC_ALWAYSON) LowPower.switch_adc(true);
	if (LowPower.timer5 == TIMER5_ALWAYSOFF) LowPower.switch_timer5(false);
	else if (LowPower.timer5 == TIMER5_ALWAYSON) LowPower.switch_timer5(true);
	if (LowPower.timer4 == TIMER4_ALWAYSOFF) LowPower.switch_timer4(false);
	else if (LowPower.timer4 == TIMER4_ALWAYSON) LowPower.switch_timer4(true);
	if (LowPower.timer3 == TIMER3_ALWAYSOFF) LowPower.switch_timer3(false);
	else if (LowPower.timer3 == TIMER3_ALWAYSON) LowPower.switch_timer3(true);
	if (LowPower.timer2 == TIMER2_ALWAYSOFF) LowPower.switch_timer2(false);
	else if (LowPower.timer2 == TIMER2_ALWAYSON) LowPower.switch_timer2(true);
	if (LowPower.timer1 == TIMER1_ALWAYSOFF) LowPower.switch_timer1(false);
	else if (LowPower.timer1 == TIMER1_ALWAYSON) LowPower.switch_timer1(true);
	if (LowPower.timer0 == TIMER0_ALWAYSOFF) LowPower.switch_timer0(false);
	else if (LowPower.timer0 == TIMER0_ALWAYSON) LowPower.switch_timer0(true);
	if (LowPower.spi == SPI_ALWAYSOFF) LowPower.switch_spi(false);
	else if (LowPower.spi == SPI_ALWAYSON) LowPower.switch_spi(true);
	if (LowPower.usart3 == USART3_ALWAYSOFF) LowPower.switch_usart3(false);
	else if (LowPower.usart3 == USART3_ALWAYSON) LowPower.switch_usart3(true);
	if (LowPower.usart2 == USART2_ALWAYSOFF) LowPower.switch_usart2(false);
	else if (LowPower.usart2 == USART2_ALWAYSON) LowPower.switch_usart2(true);
	if (LowPower.usart1 == USART1_ALWAYSOFF) LowPower.switch_usart1(false);
	else if (LowPower.usart1 == USART1_ALWAYSON) LowPower.switch_usart1(true);
	if (LowPower.usart0 == USART0_ALWAYSOFF) LowPower.switch_usart0(false);
	else if (LowPower.usart0 == USART0_ALWAYSON) LowPower.switch_usart0(true);
	if (LowPower.twi == TWI_ALWAYSOFF) LowPower.switch_twi(false);
	else if (LowPower.twi == TWI_ALWAYSON) LowPower.switch_twi(true);
}
#endif

/*******************************************************************************
* Name: setup
\* Description: Setup ATmega256RFR2. Please make sure
*				 you understand the implication and result of disabling module.
*				 Take note that extra Timer 5, 4, 3 compared to an ATmega328P/168.
*				 Also take note that extra USART 1 compared to an
*				 ATmega328P/168.
*
* Argument		Description
* =========		===========
* 1. adc		ADC module disable control:
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 2. timer5		Timer 5 module disable control:
*				(a) TIMER5_SWITCH - Turn off Timer 5 module during sleep
*				(b) TIMER5_ALWAYSOFF - Always turn off Timer 5 module
*				(c) TIMER5_ALWAYSON - Always turn on Timer 5 module
*				(d) TIMER5_DEFAULT - Leave Timer 5 module in its default state
*
* 3. timer4		Timer 4 module disable control:
*				(a) TIMER4_SWITCH - Turn off Timer 4 module during sleep
*				(b) TIMER4_ALWAYSOFF - Always turn off Timer 4 module
*				(c) TIMER4_ALWAYSON - Always turn on Timer 4 module
*				(d) TIMER4_DEFAULT - Leave Timer 4 module in its default state
*
* 4. timer3		Timer 3 module disable control:
*				(a) TIMER3_SWITCH - Turn off Timer 3 module during sleep
*				(b) TIMER3_ALWAYSOFF - Always turn off Timer 3 module
*				(c) TIMER3_ALWAYSON - Always turn on Timer 3 module
*				(d) TIMER3_DEFAULT - Leave Timer 3 module in its default state
*
* 5. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
* 6. timer1		Timer 1 module disable control:
*				(a) TIMER1_SWITCH - Turn off Timer 1 module during sleep
*				(b) TIMER1_ALWAYSOFF - Always turn off Timer 1 module
*				(c) TIMER1_ALWAYSON - Always turn on Timer 1 module
*				(d) TIMER1_DEFAULT - Leave Timer 1 module in its default state
*
* 7. timer0		Timer 0 module disable control:
*				(a) TIMER0_SWITCH - Turn off Timer 0 module during sleep
*				(b) TIMER0_ALWAYSOFF - Always turn off Timer 0 module
*				(c) TIMER0_ALWAYSON - Always turn on Timer 0 module
*				(d) TIMER0_DEFAULT - Leave Timer 0 module in its default state
*
* 8. spi		SPI module disable control:
*				(a) SPI_SWITCH - Turn off SPI module during sleep
*				(b) SPI_ALWAYSOFF - Always turn off SPI module
*				(c) SPI_ALWAYSON - Always turn on SPI module
*				(d) SPI_DEFAULT - Leave SPI module in its default state
*
* 9.usart1		USART1 module disable control:
*				(a) USART1_SWITCH - Turn off USART1	module during sleep
*				(b) USART1_ALWAYSOFF - Always turn off USART1	module
*				(c) USART1_ALWAYSON - Always turn on USART1	module
*				(d) USART1_DEFAULT - Leave USART1 module in its default state
*
* 10.usart0		USART0 module disable control:
*				(a) USART0_SWITCH - Turn off USART0	module during sleep
*				(b) USART0_ALWAYSOFF - Always turn off USART0	module
*				(c) USART0_ALWAYSON - Always turn on USART0	module
*				(d) USART0_DEFAULT - Leave USART0 module in its default state
*
* 11.twi		TWI module disable control:
*				(a) TWI_SWITCH - Turn off TWI module during sleep
*				(b) TWI_ALWAYSOFF - Always turn off TWI module
*				(c) TWI_ALWAYSON - Always turn on TWI module
*				(d) TWI_DEFAULT - Leave TWI module in its default state
*
*******************************************************************************/
#if defined (__AVR_ATmega256RFR2__)
void	LowPowerClass::setup(adc_t adc, timer5_t timer5,
													timer4_t timer4, timer3_t timer3, timer2_t timer2,
													timer1_t timer1, timer0_t timer0, spi_t spi,
													usart1_t usart1,
													usart0_t usart0, twi_t twi)
{
	this->adc = adc;
	this->timer5 = timer5;
	this->timer4 = timer4;
	this->timer3 = timer3;
	this->timer2 = timer2;
	this->timer1 = timer1;
	this->timer0 = timer0;
	this->spi = spi;
	this->usart1 = usart1;
	this->usart0 = usart0;
	this->twi = twi;
	if (LowPower.adc == ADC_ALWAYSOFF) LowPower.switch_adc(false);
	else if (LowPower.adc == ADC_ALWAYSON) LowPower.switch_adc(true);
	if (LowPower.timer5 == TIMER5_ALWAYSOFF) LowPower.switch_timer5(false);
	else if (LowPower.timer5 == TIMER5_ALWAYSON) LowPower.switch_timer5(true);
	if (LowPower.timer4 == TIMER4_ALWAYSOFF) LowPower.switch_timer4(false);
	else if (LowPower.timer4 == TIMER4_ALWAYSON) LowPower.switch_timer4(true);
	if (LowPower.timer3 == TIMER3_ALWAYSOFF) LowPower.switch_timer3(false);
	else if (LowPower.timer3 == TIMER3_ALWAYSON) LowPower.switch_timer3(true);
	if (LowPower.timer2 == TIMER2_ALWAYSOFF) LowPower.switch_timer2(false);
	else if (LowPower.timer2 == TIMER2_ALWAYSON) LowPower.switch_timer2(true);
	if (LowPower.timer1 == TIMER1_ALWAYSOFF) LowPower.switch_timer1(false);
	else if (LowPower.timer1 == TIMER1_ALWAYSON) LowPower.switch_timer1(true);
	if (LowPower.timer0 == TIMER0_ALWAYSOFF) LowPower.switch_timer0(false);
	else if (LowPower.timer0 == TIMER0_ALWAYSON) LowPower.switch_timer0(true);
	if (LowPower.spi == SPI_ALWAYSOFF) LowPower.switch_spi(false);
	else if (LowPower.spi == SPI_ALWAYSON) LowPower.switch_spi(true);
	if (LowPower.usart1 == USART1_ALWAYSOFF) LowPower.switch_usart1(false);
	else if (LowPower.usart1 == USART1_ALWAYSON) LowPower.switch_usart1(true);
	if (LowPower.usart0 == USART0_ALWAYSOFF) LowPower.switch_usart0(false);
	else if (LowPower.usart0 == USART0_ALWAYSON) LowPower.switch_usart0(true);
	if (LowPower.twi == TWI_ALWAYSOFF) LowPower.switch_twi(false);
	else if (LowPower.twi == TWI_ALWAYSON) LowPower.switch_twi(true);
}
#endif


/*******************************************************************************
* Name: idle
* Description: Putting microcontroller into idle state. Please make sure you
*							 understand the implication and result of disabling module.
*
* Argument		Description
* =========		===========
* 1. period	 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
*******************************************************************************/
void	LowPowerClass::idle(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer5 == TIMER5_SWITCH) LowPower.switch_timer5(false);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.timer4 == TIMER4_SWITCH) LowPower.switch_timer4(false);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.timer3 == TIMER3_SWITCH) LowPower.switch_timer3(false);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(false);
	#endif
	#if defined (__AVR__)
	if (LowPower.timer1 == TIMER1_SWITCH) LowPower.switch_timer1(false);
	#endif
	#if defined (__AVR__)
	if (LowPower.timer0 == TIMER0_SWITCH) LowPower.switch_timer0(false);
	#endif
	#if defined (__AVR__)
	if (LowPower.spi == SPI_SWITCH) LowPower.switch_spi(false);
	#endif
	#if defined (__AVR_ATmega2560__)
	if (LowPower.usart3 == USART3_SWITCH) LowPower.switch_usart3(false);
	#endif
	#if defined (__AVR_ATmega2560__)
	if (LowPower.usart2 == USART2_SWITCH) LowPower.switch_usart2(false);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.usart1 == USART1_SWITCH) LowPower.switch_usart1(false);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.usart0 == USART0_SWITCH) LowPower.switch_usart0(false);
	#endif
	#if defined (__AVR__)
	if (LowPower.twi == TWI_SWITCH) LowPower.switch_twi(false);
	#endif
	#if defined (__AVR__)
	if (LowPower.usb == USB_SWITCH) LowPower.switch_usb(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_IDLE);
	cli();
	sleep_enable();
	#if defined (__AVR_ATmega328P__)
	if (LowPower.bod == BOD_SWITCH) LowPower.switch_bod(false);
	#endif
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer5 == TIMER5_SWITCH) LowPower.switch_timer5(true);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.timer4 == TIMER4_SWITCH) LowPower.switch_timer4(true);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.timer3 == TIMER3_SWITCH) LowPower.switch_timer3(true);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(true);
	#endif
	#if defined (__AVR__)
	if (LowPower.timer1 == TIMER1_SWITCH) LowPower.switch_timer1(true);
	#endif
	#if defined (__AVR__)
	if (LowPower.timer0 == TIMER0_SWITCH) LowPower.switch_timer0(true);
	#endif
	#if defined (__AVR__)
	if (LowPower.spi == SPI_SWITCH) LowPower.switch_spi(true);
	#endif
	#if defined (__AVR_ATmega2560__)
	if (LowPower.usart3 == USART3_SWITCH) LowPower.switch_usart3(true);
	#endif
	#if defined (__AVR_ATmega2560__)
	if (LowPower.usart2 == USART2_SWITCH) LowPower.switch_usart2(true);
	#endif
	#if defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__) || defined (__AVR_ATmega32U4__)
	if (LowPower.usart1 == USART1_SWITCH) LowPower.switch_usart1(true);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.usart0 == USART0_SWITCH) LowPower.switch_usart0(true);
	#endif
	#if defined (__AVR__)
	if (LowPower.twi == TWI_SWITCH) LowPower.switch_twi(true);
	#endif
	#if defined (__AVR__)
	if (LowPower.usb == USB_SWITCH) LowPower.switch_usb(true);
	#endif
}

/*******************************************************************************
* Name: adcNoiseReduction
* Description: Putting microcontroller into ADC noise reduction state. This is
*				 a very useful state when using the ADC to achieve best and low
*							noise signal.
*
* Argument		Description
* =========		===========
* 1. period		 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc		ADC module disable control. Turning off the ADC module is
*				basically removing the purpose of this low power mode.
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
*******************************************************************************/
void	LowPowerClass::adcNoiseReduction(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_ADC);
	cli();
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(true);
	#endif
}

/*******************************************************************************
* Name: powerDown
* Description: Putting microcontroller into power down state. This is
*				 the lowest current consumption state. Use this together with
*				 external pin interrupt to wake up through external event
*				 triggering (example: RTC clockout pin, SD card detect pin).
*
* Argument		Description
* =========		===========
* 1. period		 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc		ADC module disable control. Turning off the ADC module is
*				basically removing the purpose of this low power mode.
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. bod		Brown Out Detector (BOD) module disable control:
*				(a) BOD_SWITCH - Turn off BOD module during sleep
*				(b) BOD_ALWAYSOFF - Always turn off BOD module
*				(c) BOD_ALWAYSON - Always turn on BOD module
*				(d) BOD_DEFAULT - Leave BOD module in its default state
*
*******************************************************************************/
void	LowPowerClass::powerDown(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	cli();
	sleep_enable();
	#if defined (__AVR_ATmega328P__)
	if (LowPower.bod == BOD_SWITCH) LowPower.switch_bod(false);
	#endif
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
}

/*******************************************************************************
* Name: powerSave
* Description: Putting microcontroller into power save state. This is
*				 the lowest current consumption state after power down.
*				 Use this state together with an external 32.768 kHz crystal (but
*				 8/16 MHz crystal/resonator need to be removed) to provide an
*				 asynchronous clock source to Timer 2. Please take note that
*				 Timer 2 is also used by the Arduino core for PWM operation.
*				 Please refer to wiring.c for explanation. Removal of the external
*				 8/16 MHz crystal/resonator requires the microcontroller to run
*				 on its internal RC oscillator which is not so accurate for time
*				 critical operation.
*
* Argument		Description
* =========		===========
* 1. period	 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc		ADC module disable control. Turning off the ADC module is
*				basically removing the purpose of this low power mode.
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. bod		Brown Out Detector (BOD) module disable control:
*				(a) BOD_SWITCH - Turn off BOD module during sleep
*				(b) BOD_ALWAYSOFF - Always turn off BOD module
*				(c) BOD_ALWAYSON - Always turn on BOD module
*				(d) BOD_DEFAULT - Leave BOD module in its default state
*
* 4. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
*******************************************************************************/
void	LowPowerClass::powerSave(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	cli();
	sleep_enable();
	#if defined (__AVR_ATmega328P__)
	if (LowPower.bod == BOD_SWITCH) LowPower.switch_bod(false);
	#endif
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(true);
	#endif
}

/*******************************************************************************
* Name: powerStandby
* Description: Putting microcontroller into power standby state.
*
* Argument		Description
* =========		===========
* 1. period		 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc		ADC module disable control. Turning off the ADC module is
*				basically removing the purpose of this low power mode.
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. bod		Brown Out Detector (BOD) module disable control:
*				(a) BOD_SWITCH - Turn off BOD module during sleep
*				(b) BOD_ALWAYSOFF - Always turn off BOD module
*				(c) BOD_ALWAYSON - Always turn on BOD module
*				(d) BOD_DEFAULT - Leave BOD module in its default state
*
*******************************************************************************/
void	LowPowerClass::powerStandby(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_STANDBY);
	cli();
	sleep_enable();
	#if defined (__AVR_ATmega328P__)
	if (LowPower.bod == BOD_SWITCH) LowPower.switch_bod(false);
	#endif
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
}

/*******************************************************************************
* Name: powerExtStandby
* Description: Putting microcontroller into power extended standby state. This
*				 is different from the power standby state as it has the
*				 capability to run Timer 2 asynchronously.
*
* Argument		Description
* =========		===========
* 1. period		 Duration of low power mode. Use SLEEP_FOREVER to use other wake
*				up resource:
*				(a) SLEEP_15MS - 15 ms sleep
*				(b) SLEEP_30MS - 30 ms sleep
*				(c) SLEEP_60MS - 60 ms sleep
*				(d) SLEEP_120MS - 120 ms sleep
*				(e) SLEEP_250MS - 250 ms sleep
*				(f) SLEEP_500MS - 500 ms sleep
*				(g) SLEEP_1S - 1 s sleep
*				(h) SLEEP_2S - 2 s sleep
*				(i) SLEEP_4S - 4 s sleep
*				(j) SLEEP_8S - 8 s sleep
*				(k) SLEEP_FOREVER - Sleep without waking up through WDT
*
* 2. adc		ADC module disable control.
*				(a) ADC_SWITCH - Turn off ADC module during sleep
*				(b) ADC_ALWAYSOFF - Always turn off ADC module
*				(c) ADC_ALWAYSON - Always turn on ADC module
*				(d) ADC_DEFAULT - Leave ADC module in its default state
*
* 3. bod		Brown Out Detector (BOD) module disable control:
*				(a) BOD_SWITCH - Turn off BOD module during sleep
*				(b) BOD_ALWAYSOFF - Always turn off BOD module
*				(c) BOD_ALWAYSON - Always turn on BOD module
*				(d) BOD_DEFAULT - Leave BOD module in its default state
*
* 4. timer2		Timer 2 module disable control:
*				(a) TIMER2_SWITCH - Turn off Timer 2 module during sleep
*				(b) TIMER2_ALWAYSOFF - Always turn off Timer 2 module
*				(c) TIMER2_ALWAYSON - Always turn on Timer 2 module
*				(d) TIMER2_DEFAULT - Leave Timer 2 module in its default state
*
*******************************************************************************/
void	LowPowerClass::powerExtStandby(period_t period)
{
	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(false);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(false);
	#endif

	if (period != SLEEP_FOREVER)
	{
		wdt_enable(period);
		WDTCSR |= (1 << WDIE);
	}

	set_sleep_mode(SLEEP_MODE_EXT_STANDBY);
	cli();
	sleep_enable();
	#if defined (__AVR_ATmega328P__)
	if (LowPower.bod == BOD_SWITCH) LowPower.switch_bod(false);
	#endif
	sei();
	sleep_cpu();
	sleep_disable();
	sei();

	#if defined (__AVR__)
	if (LowPower.adc == ADC_SWITCH) LowPower.switch_adc(true);
	#endif
	#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega2560__) || defined (__AVR_ATmega256RFR2__)
	if (LowPower.timer2 == TIMER2_SWITCH) LowPower.switch_timer2(true);
	#endif
}

/*******************************************************************************
* Name: ISR (WDT_vect)
* Description: Watchdog Timer interrupt service routine. This routine is
*					 required to allow automatic WDIF and WDIE bit clearance in
*				 hardware.
*
*******************************************************************************/
ISR (WDT_vect)
{
	// WDIE & WDIF is cleared in hardware upon entering this ISR
	wdt_disable();
}

#elif defined (__arm__)
#if defined (__SAMD21G18A__)
/*******************************************************************************
* Name: standby
* Description: Putting SAMD21G18A into idle mode. This is the lowest current
*							consumption mode. Requires separate handling of clock and
* 				 peripheral management (disabling and shutting down) to achieve
* 				 the desired current consumption.
*
* Argument		Description
* =========		===========
* 1. idleMode	 Idle mode level (0, 1, 2) where IDLE_2 level provide lowest
* 				current consumption in this mode.
*
*******************************************************************************/
void	LowPowerClass::idle(idle_t idleMode)
{
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	PM->SLEEP.reg = idleMode;
	__DSB();
	__WFI();
}

/*******************************************************************************
* Name: standby
* Description: Putting SAMD21G18A into standby mode. This is the lowest current
*							consumption mode. Use this together with the built-in RTC (use
*							RTCZero library) or external pin interrupt to wake up through
*							external event triggering.
*
* Argument		Description
* =========		===========
* 1. NIL
*
*******************************************************************************/
void	LowPowerClass::standby()
{
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	__DSB();
	__WFI();
}

#else
	#error "Please ensure chosen MCU is ATSAMD21G18A."
#endif
#else
	#error "Processor architecture is not supported."
#endif


bod_t LowPowerClass::bod = BOD_DEFAULT;
adc_t LowPowerClass::adc = ADC_DEFAULT;
timer5_t LowPowerClass::timer5 = TIMER5_DEFAULT;
timer4_t LowPowerClass::timer4 = TIMER4_DEFAULT;
timer3_t LowPowerClass::timer3 = TIMER3_DEFAULT;
timer2_t LowPowerClass::timer2 = TIMER2_DEFAULT;
timer1_t LowPowerClass::timer1 = TIMER1_DEFAULT;
timer0_t LowPowerClass::timer0 = TIMER0_DEFAULT;
spi_t LowPowerClass::spi = SPI_DEFAULT;
usart3_t LowPowerClass::usart3 = USART3_DEFAULT;
usart2_t LowPowerClass::usart2 = USART2_DEFAULT;
usart1_t LowPowerClass::usart1 = USART1_DEFAULT;
usart0_t LowPowerClass::usart0 = USART0_DEFAULT;
twi_t LowPowerClass::twi = TWI_DEFAULT;
usb_t LowPowerClass::usb = USB_DEFAULT;

LowPowerClass LowPower;
