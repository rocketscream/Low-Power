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
	BOD_ALWAYSON,
	BOD_DEFAULT,
	BOD_SWITCH
};

enum adc_t
{
	ADC_ALWAYSOFF,
	ADC_ALWAYSON,
	ADC_DEFAULT,
	ADC_SWITCH
};

enum timer5_t
{
	TIMER5_ALWAYSOFF,
	TIMER5_ALWAYSON,
	TIMER5_DEFAULT,
	TIMER5_SWITCH
};

enum timer4_t
{
	TIMER4_ALWAYSOFF,
	TIMER4_ALWAYSON,
	TIMER4_DEFAULT,
	TIMER4_SWITCH
};

enum timer3_t
{
	TIMER3_ALWAYSOFF,
	TIMER3_ALWAYSON,
	TIMER3_DEFAULT,
	TIMER3_SWITCH
};

enum timer2_t
{
	TIMER2_ALWAYSOFF,
	TIMER2_ALWAYSON,
	TIMER2_DEFAULT,
	TIMER2_SWITCH
};

enum timer1_t
{
	TIMER1_ALWAYSOFF,
	TIMER1_ALWAYSON,
	TIMER1_DEFAULT,
	TIMER1_SWITCH
};

enum timer0_t
{
	TIMER0_ALWAYSOFF,
	TIMER0_ALWAYSON,
	TIMER0_DEFAULT,
	TIMER0_SWITCH
};

enum spi_t
{
	SPI_ALWAYSOFF,
	SPI_ALWAYSON,
	SPI_DEFAULT,
	SPI_SWITCH
};

enum usart0_t
{
	USART0_ALWAYSOFF,
	USART0_ALWAYSON,
	USART0_DEFAULT,
	USART0_SWITCH
};

enum usart1_t
{
	USART1_ALWAYSOFF,
	USART1_ALWAYSON,
	USART1_DEFAULT,
	USART1_SWITCH
};

enum usart2_t
{
	USART2_ALWAYSOFF,
	USART2_ALWAYSON,
	USART2_DEFAULT,
	USART2_SWITCH
};

enum usart3_t
{
	USART3_ALWAYSOFF,
	USART3_ALWAYSON,
	USART3_DEFAULT,
	USART3_SWITCH
};

enum twi_t
{
	TWI_ALWAYSOFF,
	TWI_ALWAYSON,
	TWI_DEFAULT,
	TWI_SWITCH
};

enum usb_t
{
	USB_ALWAYSOFF,
	USB_ALWAYSON,
	USB_DEFAULT,
	USB_SWITCH
};

enum idle_t
{
	IDLE_0,
	IDLE_1,
	IDLE_2
};

class LowPowerClass
{
	private:
		static unsigned char clockSource;

		static bod_t bod;
		static adc_t adc;
		static timer5_t timer5;
		static timer4_t timer4;
		static timer3_t timer3;
		static timer2_t timer2;
		static timer1_t timer1;
		static timer0_t timer0;
		static spi_t spi;
		static usart3_t usart3;
		static usart2_t usart2;
		static usart1_t usart1;
		static usart0_t usart0;
		static twi_t twi;
		static usb_t usb;

	public:
		#if defined (__AVR__)

			void switch_bod(bool on);
			void switch_adc(bool on);
			void switch_timer5(bool on);
			void switch_timer4(bool on);
			void switch_timer3(bool on);
			void switch_timer2(bool on);
			void switch_timer1(bool on);
			void switch_timer0(bool on);
			void switch_spi(bool on);
			void switch_usart3(bool on);
			void switch_usart2(bool on);
			void switch_usart1(bool on);
			void switch_usart0(bool on);
			void switch_twi(bool on);
			void switch_usb(bool on);

			#if defined (__AVR_ATmega328P__) || defined (__AVR_ATmega168__)
				void	setup(bod_t bod, adc_t adc, timer2_t timer2,
									timer1_t timer1, timer0_t timer0, spi_t spi,
									usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega2560__
				void	setup(adc_t adc, timer5_t timer5,
									timer4_t timer4, timer3_t timer3, timer2_t timer2,
									timer1_t timer1, timer0_t timer0, spi_t spi,
									usart3_t usart3, usart2_t usart2, usart1_t usart1,
									usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega256RFR2__
				void	setup(adc_t adc, timer5_t timer5,
									timer4_t timer4, timer3_t timer3, timer2_t timer2,
									timer1_t timer1, timer0_t timer0, spi_t spi,
									usart1_t usart1, usart0_t usart0, twi_t twi);
			#elif defined __AVR_ATmega32U4__
				void	setup(adc_t adc, timer4_t timer4,
									timer3_t timer3, timer1_t timer1, timer0_t timer0,
									spi_t spi, usart1_t usart1, twi_t twi, usb_t usb);
			#else
				#error "Please ensure chosen MCU is either 168, 328P, 32U4, 2560 or 256RFR2."
			#endif
			void	idle(period_t period);
			void	adcNoiseReduction(period_t period) __attribute__((optimize("-O1")));
			void	powerDown(period_t period) __attribute__((optimize("-O1")));
			void	powerSave(period_t period) __attribute__((optimize("-O1")));
			void	powerStandby(period_t period) __attribute__((optimize("-O1")));
			void	powerExtStandby(period_t period) __attribute__((optimize("-O1")));

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
