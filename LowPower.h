#ifndef LowPower_h
#define LowPower_h

#include "Arduino.h"

enum period_t
{
	SLEEP_15MS,
	SLEEP_30MS,
	SLEEP_60MS,
	SLEEP_120MS,
	SLEEP_250MS,
	SLEEP_500MS,
	SLEEP_1S,
	SLEEP_2S,
	SLEEP_4S,
	SLEEP_8S,
	SLEEP_FOREVER
};

enum bod_t
{
	BOD_OFF,
	BOD_ON
};

enum adc_t
{
	ADC_OFF,
	ADC_ON
};

enum timer5_t
{
	TIMER5_OFF,
	TIMER5_ON
};

enum timer4_t
{
	TIMER4_OFF,
	TIMER4_ON
};

enum timer3_t
{
	TIMER3_OFF,
	TIMER3_ON
};

enum timer2_t
{
	TIMER2_OFF,
	TIMER2_ON
};

enum timer1_t
{
	TIMER1_OFF,
	TIMER1_ON
};

enum timer0_t
{
	TIMER0_OFF,
	TIMER0_ON
};

enum spi_t
{
	SPI_OFF,
	SPI_ON
};

enum usart0_t
{
	USART0_OFF,
	USART0_ON
};

enum usart1_t
{
	USART1_OFF,
	USART1_ON
};

enum usart2_t
{
	USART2_OFF,
	USART2_ON
};

enum usart3_t
{
	USART3_OFF,
	USART3_ON
};

enum twi_t
{
	TWI_OFF,
	TWI_ON
};

enum usb_t
{
	USB_OFF,
	USB_ON
};

enum idle_t
{
	IDLE_0,
	IDLE_1,
	IDLE_2
};

class LowPowerClass
{
	public:
		#if defined (__AVR__)

			#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega88__)
				void	idle(period_t period, adc_t adc, timer2_t timer2,
						     timer1_t timer1, timer0_t timer0, spi_t spi,
					         usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega644P__ || defined (__AVR_ATmega1284P__)
				void	idle(period_t period, adc_t adc, timer2_t timer2,
							 timer1_t timer1, timer0_t timer0, spi_t spi,
							 usart1_t usart1, usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega2560__
				void	idle(period_t period, adc_t adc, timer5_t timer5,
							 timer4_t timer4, timer3_t timer3, timer2_t timer2,
							 timer1_t timer1, timer0_t timer0, spi_t spi,
							 usart3_t usart3, usart2_t usart2, usart1_t usart1,
							 usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega256RFR2__
				void	idle(period_t period, adc_t adc, timer5_t timer5,
									 timer4_t timer4, timer3_t timer3, timer2_t timer2,
				   				 timer1_t timer1, timer0_t timer0, spi_t spi,
						       usart1_t usart1,
									 usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega32U4__
				void	idle(period_t period, adc_t adc, timer4_t timer4,
				             timer3_t timer3, timer1_t timer1, timer0_t timer0,
				             spi_t spi, usart1_t usart1, twi_t twi, usb_t usb);
			#else
				#error "Please ensure chosen MCU is either 88, 168, 168P, 328P, 32U4, 2560 or 256RFR2."
			#endif
			void	adcNoiseReduction(period_t period, adc_t adc, timer2_t timer2) __attribute__((optimize("-O1")));
			void	powerDown(period_t period, adc_t adc, bod_t bod) __attribute__((optimize("-O1")));
			void	powerSave(period_t period, adc_t adc, bod_t bod, timer2_t timer2) __attribute__((optimize("-O1")));
			void	powerStandby(period_t period, adc_t adc, bod_t bod) __attribute__((optimize("-O1")));
			void	powerExtStandby(period_t period, adc_t adc, bod_t bod, timer2_t timer2) __attribute__((optimize("-O1")));

		#elif defined (__arm__)

			#if defined (__SAMD21G18A__)
				void	idle(idle_t idleMode);
				void	standby();
			#else
				#error "Please ensure chosen MCU is ATSAMD21G18A."
			#endif

		#else

			#error "Processor architecture is not supported."

		#endif
};

extern LowPowerClass LowPower;
#endif
