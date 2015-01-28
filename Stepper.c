#include "Stepper.h"
#include "msp430g2553.h"

#define MAX_MOTOR_COUNT 2

#define POUT P1OUT
#define PDIR P1DIR

int timerInitialized = 0;
int tick = FALSE;

Params params [MAX_MOTOR_COUNT] = { {0}, {0} };

void initTimer(void)
 {
  CCTL0 = CCIE;
  //TO DO: connect this value with a speed. I'm guessing this will be set to 1ms
  CCR0 = 0X03E8;    //1000
  TACTL = TASSEL_2 + ID_3 + MC_3;
  timerInitialized = 1;
 }

Params* InitStepper ( int _speed, int _direction, uint _controlA, uint _controlB )
{
  for ( int i = 0; i < MAX_MOTOR_COUNT; i++ )
  {
    if ( params[i].motorInitialized == FALSE )
    {
     params[i].motorInitialized = TRUE; 
     params[i].speed = _speed;
     params[i].countDown = _speed;
     params[i].position = state1;
     
     params[i].direction = _direction;
     params[i].controlA = _controlA;
     params[i].controlB = _controlB;

     PDIR |= _controlA + _controlB;        //output
     POUT |= _controlA + _controlB;        //set 0
  
     if ( !timerInitialized )
        initTimer();
    
     return &params[i];
    }
  }
  return NULL;
  
}

void CheckStateAndStep ( Params* paramsPtr )
{
  if ( paramsPtr != NULL && paramsPtr->motorInitialized == TRUE )
  {
    if ( paramsPtr->countDown == 0 )
    {
      Step( paramsPtr );
      paramsPtr->countDown = paramsPtr->speed;
    }
    paramsPtr->countDown -- ;
  }
}

void ProcessCommands ( Params* _motor1, Params* _motor2 )
{
  if ( tick == TRUE )
  {
     CheckStateAndStep ( _motor1 );
     CheckStateAndStep ( _motor2 );       
     tick = FALSE;
  }
  
}
  
//void Delay( int speed )
//{
//  for ( int i = 0; i < speed ; i++ )
//  {
//    __delay_cycles( 250 );
//  }
//}

void StepForward( Params* paramsPtr )
{
      switch ( paramsPtr->position )
      {
        case state1:
          POUT |= paramsPtr->controlA + paramsPtr->controlB;           // 1 1
          paramsPtr->position = state2;
          break;
        case state2:
          POUT &= ~paramsPtr->controlB;                // 1 0
          paramsPtr->position = state3;
          break;
        case state3:
          POUT &= ~paramsPtr->controlA;                 // 0 0
          paramsPtr->position = state4;
          break;        
        case state4:
          POUT |= paramsPtr->controlB;                  // 0 1
          paramsPtr->position = state1;
          break;          
      }
}

void StepBackward( Params* paramsPtr )
{
      switch ( paramsPtr->position )
      {
        case state4:
          POUT &= ~paramsPtr->controlA;              // 0 1
          POUT |= paramsPtr->controlB;
          paramsPtr->position = state3;
          break;        
        case state3:
          POUT &= ~ paramsPtr->controlB;                // 0 0
          paramsPtr->position = state2;
          break;
        case state2:
          POUT |= paramsPtr->controlA;                 // 1 0
          paramsPtr->position = state1;
          break;
        case state1:
          POUT |= paramsPtr->controlB;                  // 1 1
          paramsPtr->position = state4;
          break;
      }
}

void Step( Params* paramsPtr )
{
  switch ( paramsPtr->direction )
  {
    case CCW:    //CCW
      StepBackward ( paramsPtr );
      break;
    case CW:    // CW
      StepForward ( paramsPtr );
      break;
  }
}

void SetDirection( Params* paramsPtr, int _direction)
{
  paramsPtr->direction = _direction;
}

void SetSpeed( Params* paramsPtr, int _speed)
{
  paramsPtr->speed = _speed;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  tick = TRUE;
}