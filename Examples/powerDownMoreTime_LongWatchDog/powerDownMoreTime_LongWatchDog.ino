//Choose one of these see the samples
#define TEST_LOWPOWER
//#define TEST_LONGWATCHDOG
//#define TEST_LOWPOWER_AND_LONGWATCHDOG

#include "LowPower.h"

#ifndef TEST_LOWPOWER
    //This is the code if you want to use the AUTOLONGRESET feature, only define AUTOLONGRESETcounterMAX. You could put a watchdog feature for minutes, even hours.
    #include "avr/wdt.h"
    #define AUTOLONGRESETcounterMAX 3   //Number of times of ISR(WDT_vect) to autoreset the board. I will autoreset the board after 8 secondes x AUTOLONGRESETcounterMAX
    volatile boolean autoLongResetCounterEnabled = false;
    volatile int autoLongResetCounter;
    void ISR_RUTINE() {  //This function is declared as WEAK in LowPower library, so you can override if you want.
        if (autoLongResetCounterEnabled) {
            autoLongResetCounter += 1;
            if (autoLongResetCounter < AUTOLONGRESETcounterMAX - 1) {  //-1 is because of the next lines (see: WDTCSR = 0b00001000 | 0b100001;) I will wait again 8 seconds
                wdt_reset(); // Reset timer, still in interrupt mode
            } else {
                wdt_enabled(WDTO_8S);
                MCUSR = 0;
                WDTCSR |= 0b00011000;    //WDCE y WDE = 1 --> config mode
                WDTCSR = 0b00001000 | 0b100001;    //clear WDIE (interrupt mode disabled), set WDE (reset mode enabled) and set interval to 8 seconds
                                                   //We could take out "-1" from "if" and sleep here for 64 ms, but I prefer to repeat the 8s loop again, finally I get what I want.
            }
        } else {
            // It is only for sleep (autoLongResetCounterEnabled = false), so I copy source code from LowPower.h
            // WDIE & WDIF is cleared in hardware upon entering this ISR
            wdt_disable();        
        }
    }  

    void wdt_long_enable() {
        autoLongResetCounter = 0;
        autoLongResetCounterEnabled = true;
        cli();    //disabled ints
        MCUSR = 0;   //clear reset status
        WDTCSR |= 0b00011000;    //WDCE y WDE = 1 --> config mode
        WDTCSR = 0b01000000 | 0b100001;    //set WDIE (interrupt mode enabled), clear WDE (reset mode disabled) and set interval to 8 seconds
        sei();   //enable ints
    }
    
    void wdt_long_disable() {
        autoLongResetCounterEnabled = false;
        wdt_disable();
    }
#endif

void setup() {
#ifndef TEST_LOWPOWER
    wdt_long_disable();
#endif

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Serial.begin(115200);
  Serial.println("LowPower ISR weak TEST - FROM SETUP");
  delay(100);
#ifndef TEST_LOWPOWER
    wdt_long_enable();   //You have to enabled it when you want.
#endif
}

void loop() {
#ifdef TEST_LONGWATCHDOG
    //In real sketch you have to remove the comment in the next line
    //wdt_long_enable();   //Each loop, or each time you want, you have to reenable it to "reset" the timer,
                           //     otherwise, after AUTOLONGRESETcounterMAX x 8 seconds, your arduino will autoreset itself.
    delay(1000);
    Serial.print(millis()/1000);
    Serial.println(" seconds");  //After AUTOLONGRESETcounterMAX x 8 seconds you will see again the "LowPower ISR weak TEST - FROM SETUP" that is printed in SETUP,
                                 //That is meant that the IC has autoreseted itself.
#endif
#ifdef TEST_LOWPOWER
    LowPower.powerDownMoreTime(10, ADC_OFF, BOD_OFF);  //Working perfectly without defining a new ISR_RUTINE
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
#endif
#ifdef TEST_LOWPOWER_AND_LONGWATCHDOG
    wdt_long_disable();  //Disable WatchDog before sleeping.
    LowPower.powerDownMoreTime(10, ADC_OFF, BOD_OFF);  //Working perfectly defining my new special ISR_RUTINE that can give me a LONG WATCHDOG AUTORESET feature
    wdt_long_enable();   //Enabe it again
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(100000); //With this delay, it will autoreset
#endif
}
