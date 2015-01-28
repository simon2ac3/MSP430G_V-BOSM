
#include "msp430g2553.h"
#include "Stepper.h"

#define LED0 BIT0
#define LED1 BIT6
#define BUTTON BIT3

// Stepper motors configuration
#define MOTOR1_A BIT0
#define MOTOR1_B BIT6

#define MOTOR2_A BIT0
#define MOTOR2_B BIT6

// Declarations
void init();
void initLEDS();
void initClock();
void initButton(void);

// Globals
Params* motor1;
Params* motor2;

int main()
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  __enable_interrupt();
  init();

  while( (P1IFG && BUTTON) )
  {
    ProcessCommands( motor1, motor2 );
    
//    // TO DO: REMOVE THIS LATER
//    int i;
//    for ( i = 0 ; i < 100 ; i++ )
//    {
//      Step( motor1 );
//    }
//    
//    SetDirection ( motor1, CCW );
//    
//    for ( i = 0 ; i < 100 ; i++ )
//    {
//      Step( motor1 );
//    }
//    
//    SetDirection ( motor1, CW );
  }
  
  return 0;
}

// Definitions
void init()
{
  initLEDS();
  initClock();
  initButton();
  motor1 = InitStepper(30, CCW, MOTOR1_A, MOTOR1_B);
  //motor2 = InitStepper(5, CW, MOTOR2_A, MOTOR2_B);
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

void initButton(void)
{
  P1DIR &= ~BUTTON;  // direction, set 0 (input)
  P1REN |= BUTTON;   // set 1, resistor pull-up/down enabled
  P1OUT |= BUTTON;   // set 1, resistor pulled-up
  P1IES &= ~BUTTON;  // set 1 falling edge triggered
  P1IFG &= ~BUTTON;  // set 0, int. flag reg.
  P1IE |= BUTTON;    // set 1, interrupts enable
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{}