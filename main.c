#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC.h"
#include "TExaS.h"
#include "ST7735.h"
#include "SysTick.h"
#include "images.h"
#include "Timer0.h"
#include "sprite.h"

extern uint32_t ADCMail;
extern uint8_t ADCStatus;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

sprite_t player = {0, 0, 16, 13, mario, alive};

int main(void){  
  TExaS_Init();                                   // initializes TExaS Display 
  ST7735_InitR(INITR_REDTAB);                     // initialize ST7735 LCD Display
  ADC_Init();                                     // initialize ADC on PD2
  Timer0A_Init(1333333);                          // initialize SysTick to 60Hz
  EnableInterrupts();
  ST7735_DrawBitmap(0, 0, bg, 128, 160);
  while(1) {
      ST7735_DrawBitmap(player.x, player.y, player.image, player.height, player.width);
  }
}
