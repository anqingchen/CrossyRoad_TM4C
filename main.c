#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC.h"
#include "TExaS.h"
#include "ST7735.h"
#include "SysTick.h"
#include "images.h"
#include "Timer0.h"
#include "Timer2.h"
#include "sprite.h"
#include "Inputs.h"
#include "dac.h"
#include "random.h"

extern uint32_t ADCMail;
extern uint8_t ADCStatus;
void gameRestart(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Interrupts_Init(void);

uint8_t enemySpeed = 2;
uint32_t totalScore = 0;

sprite_t player = {0, 160, 12, 12, player0img, GRASS, RIGHT};
sprite_t cars[] = { {0, 147, 20, 12, carimg, ROAD, RIGHT},
                    {20, 134,      20, 12, carimg, ROAD, RIGHT},
                    {40, 121, 20, 12, carimg, ROAD, RIGHT},
                    {80, 108, 20, 12, carimg, ROAD, RIGHT},
                    {100, 95, 20, 12, carimg, ROAD, RIGHT},
                    {0, 69, 20, 12, car2, RIVER, RIGHT},
                    {20, 56, 20, 12, car2, RIVER, RIGHT},
                    {40, 43, 20, 12, car2, RIVER, RIGHT},
                    {80, 30, 20, 12, car2, RIVER, RIGHT},
                    {100, 17, 20, 12, car2, RIVER, RIGHT}};

int main(void){  
  TExaS_Init();                                   // initializes TExaS Display 
  ST7735_InitR(INITR_REDTAB);                     // initialize ST7735 LCD Display
  ADC_Init();                                     // initialize ADC on PD2
  DAC_Init();
  gameRestart();
  ST7735_DrawBitmap(0, 160, bg, 128, 160);
  Interrupts_Init();
  while(1){
  }
}

void Interrupts_Init(void) {
  Input_Init();
  Input1_Init(); 
  Timer2_Init();
  Timer0A_Init(1333333);                          // initialize Timer0A to 60Hz
  SysTick_Init(enemySpeed);
  EnableInterrupts();
}
