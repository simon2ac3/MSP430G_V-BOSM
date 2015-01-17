
#include "msp430g2553.h"
#include "Stepper.h"

#define LED0 BIT0
#define LED1 BIT6

// Stepper motors configuration
#define MOTOR1_A BIT0
#define MOTOR1_B BIT6

void init();
void initLEDS();
void initClock();

Params* Motor1;

int main()
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  __enable_interrupt();
  init();

  while( 1 )
  {
    // TO DO: REMOVE THIS LATER
    int i;
    for ( i = 0 ; i < 100 ; i++ )
    {
      Step( Motor1 );
    }
    
    SetDirection ( Motor1, CCW );
    
    for ( i = 0 ; i < 100 ; i++ )
    {
      Step( Motor1 );
    }
    
    SetDirection ( Motor1, CW );
  }
  
  return 0;
}

void init()
{
  initLEDS();
  initClock();
  Motor1 = InitStepper(10, CW, MOTOR1_A, MOTOR1_B);
}

void initLEDS()
{
  P1DIR |= LED0 + LED1;   //set 1, direction on pin0 i pin6 (output)                         
  P1OUT |= LED1;         //set 0 on pin0
  P1OUT &= ~LED0;          //set 1 na pin6
}

void initClock()
{
  BCSCTL1 = CALBC1_1MHZ;                        // Set range
  DCOCTL = CALDCO_1MHZ;
  BCSCTL2 &= ~(DIVS_3);                         // SMCLK = DCO / 8 = 1MHz  

}