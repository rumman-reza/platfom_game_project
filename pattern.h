#ifndef PATTERN_H
#define PATTERN_H

#include "types.h"

typedef struct pattern{
    const char **rows;
    int rowcount;
}pattern;

extern const pattern all_patterns[];
extern const int pattern_count;

void spawn_pattern(GS* gs, const pattern *p,float baseX,float groundY,float screenBottomY);
int getPatternWidth(const pattern* p);


#endif