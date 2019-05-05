#include <stdint.h>

#define GRASS 0
#define ROAD 1
#define RIVER 2
#define LEFT 0
#define RIGHT 1

struct sprite{
  int32_t x;      // x coordinate
  int32_t y;      // y coordinate
  uint32_t height;
  uint32_t width;
  const unsigned short *image; // ptr->image
  uint8_t location;
  uint8_t direction;        // FOR OBEJECTS ONLY!
};          
typedef struct sprite sprite_t;
