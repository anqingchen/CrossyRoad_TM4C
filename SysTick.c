#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADC.h"
#include "ST7735.h"
#include "sprite.h"
#include "random.h"

#define TOTAL_ENEMY 10

extern sprite_t cars[];
extern sprite_t player;
extern const unsigned short bg[];
extern const unsigned short player0img[];
extern const unsigned short gameover[];
extern volatile uint32_t FallingEdges;
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Reset_Handler(void);
uint32_t Random90(void);

void gameEngine(void);
int8_t gameCollision(void);
void gameOver(void);

void SysTick_Init(uint32_t reloadVal){
  NVIC_ST_CTRL_R = 0;                             // 1) disable SysTick during setup 
  NVIC_ST_RELOAD_R = reloadVal;                   // 2) maximum reload value 
  NVIC_ST_CURRENT_R = 0;                          // 3) any write to CURRENT clears it 
  NVIC_ST_CTRL_R = 0x00000007;                    // 4) enable SysTick with core clock
}

void SysTick_Handler(void) {
  gameEngine();
  if(gameCollision() == -1) {
    gameOver();
  }
}

void gameEngine(void) {
  for(int i = 0; i < TOTAL_ENEMY; i++) {
    if(cars[i].direction == RIGHT){
      cars[i].x++;
      if(cars[i].x + 1 == 110) {
        cars[i].direction = LEFT;
      }
    } else {
      cars[i].x--;
        if(cars[i].x - 1 <= 0) {
          cars[i].direction = RIGHT;
        }
    }
    ST7735_DrawBitmap(cars[i].x, cars[i].y, cars[i].image, cars[i].height, cars[i].width);
  }
}

int8_t gameCollision(void) {
  for(int i = 0; i < TOTAL_ENEMY; i++) {
    if(player.y == cars[i].y) {
      if((player.x >= cars[i].x) && (player.x <= (cars[i].x + 20))){
        return -1;
      }
      if(((player.x + 12) >= cars[i].x) && (player.x <= cars[i].x)){
        return -1;
      }
    }
  }
  return 0;
}

void gameRestart(void) {
  FallingEdges = 0;
  player.x = 0;
  player.y = 160;
  player.life = alive;
  player.location = GRASS;
  player.image = player0img;
  cars[0].x = Random90();
  cars[1].x = Random90();
  cars[2].x = Random90();
  cars[3].x = Random90();
  cars[4].x = Random90();
  cars[5].x = Random90();
  cars[6].x = Random90();
  cars[7].x = Random90();
  cars[8].x = Random90();
  cars[9].x = Random90();
}

void gameOver(void) {
  DisableInterrupts();
  ST7735_FillScreen(0xFFFF);
  ST7735_DrawBitmap(3, 100, gameover, 120, 50);
  int32_t delay = 100000000;
  while(delay > 0) {
    delay--;
  }
  gameRestart();
  ST7735_DrawBitmap(0, 160, bg, 128, 160);
  EnableInterrupts();
}

uint32_t Random90(void){
  return ((Random32()>>24)%85)+10;  // returns 1, 2, 3, 4, or 5
}

