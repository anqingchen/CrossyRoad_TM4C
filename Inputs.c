#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "sprite.h"
#include "ST7735.h"
extern const unsigned short player0bg[];
extern const unsigned short player0img[];
extern const unsigned short player1bg[];
extern const unsigned short player1img[];
extern const unsigned short player2bg[];
extern const unsigned short player2img[];

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

extern sprite_t player;

volatile uint32_t FallingEdges = 0;
void Input_Init(void){                          
  SYSCTL_RCGCGPIO_R |= 0x00000020; // (a) activate clock for port F
  FallingEdges = 0;             // (b) initialize counter
  GPIO_PORTF_DIR_R &= ~0x10;    // (c) make PF4 in (built-in button)
  GPIO_PORTF_AFSEL_R &= ~0x10;  //     disable alt funct on PF4
  GPIO_PORTF_DEN_R |= 0x10;     //     enable digital I/O on PF4   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // configure PF4 as GPIO
  GPIO_PORTF_AMSEL_R = 0;       //     disable analog functionality on PF
  GPIO_PORTF_PUR_R |= 0x10;     //     enable weak pull-up on PF4
  GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= ~0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00200000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
  EnableInterrupts();           // (i) Clears the I bit
}

void GPIOPortF_Handler(void){
  DisableInterrupts();
  FallingEdges = FallingEdges + 1;
  switch(player.location){
    case 0:
      ST7735_DrawBitmap(player.x, player.y, player0bg, player.height, player.width);
      break;
    case 1:
      ST7735_DrawBitmap(player.x, player.y, player1bg, player.height, player.width);
      break;
    case 2:
      ST7735_DrawBitmap(player.x, player.y, player2bg, player.height, player.width);
      break;
  }
  player.y -= 13;
  switch(FallingEdges){
    case 1: case 2: case 3: case 4: case 5:
      player.location = ROAD;
      player.image = player1img;
      break;
    case 6: 
      player.location = GRASS;
      player.image = player0img;
      break;
    case 7: case 8: case 9: case 10: case 11:
      player.location = RIVER;
      player.image = player2img;
      break;
    case 12:
      //gameWon();
      break;
  }
  EnableInterrupts();
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag4
}
