#include "application.h"
#include "score_recorder.h"

int scores_screen(square_application* application) {

  square_read_scores(application);
  
  return QUIT_SCREEN;
}
