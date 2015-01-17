#pragma once
#define CW 1
#define CCW 0
#define uint unsigned int

typedef struct
{
  int speed;         // Step / ms
  int direction;    // 1 - CW ; 0 - CCW
  uint controlA, controlB;
} Params;

Params* InitStepper(int, int, uint, uint);
void Step( Params* );
void SetDirection ( Params*, int );
