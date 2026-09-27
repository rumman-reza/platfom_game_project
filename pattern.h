#ifndef PATTERN_H
#define PATTERN_H

#include "types.h"

typedef struct pattern{
    const char **rows;
    int rowcount;
}pattern;

extern const pattern all_easy_patterns[];
extern const pattern all_medium_patterns[];
extern const int pattern_count1;
extern const int pattern_count2;

void spawn_pattern(GS* gs, const pattern *p,float baseX,float groundY,float screenBottomY);
int getPatternWidth(const pattern* p);


#endif