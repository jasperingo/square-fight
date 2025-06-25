#include "application.h"
#include "score_recorder.h"

int scores_screen(square_application* application) {
  Uint16 i;

  int selected = 0;

	int next_screen = HOME_SCREEN;

  SDL_Event event;

	int mouse_x = 0;

	int mouse_y = 0;

  square_scores* scores;

  SDL_Color text_color = { 0xFF, 0xFF, 0xFF };
	
	SDL_Rect header = { 0, 0, SCREEN_WIDTH, 60 };

	SDL_Surface* back_surface;

	SDL_Texture* back_texture;

	SDL_Rect back_position = { 0, 0, SCREEN_WIDTH / 3, 50 };


  back_surface = TTF_RenderText_Solid(application->font, "< Back", text_color);

	if (back_surface == NULL) {
		fprintf(application->log_file, "Unable to render back surface! SDL_ttf Error: %s\n", TTF_GetError());

		return QUIT_SCREEN;
	}
	
	back_texture = SDL_CreateTextureFromSurface(application->renderer, back_surface);

	SDL_FreeSurface(back_surface);

	if (back_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered back! SDL Error: %s\n", SDL_GetError());

		return QUIT_SCREEN;
	}
  

  scores = square_read_scores(application);

  if (scores == NULL) {
		fprintf(application->log_file, "Unable to read scores\n");
    
		SDL_DestroyTexture(back_texture);

		return QUIT_SCREEN;
	}


  for (i = 0; i < scores->size; i++) {
    fprintf(application->log_file, "High score: %s\n", scores->items[i]);
    fflush(application->log_file);
	}


  while (selected == 0) { 

		while (SDL_PollEvent(&event) != 0) {

			if (event.type == SDL_QUIT) {
				selected = 1;
				next_screen = QUIT_SCREEN;
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);

				if (
					back_position.x <= mouse_x &&
					(back_position.x + back_position.w) >= mouse_x &&
					back_position.y <= mouse_y &&
					(back_position.y + back_position.h) >= mouse_y
				) {
					selected = 1;
				}

			}
    }

    SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(application->renderer);

    SDL_SetRenderDrawColor(application->renderer, 0x00, 0x00, 0x00, 0x00);   

		SDL_RenderFillRect(application->renderer, &header);

		SDL_RenderCopy(application->renderer, back_texture, NULL, &back_position);

		SDL_RenderPresent(application->renderer);
  }

  square_scores_cleanup(scores);

  SDL_DestroyTexture(back_texture);

  return next_screen;
}
