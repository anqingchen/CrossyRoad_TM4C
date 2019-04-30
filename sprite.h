#include <stdint.h>

typedef enum {dead,alive} status_t;
struct sprite{
  uint32_t x;      // x coordinate
  uint32_t y;      // y coordinate
  uint32_t height;
  uint32_t width;
  const unsigned short *image; // ptr->image
  status_t life;            // dead/alive
};          
typedef struct sprite sprite_t;
