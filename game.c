#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
#include "images/garbage.h"
#include "images/scene.h"
#include "images/CF.h"
#include "images/FP.h"
#include "images/FP2.h"
#include "images/END.h"
/* TODO: */
// Add any additional states you need for your app.


int main(void) {

  REG_DISPCNT = MODE3 | BG2_ENABLE;

  player.row=80;
  player.col=80;
  int deltas[] = {-2, -1, 1, 2};
  int ndeltas = sizeof(deltas) / sizeof(deltas[0]);

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;


  while (1) {

    
    currentButtons = BUTTONS;  // Load the current state of the buttons


    switch (state) {
      case START:
      waitForVBlank();
      drawFullScreenImageDMA(END);
      drawString(110, 80, "FALCON DODGE!", WHITE);
      int health = 50;
      int numActive = Max_Enemys;
      int win = 0;

      //initializes enemys
      for (int i = 0; i < Max_Enemys; i++) {
      enemys[i].row = rand() % 160;
      enemys[i].col = rand() % 240;
      enemys[i].rd = deltas[rand() % ndeltas];
      enemys[i].cd = deltas[rand() % ndeltas];
      enemys[i].color = GRAY;
      enemys[i].size = (rand() % 3) + 5;
      enemys[i].active = 1;
  }

      if (KEY_JUST_PRESSED(BUTTON_START,currentButtons, previousButtons)) {
        state = PLAY;
      }
        // state = ?
        break;
      case PLAY:


      //Defines player controlled movement for player
      if (KEY_DOWN(BUTTON_DOWN,currentButtons)) {
        if (player.row != 159-CF_HEIGHT + 1) {
        player.row += 1;
        }
      }
      if (KEY_DOWN(BUTTON_UP,currentButtons)) {
        if (player.row != 0) { 
        player.row -= 1;
        }
      }
      if (KEY_DOWN(BUTTON_LEFT,currentButtons)) {
        if (player.col != 0) { 
       player.col -= 1;
        }
      }
      if (KEY_DOWN(BUTTON_RIGHT,currentButtons)) {
        if (player.col !=239-CF_WIDTH + 1) {
        player.col += 1;
        } 
      }

      // Defines movement for enemys
    for (int i = 0; i < Max_Enemys; i++) {
    enemys[i].row = enemys[i].row + enemys[i].rd;
    enemys[i].col += enemys[i].cd;
    if(enemys[i].row < 0) {
        enemys[i].row = 0;
        enemys[i].rd = -enemys[i].rd;
    }
    if(enemys[i].row > 159-enemys[i].size+1) {
        enemys[i].row = 159-enemys[i].size+1;
        enemys[i].rd = -enemys[i].rd;
    }
    if(enemys[i].col < 0) {
        enemys[i].col = 0;
        enemys[i].cd = -enemys[i].cd;
    }
    if(enemys[i].col > 239-enemys[i].size+1) {
        enemys[i].col = 239-enemys[i].size+1;
        enemys[i].cd = -enemys[i].cd;
    }

    //If an enemy is colliding with the player (they are in same spot), player loses health
    if ( (enemys[i].active == 1) && ((enemys[i].row > player.row) && (enemys[i].row < player.row+CF_HEIGHT)) && ((enemys[i].col > player.col) && (enemys[i].col < player.col+CF_WIDTH)) ){
        health--;
    }
  }




    // Fills background of screen and draws enemies
      waitForVBlank();
      fillScreenDMA(MAGENTA);
      for (int i = 0; i < Max_Enemys; i++) {
        if (enemys[i].active == 1) {
        drawRectDMA(enemys[i].row, enemys[i].col, 
        enemys[i].size, enemys[i].size, enemys[i].color);
        }
      }

      // str is health string that will be displayed on the screen
      char str[12];
      sprintf(str,"Health: %d", health);
      

      // If B is pressed the screenshows the falcon punch image, pauses the screen and removes the enemy if it is hit
      if (KEY_JUST_PRESSED(BUTTON_B,currentButtons, previousButtons)) {
        drawString(150, 5, str, BLACK);
        drawImageDMA(player.row,player.col, FP2_WIDTH,FP2_HEIGHT, FP2);;
        delay(20);
        for (int i = 0; i < Max_Enemys; i++) {
          if ((enemys[i].active == 1) && ((enemys[i].row > player.row) && (enemys[i].row < player.row+FP2_HEIGHT)) && ((enemys[i].col > player.col) && (enemys[i].col < player.col+FP2_WIDTH))){
          enemys[i].active = 0;
          numActive--;
          }
        }
      // If B is not pressed it just draws the falcon character
      } else {
        drawImageDMA(player.row,player.col, CF_WIDTH,CF_HEIGHT, CF);
        drawString(150, 5, str, BLACK);
      }
      
      
    // Logic for transitioning between game states... if player health is 0 player loses, if select then. game restarts, if all enemies dead the game restars  
    if(health == 0){
      state = LOSE;
    } else if (KEY_JUST_PRESSED(BUTTON_SELECT,currentButtons, previousButtons)) {
        state = START;
    } else if (numActive == 0) {
      state = WIN;
    }
    

        // state = ?
        break;
      case WIN:

        // logic so that win screen. does not have to be drawn overagain
        if (win == 0) {
          waitForVBlank();
          drawFullScreenImageDMA(scene);
          drawString(70, 95, "YOU WIN", WHITE);
        }
        win = 1;
        

        // Game Restarts if select button hit
        if (KEY_JUST_PRESSED(BUTTON_SELECT,currentButtons, previousButtons)) {
          state = START;
        }
        break;

      case LOSE:

      // Draws lose screen
      waitForVBlank();
      fillScreenDMA(BLACK);
      drawString(70, 95, "YOU LOSE", WHITE);
      if (KEY_JUST_PRESSED(BUTTON_SELECT,currentButtons, previousButtons)) {
        state = START;
      }
        // state = ?
        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }


  return 0;
}


// Delay function to help add emphasis onthe falcon punch
void delay(int n) {
  volatile int x = 0;
  for(int i=0; i<n*8000; i++) {
    x++;
  }
}