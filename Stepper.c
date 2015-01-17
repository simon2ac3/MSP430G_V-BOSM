#include "Stepper.h"
#include "msp430g2553.h"

#define POUT P1OUT
#define PDIR P1DIR

Params params = { .speed = 0, .direction = 0, .controlA = 0x0000u, .controlB = 0x0000u };

Params* InitStepper ( int _speed, int _direction, uint _controlA, uint _controlB )
{
  //Params params = { .speed = 0, .direction = 0, .controlA = 0x0000u, .controlB = 0x0000u};
  params.speed = _speed;
  params.direction = _direction;
  params.controlA = _controlA;
  params.controlB = _controlB;

  PDIR |= _controlA + _controlB;        //output
  POUT |= _controlA + _controlB;        //set 0
  
  return &params;
}

void Delay( int speed )
{
  for ( int i = 0; i < speed ; i++ )
  {
    __delay_cycles( 250 );
  }
}

void StepForward( Params* paramsPtr )
{
      //Stage1
      POUT |= paramsPtr->controlA + paramsPtr->controlB;           // 1 1
      Delay( paramsPtr->speed );
      //Stage2
      POUT &= ~paramsPtr->controlB;                // 1 0
      Delay( paramsPtr->speed );
      //Stage3
      POUT &= ~paramsPtr->controlA;                 // 0 0
      Delay( paramsPtr->speed );
      //Stage4
      POUT |= paramsPtr->controlB;                  // 0 1
      Delay( paramsPtr->speed );
}

void StepBackward( Params* paramsPtr )
{
      //Stage1
      POUT |= paramsPtr->controlA + paramsPtr->controlB;           // 0 1
      Delay( paramsPtr->speed );
      //Stage2
      POUT &= ~ paramsPtr->controlA;                // 0 0
      Delay( paramsPtr->speed );
      //Stage3
      POUT &= ~paramsPtr->controlB;                 // 1 0
      Delay( paramsPtr->speed );
      //Stage4
      POUT |= paramsPtr->controlA;                  // 1 1
      Delay( paramsPtr->speed );
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