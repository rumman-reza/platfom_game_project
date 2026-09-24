#include <stdio.h>
#include <string.h>
#include "score.h"
#include "constants.h"

void loadHighScores(HighScoreEntry highScores[MAX_HIGH_SCORES]) {
    FILE* f = fopen(HIGHSCORE_FILE, "r");
    if (!f) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            highScores[i].name[0] = '\0';
            highScores[i].score = 0;
        }
        return;
    }
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        // "%15s" width must stay one less than MAX_NAME_LEN, to leave room for '\0'
        if (fscanf(f, "%15s %d", highScores[i].name, &highScores[i].score) != 2) {
            highScores[i].name[0] = '\0';
            highScores[i].score = 0;
        }
    }
    fclose(f);
}

void saveHighScores(const HighScoreEntry highScores[MAX_HIGH_SCORES]) {
    FILE* f = fopen(HIGHSCORE_FILE, "w");
    if (!f) return;
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        fprintf(f, "%s %d\n", highScores[i].name, highScores[i].score);
    }
    fclose(f);
}

int tryAddHighScore(HighScoreEntry highScores[MAX_HIGH_SCORES], const char* name, int newScore) {
    if (newScore <= highScores[MAX_HIGH_SCORES - 1].score) return 0;

    int i = MAX_HIGH_SCORES - 1;
    while (i > 0 && highScores[i - 1].score < newScore) {
        highScores[i] = highScores[i - 1];   // struct assignment copies name + score together
        i--;
    }

    highScores[i].score = newScore;
    strncpy(highScores[i].name, name, MAX_NAME_LEN - 1);
    highScores[i].name[MAX_NAME_LEN - 1] = '\0';   // strncpy doesn't guarantee this on truncation

    saveHighScores(highScores);
    return 1;
}

void drawGameOverScores(const GS* gs, int isNewHighScore) {
    DrawText(TextFormat("Score: %d", gs->score), s_width/2 - 80, 150, 30, RAYWHITE);
    if (isNewHighScore) {
        DrawText("New High Score!", s_width/2 - 100, 190, 24, YELLOW);
    }

    DrawText("High Scores", s_width/2 - 80, 240, 24, RAYWHITE);
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        DrawText(TextFormat("%d. %s - %d", i + 1, gs->highScores[i].name, gs->highScores[i].score),s_width/2 - 80, 270 + i * 30, 20, LIGHTGRAY);
    }
}