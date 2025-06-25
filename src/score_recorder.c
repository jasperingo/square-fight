#include "score_recorder.h"

#define DATETIME_SIZE 50

Uint8 square_record_score(square_application* application, Uint64 score) {
  time_t rawtime = time(NULL);

  fwrite(&rawtime, sizeof(rawtime), 1, application->score_file);
  fwrite(&score, sizeof(score), 1, application->score_file);

  fflush(application->score_file);

  return 0;
}

square_scores* square_read_scores(square_application* application) {
  Uint16 i;

  char datetime[DATETIME_SIZE];
  
  struct tm *timeinfo;

  size_t bytes_read;

  time_t rawtime;

  Uint64 score;

  Uint64 score_char_size;

  char** temp_score_items = NULL;

  square_scores* scores = malloc(sizeof(*scores));

  if (scores == NULL) {
    fprintf(application->log_file, "High score: failed to allocate memory for square_scores\n");
    fflush(application->log_file);

    return NULL;
  }

  scores->size = 0;
  scores->items = malloc(sizeof(*scores->items));

  if (scores->items == NULL) {
    fprintf(application->log_file, "High score: failed to allocate memory for square_scores items\n");
    fflush(application->log_file);

    return NULL;
  }

  rewind(application->score_file);

  while (1) {

    if (fread(&rawtime, sizeof(rawtime), 1, application->score_file) != 1) {
      if (feof(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to EOF while reading date!\n");
        fflush(application->log_file);
      } else if (ferror(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to Error while reading date!\n");
        fflush(application->log_file);
      }

      break;
    }

    if (fread(&score, sizeof(score), 1, application->score_file) != 1) {
      if (feof(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to EOF while reading score!\n");
        fflush(application->log_file);
      } else if (ferror(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to Error while reading score!\n");
        fflush(application->log_file);
      }

      break;
    }

    if (scores->size > 0) {
      temp_score_items = realloc(scores->items, (scores->size + 1) * sizeof(*scores->items));

      if (temp_score_items == NULL) {
        fprintf(application->log_file, "High score: failed to re-allocate memory for square_scores items\n");
        fflush(application->log_file);
        
        continue;
      }

      scores->items = temp_score_items;
    }

    timeinfo = localtime(&rawtime);

    strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", timeinfo);

    score_char_size = score;

    for (i = 1; score_char_size > 0; i++) {
      score_char_size /= 10;
    }

    scores->items[scores->size] = malloc(DATETIME_SIZE * i * sizeof(**scores->items));

    if (scores->items[scores->size] == NULL) {
      fprintf(application->log_file, "High score: failed to allocate memory for square_scores item\n");
      fflush(application->log_file);
      
      continue;
    }

    sprintf(scores->items[scores->size], "%s - %d", datetime, score);

    scores->size++;
  }

  return scores;
}

void square_scores_cleanup(square_scores* scores) {
  Uint64 i;

	for (i = 0; i < scores->size; i++) {
		free(scores->items[i]);
	}

	free(scores);
}
