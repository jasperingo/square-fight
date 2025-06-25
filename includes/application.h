#ifndef __SQUARE_APPLICATION_H__
#define __SQUARE_APPLICATION_H__
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define APPLICATION_NAME "SquareFight"

#define QUIT_SCREEN 0
#define HOME_SCREEN 1
#define GAME_SCREEN 2
#define SCORES_SCREEN 3

typedef struct {
  FILE* log_file;
  FILE* score_file;
	TTF_Font* font;
  SDL_Window* window;
	SDL_Renderer* renderer;
} square_application;

square_application* square_application_init(void);

void square_application_cleanup(square_application* application);

int home_screen(square_application* application);

int game_screen(square_application* application);

int scores_screen(square_application* application);

#endif
