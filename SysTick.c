#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADC.h"
#include "ST7735.h"
#include "sprite.h"
#include "random.h"
#include "print.h"
#include "dac.h"
#include "music.h"

#define TOTAL_ENEMY 10


long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

void SysTick_Init(){
  NVIC_ST_CTRL_R = 0;                             // 1) disable SysTick during setup 
  NVIC_ST_RELOAD_R = 7255;                   // 2) maximum reload value 
  NVIC_ST_CURRENT_R = 0;                          // 3) any write to CURRENT clears it 
  NVIC_ST_CTRL_R = 0x00000007;                    // 4) enable SysTick with core clock
}

// trigger is Timer2A Time-Out Interrupt
// set periodically TATORIS set on rollover
uint32_t musicPt = 0;

void SysTick_Handler(void){
  DAC_Out(happy[musicPt]);
  musicPt = (musicPt + 1) % 111979;
}


void SysTick_Stop(void){ 
  TIMER2_CTL_R &= ~0x00000001; // disable
}

void Systick_Start(void){ 
  TIMER2_CTL_R |= 0x00000001;   // enable
}