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
    float startY = 450.0f;
    
    const char* scoreText = TextFormat("Score: %d", gs->score);
    Vector2 scoreSize = MeasureTextEx(gs->cfonts.menu_font3, scoreText, 40, 0);
    DrawTextEx(gs->cfonts.menu_font3, scoreText, (Vector2){(s_width/2.0f) - (scoreSize.x/2.0f), startY}, 40, 0, RAYWHITE);
    
    if (isNewHighScore) {
        const char* hsText = "New High Score!";
        Vector2 hsSize = MeasureTextEx(gs->cfonts.menu_font3, hsText, 30, 0);
        DrawTextEx(gs->cfonts.menu_font3, hsText, (Vector2){(s_width/2.0f) - (hsSize.x/2.0f), startY + 50}, 30, 0, YELLOW);
    }

    const char* titleText = "High Scores";
    Vector2 titleSize = MeasureTextEx(gs->cfonts.menu_font3, titleText, 30, 0);
    DrawTextEx(gs->cfonts.menu_font3, titleText, (Vector2){(s_width/2.0f) - (titleSize.x/2.0f), startY + 130}, 30, 0, GOLD);
    
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        const char* entryText = TextFormat("%d. %s - %d", i + 1, gs->highScores[i].name, gs->highScores[i].score);
        Vector2 entrySize = MeasureTextEx(gs->cfonts.menu_font3, entryText, 25, 0);
        DrawTextEx(gs->cfonts.menu_font3, entryText, (Vector2){(s_width/2.0f) - (entrySize.x/2.0f), startY + 180 + (i * 40)}, 25, 0, LIGHTGRAY);
    }
}