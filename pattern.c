#include"pattern.h"
#include"ground.h"
#include<string.h>
#include"enemy.h"

//defining the patterns
static const char* gapspike[] = { 
    ".....S....",
    "GG...GGG.."};
static const char* floatingPlatform[] = {
    ".......H..PP....",
    ".......PP.......",
    "..PP............",
    ".E..............",
    "GG....GGGGGG...."
};
static const char* enemyAmbush[] = {
     "...E.....E",
     "GGGGGGGGGG" };
static const char* spikeGaunlet[] = {
    ".S.....S...S..",
    "GGG...GG...GGG"
};
//storing the patterns in all in one struct

const pattern all_patterns[]={
    {.rows = gapspike, .rowcount = sizeof(gapspike)/sizeof(gapspike[0])},
    {.rows = floatingPlatform, .rowcount = sizeof(floatingPlatform)/sizeof(floatingPlatform[0])},
    {.rows = spikeGaunlet,.rowcount = sizeof(spikeGaunlet)/sizeof(spikeGaunlet[0])},
    {.rows=enemyAmbush,.rowcount=sizeof(enemyAmbush)/sizeof(enemyAmbush[0])}
};

const int pattern_count = sizeof(all_patterns)/sizeof(all_patterns[0]);

int getPatternWidth(const pattern* p)
{
    int maxWidth = 0;

    for(int i = 0; i < p->rowcount; i++)
    {
        int width = strlen(p->rows[i]);

        if(width > maxWidth)
            maxWidth = width;
    }

    return maxWidth;
}

void spawn_pattern(GS* gs,const pattern *p,float baseX,float groundY,float screenBottomY){

    
    for(int row = 0; row < p->rowcount; row++)
    {
        const char* line = p->rows[row];
        
        int cols = strlen(line);
        
        bool isGroundRow = (row == p->rowcount - 1);
        
        float pattern_height = patternheight; // this is for setting the position
        float rowY;

        if(isGroundRow) rowY = groundY;

        else rowY = groundY- (p->rowcount - row)* pattern_height;
        
        float tileHeight = isGroundRow ? screenBottomY - groundY : pattern_height; // this is the actual height of the tile

        
        for(int col = 0; col < cols; col++)
        {
            
            float colX =  baseX + col * pattern_tile_width;

            switch(line[col])
            {
                case '.':
                    break;

                case 'G':

                case 'P':
                {
                    pushgroundchunk(gs,colX,rowY,tileHeight,pattern_tile_width,false);
                    break;
                }
                case 'E':
                {
                    // if(!isGroundRow){
                    //     TraceLog(LOG_WARNING,"Pattern: 'E' at row %d not on ground row, skipping (enemy AI is ground-only)",row);
                    //     break;
                    // }
                    spawnEnemy(gs,colX,rowY);
                    break;
                }
                case 'S':
                {
                    float spikeHeight = 80.0f;
                    addspike(gs,colX,rowY,pattern_tile_width,spikeHeight);
                    break;
                }
                case 'H':{
                    spawn_healthrect(gs,colX,rowY,pattern_tile_width);
                    break;
                }

                default:
                    TraceLog(LOG_WARNING,"Pattern: unknown char '%c' at row %d col %d",line[col],row,col);
                    break;
            }
        }
    }
}