#ifndef __DISPLAY_H_
#define __DISPLAY_H_

#include "globals.h"
#define TUBE_COUNT 8
enum {TubeEmpty = 10, TubeHyphen = 11, TubeC = 12};
extern u8 TubeBuf[TUBE_COUNT];
void InitTube();
#endif