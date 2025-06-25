#ifndef __SQUARE_SCORE_RECORDER_H__
#define __SQUARE_SCORE_RECORDER_H__
#include <stdio.h>
#include <time.h>
#include <SDL.h>
#include "application.h"

typedef struct {
  Uint64 size;
  char** items;
} square_scores;

Uint8 square_record_score(square_application* application, Uint64 score);

square_scores* square_read_scores(square_application* application);

void square_scores_cleanup(square_scores* scores);

#endif
