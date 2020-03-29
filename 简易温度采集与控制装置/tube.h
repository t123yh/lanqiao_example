#ifndef __TUBE_H
#define __TUBE_H

#include "globals.h"

#define TUBE_COUNT 8

enum {TubeEmpty = 10, TubeHyphen = 11};

extern int TubeBuf[TUBE_COUNT];
void InitTube();

#endif