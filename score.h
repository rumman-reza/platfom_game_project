#ifndef SCORE_H
#define SCORE_H
#include"types.h"

void loadHighScores(HighScoreEntry highScores[MAX_HIGH_SCORES]);
void saveHighScores(const HighScoreEntry highScores[MAX_HIGH_SCORES]);
int  tryAddHighScore(HighScoreEntry highScores[MAX_HIGH_SCORES], const char* name, int newScore);
void drawGameOverScores(const GS* gs, int isNewHighScore);
#endif