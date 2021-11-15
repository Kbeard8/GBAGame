#ifndef GAME_H
#define GAME_H

#include "gba.h"

#define Max_Enemys 5

                    /* TODO: */

            // Create any necessary structs //

void delay(int n);

  struct falcon {
  int row;
  int col;
  } player;

  struct enemy {
  int row;
  int col;
  int cd;
  int rd;
  int size;
  u16 color;
  int active; // 1 is active 0 is inactive
  } enemys[Max_Enemys];

  typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
} GBAState;




/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
*
*
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

#endif
