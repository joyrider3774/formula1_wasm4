#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>


#define enemyWidth 24
#define enemyHeight 24
#define enemyFlags BLIT_2BPP
extern const uint8_t enemy[];

#define playerWidth 24
#define playerHeight 24
#define playerFlags BLIT_2BPP
extern const uint8_t player[];

#define backgroundWidth 160
#define backgroundHeight 160
#define backgroundFlags BLIT_2BPP
extern const uint8_t background[];

#endif