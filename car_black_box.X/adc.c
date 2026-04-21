#include <xc.h>
#include "adc.h"

/* NOTE: Justification ADFM
 * ADFM = 1 for Right Justification
 * ADFM = 0 for Left Justification
 * ADC result bit is 10 bits but its stored in two 8 bit registers ADRESH and ADRESL
 * Right justified (ADFM=1):
 *  ADRESH  →  0  0  0  0  0  0  b9 b8
 *  ADRESL  →  b7 b6 b5 b4 b3 b2 b1 b0
 *                                  ↑ easy to read as one number
 * Left justified (ADFM=0):
 * ADRESH  →  b9 b8 b7 b6 b5 b4 b3 b2
 * ADRESL  →  b1 b0  0  0  0  0  0  0
 *                                  ↑ harder to combin
 */

void init_adc() {

   ADFM = 1; // For Right justified use ADFM = 1

   /* NOTE: Acquisition Time
    * Before Converting ADC need time to charge the internal capacitor from the pin
    * This is called Acquisition Time. 010 in binary is 4 TAD ( units of ADC Time)
    * in simple words 0 TAD is too fast thus it can lead to inaccurate conversion
    * 4 TAD is good enough
    */
   ACQT2 = 0;
   ACQT1 = 1;
   ACQT0 = 0;


   /* NOTE: Conversion Clock
    * ADC needs its own clock to do the conversion. here we are selecting a fraction
    * of main clock
    * 010 in binary --> Fosc / 32 here
    * At 20 MHz:
    *           Fosc / 32 = 20,000,000 / 32 = 625,000 Hz
    *           1 TAD = 1 / 625,000 = 1.6 us
    *           Minimum TAD required by PIC is 0.7 us, so 1.6 us is safe option
    */
   ADCS0 = 0;
   ADCS1 = 1;
   ADCS2 = 0;

   /* NOTE: GODONE
    * GODONE = 0 means stop any conversion
    * This Bit starts conversion when it is set to 1.
    * During initialization it is safe to set GODONE bit as 0 for preventing accidental 
    * conversions.
    */
   GODONE = 0;

   /* NOTE: Voltage Reference
    * VCFG1 -> negative Voltage Reference = VSS (0V)
    * VCFG2 -> positive Voltage Reference = VDD (5V)
    */
   VCFG1 = 0;
   VCFG0 = 0;

   /* here ↓ we are just clearing up leftover garbage values before init
    */
   ADRESH = 0;
   ADRESL = 0;

   /* ADC is OFF by default to save power. this Turns ADC ON. Nothing Works without this
    */
   ADON = 1;
}


