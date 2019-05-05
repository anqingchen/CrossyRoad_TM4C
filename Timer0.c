#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ADC.h"
#include "sprite.h"
#include "ST7735.h"

//=========================//
// TIMER0A
// Controls 'player' movment in game
// 60Hz polls ADC
//========================//
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
extern sprite_t player;

void Timer0A_Init(uint32_t dwell){
  int32_t sr;
  sr = StartCritical(); 
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
//PeriodicTask = task;          // user function
  TIMER0_CTL_R &= ~0x00000001;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = dwell;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
  EndCritical(sr);
}

uint32_t Convert(uint32_t input){
  return 80* ((416*input)/1000+158) / 1000 - 20;                    // convert raw data from ADC to 'cm'
}

void Timer0A_Handler(void) {
  player.x = Convert(ADC_In());
  if(player.x < 0) {
    player.x = 0;
  }
  if (player.x > 116) {
    player.x = 116;
  }
  ST7735_DrawBitmap(player.x, player.y, player.image, player.height, player.width);
}
