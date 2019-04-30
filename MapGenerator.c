#include <stdint.h> 
#include "../inc/tm4c123gh6pm.h"
#include "random.h"

uint8_t chunkID;
uint8_t currentChunks[] = {0, 0, 0, 0};
uint16_t displayImg[20480];

uint32_t Random9(void){
  return ((Random32()>>24)%9);
}

void nextChunk(void) {
  uint32_t n = Random9();
  switch(n) {
    case 0: case 1: case 2: case 3:
      chunkID = 0;
    case 4: case 5: case 6:
      chunkID = 1;
    case 7: case 8:
      chunkID = 2;
  }
}
