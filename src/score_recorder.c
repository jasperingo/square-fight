#include "score_recorder.h"

Uint8 square_record_score(square_application* application, Uint64 score) {
  time_t rawtime = time(NULL);

  fwrite(&rawtime, sizeof(rawtime), 1, application->score_file);
  fwrite(&score, sizeof(score), 1, application->score_file);

  fflush(application->score_file);

  return 0;
}

square_scores* square_read_scores(square_application* application) {
  char datatime[200];
  
  struct tm *timeinfo;

  size_t bytes_read;

  time_t rawtime;

  Uint64 score;

  while (1) {

    if (fread(&rawtime, sizeof(rawtime), 1, application->score_file) != 1) {
      if (feof(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to EOF while reading date!\n");
      } else if (ferror(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to Error while reading date!\n");
      }

      break;
    }

    if (fread(&score, sizeof(score), 1, application->score_file) != 1) {
      if (feof(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to EOF while reading score!\n");
      } else if (ferror(application->score_file) > 0) {
        fprintf(application->log_file, "High score: Got to Error while reading score!\n");
      }

      break;
    }

    timeinfo = localtime(&rawtime);

    strftime(datatime, sizeof(datatime), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(application->log_file, "High score: Date = %s & Score = %d\n", datatime, score);
  }

  return NULL;
}

void square_read_scores_cleanup(square_scores* scores) {
  Uint16 i;

	for (i = 0; i < scores->size; i++) {
		free(scores->items[i]);
	}

	free(scores);
}
