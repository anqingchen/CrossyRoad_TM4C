#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC.h"
#include "TExaS.h"
#include "ST7735.h"
#include "SysTick.h"
#include "images.h"

extern uint32_t ADCMail;
extern uint8_t ADCStatus;

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

uint32_t Convert(uint32_t input){
  return 80* ((416*input)/1000+158) / 1000;                    // convert raw data from ADC to 'cm'
}

int main(void){
  uint32_t Data;                                  // 12-bit ADC
  uint32_t Position;                              // 32-bit fixed-point 0.001 cm  
  TExaS_Init();                                   // initializes TExaS Display 
  ST7735_InitR(INITR_REDTAB);                     // initialize ST7735 LCD Display
  ADC_Init();                                     // initialize ADC on PD2
  SysTick_Init(666667);                          // initialize SysTick to 60Hz
  EnableInterrupts();
  while(1) {
    if(ADCStatus == 1) {
      Data = ADCMail;
      ADCStatus = 0;
      Position = Convert(Data);
      //
      ST7735_FillScreen(0xFFFF);
      ST7735_DrawBitmap(Position, 100, mario , 13, 16);
    }
    
  }
}
