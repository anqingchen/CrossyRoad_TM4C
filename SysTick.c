#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADC.h"
#include "ST7735.h"

void SysTick_Init(uint32_t reloadVal){
  NVIC_ST_CTRL_R = 0;                             // 1) disable SysTick during setup 
  NVIC_ST_RELOAD_R = reloadVal;                   // 2) maximum reload value 
  NVIC_ST_CURRENT_R = 0;                          // 3) any write to CURRENT clears it 
  NVIC_ST_CTRL_R = 0x00000007;                    // 4) enable SysTick with core clock
}

void SysTick_Handler(void) {
}
