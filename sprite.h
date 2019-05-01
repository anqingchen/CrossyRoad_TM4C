#include <stdint.h>

#define GRASS 0
#define ROAD 1
#define RIVER 2
#define LEFT 0
#define RIGHT 1

typedef enum {dead,alive} status_t;
struct sprite{
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  uint32_t height;
  uint32_t width;
  const unsigned short *image; // ptr->image
  status_t life;            // dead/alive
  uint8_t location;
  uint8_t direction;        // FOR OBEJECTS ONLY!
};          
typedef struct sprite sprite_t;
