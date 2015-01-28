#pragma once
#define CW 1
#define CCW 0
#define uint unsigned int
#define TRUE 1
#define FALSE 0
#define NULL 0

enum Position {state1, state2, state3, state4} ;

typedef struct
{
  int speed;         // Step / ms
  int countDown;     // Countdown to the next action
  
  int direction;    // 1 - CW ; 0 - CCW
  uint controlA, controlB;
  int motorInitialized;
  enum Position position;
} Params;

Params* InitStepper(int, int, uint, uint);
void ProcessCommands ( Params* _motor1, Params* _motor2 );
void Step( Params* );
void SetDirection ( Params*, int );
void SetSpeed ( Params*, int );