#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "dac.h"
#include "sprite.h"
#include "ST7735.h"
#include "print.h"
#include "random.h"

#define TOTAL_ENEMY 10

extern sprite_t cars[];
extern sprite_t player;
extern uint32_t totalScore;
extern const unsigned short bg[];
extern const unsigned short player0img[];
extern const unsigned short gameover[];
extern volatile uint32_t FallingEdges;
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Reset_Handler(void);
uint32_t Random90(void);
uint8_t RandomDir(void);

void gameEngine(void);
int8_t gameCollision(void);
void gameOver(void);

void Timer2_Init(void){ 
  uint32_t volatile delay;
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  delay = SYSCTL_RCGCTIMER_R;
  TIMER2_CTL_R = 0x00000000;   // 1) disable timer2A
  TIMER2_CFG_R = 0x00000000;   // 2) 32-bit mode
  TIMER2_TAMR_R = 0x00000002;  // 3) periodic mode
  TIMER2_TAILR_R = 1000000;   // 4) reload value
  TIMER2_TAPR_R = 0;           // 5) clock resolution
  TIMER2_ICR_R = 0x00000001;   // 6) clear timeout flag
  TIMER2_IMR_R = 0x00000001;   // 7) arm timeout
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000;  // 8) priority 4
  NVIC_EN0_R = 1<<23;          // 9) enable IRQ 23 in
  TIMER2_CTL_R = 0x00000001;   // 10) enable timer2A
}



void Timer2A_Handler(void) {
  TIMER2_ICR_R = 0x00000001;  // acknowledge
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

uint32_t RandomSeed = 1;
void gameRestart(void) {
  Random_Init(RandomSeed);
  FallingEdges = 0;
  player.x = 0;
  player.y = 160;
  player.location = GRASS;
  player.image = player0img;
  cars[0].x = Random90();
  cars[0].direction = RandomDir();
  cars[1].x = Random90();
  cars[1].direction = RandomDir();
  cars[2].x = Random90();
  cars[2].direction = RandomDir();
  cars[3].x = Random90();
  cars[3].direction = RandomDir();
  cars[4].x = Random90();
  cars[4].direction = RandomDir();
  cars[5].x = Random90();
  cars[5].direction = RandomDir();
  cars[6].x = Random90();
  cars[6].direction = RandomDir();
  cars[7].x = Random90();
  cars[7].direction = RandomDir();
  cars[8].x = Random90();
  cars[8].direction = RandomDir();
  cars[9].x = Random90();
  cars[9].direction = RandomDir();
  RandomSeed = Random90();
}

void gameOver(void) {
  DisableInterrupts();
  ST7735_FillScreen(0xFFFF);
  ST7735_DrawBitmap(3, 100, gameover, 120, 50);
  ST7735_SetCursor(3, 11);
  ST7735_OutString("Your Score Is:");
  LCD_OutDec(totalScore- 1);
  ST7735_SetCursor(3, 12);
  ST7735_OutString("Resetting game");
  ST7735_SetCursor(3, 14);
  ST7735_OutString("Please Wait");
  ST7735_SetCursor(3, 15);
  ST7735_OutString("To Continue...");
  int32_t delay = 50000000;
  while(delay > 0) {
    delay--;
  }
  gameRestart();
  FallingEdges = 0;
  ST7735_DrawBitmap(0, 160, bg, 128, 160);
  EnableInterrupts();
  totalScore = 0;
}

void gameWon(void) {
  DisableInterrupts();
  ST7735_FillScreen(0xFFFF);
  // Need gamewon image here ==>
  // ST7735_DrawBitmap(3, 100, gamewon, 120, 50);
  ST7735_SetCursor(0, 11);
  ST7735_OutString("WINNING Score:");
  LCD_OutDec(totalScore);
  ST7735_SetCursor(3, 12);
  ST7735_OutString("Resetting game");
  ST7735_SetCursor(3, 14);
  ST7735_OutString("Please Wait");
  ST7735_SetCursor(3, 15);
  ST7735_OutString("To Continue...");
  int32_t delay = 50000000;
  while(delay > 0) {
    delay--;
  }
  gameRestart();
  ST7735_DrawBitmap(0, 160, bg, 128, 160);
  EnableInterrupts();
}

uint32_t Random90(void){
  return ((Random32()>>24)%85)+10;  // returns random location
}

uint8_t RandomDir(void){
  return ((Random32()>>24)%2);  // returns random location
}


