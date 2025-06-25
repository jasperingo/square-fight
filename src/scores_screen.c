#include "application.h"
#include "score_recorder.h"

#define PAGE_SIZE 5

int scores_screen(square_application* application) {
  Uint16 i;

  int selected = 0;

	int next_screen = HOME_SCREEN;

  SDL_Event event;

	int mouse_x = 0;

	int mouse_y = 0;

  square_scores* scores;

	SDL_Rect header = { 0, 0, SCREEN_WIDTH, 60 };
  
	SDL_Surface* text_surface;
  
	SDL_Texture* back_texture;

  SDL_Color back_color = { 0xFF, 0xFF, 0xFF };
  
	SDL_Rect back_position = { 0, 0, SCREEN_WIDTH / 3, 50 };

  SDL_Color navigation_color = { 0x00, 0x00, 0x00 };

	SDL_Texture* next_texture;
  
	SDL_Rect next_position = { SCREEN_WIDTH - (SCREEN_WIDTH / 3), SCREEN_HEIGHT - 50, SCREEN_WIDTH / 3, 50 };
  
	SDL_Texture* previous_texture;
  
	SDL_Rect previous_position = { 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH / 3, 50 };
  
  Uint64 page = 1;

  Uint64 page_start_index = 0;

  Uint64 page_stop_index = PAGE_SIZE;

  Uint64 scores_textures_size;
  
	SDL_Texture** scores_textures;
  
	SDL_Texture** temp_scores_textures;
  
  SDL_Color score_color = { 0x00, 0x00, 0x00 };

	SDL_Rect score_position = { 0, header.h, SCREEN_WIDTH / 2, 30 };


  text_surface = TTF_RenderText_Solid(application->font, "< Back", back_color);

	if (text_surface == NULL) {
		fprintf(application->log_file, "Unable to render back surface! SDL_ttf Error: %s\n", TTF_GetError());

		return QUIT_SCREEN;
	}
	
	back_texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);

	SDL_FreeSurface(text_surface);

	if (back_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered back! SDL Error: %s\n", SDL_GetError());

		return QUIT_SCREEN;
	}
  

  text_surface = TTF_RenderText_Solid(application->font, "< Prev", navigation_color);

	if (text_surface == NULL) {
		fprintf(application->log_file, "Unable to render previous surface! SDL_ttf Error: %s\n", TTF_GetError());

		SDL_DestroyTexture(back_texture);

		return QUIT_SCREEN;
	}
	
	previous_texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);

	SDL_FreeSurface(text_surface);

	if (previous_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered previous! SDL Error: %s\n", SDL_GetError());
    
		SDL_DestroyTexture(back_texture);

		return QUIT_SCREEN;
	}


  text_surface = TTF_RenderText_Solid(application->font, "Next >", navigation_color);

	if (text_surface == NULL) {
		fprintf(application->log_file, "Unable to render next surface! SDL_ttf Error: %s\n", TTF_GetError());

		SDL_DestroyTexture(back_texture);
		SDL_DestroyTexture(previous_texture);

		return QUIT_SCREEN;
	}
	
	next_texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);

	SDL_FreeSurface(text_surface);

	if (next_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered next! SDL Error: %s\n", SDL_GetError());
    
		SDL_DestroyTexture(back_texture);
		SDL_DestroyTexture(previous_texture);

		return QUIT_SCREEN;
	}


  scores = square_read_scores(application);

  if (scores == NULL) {
		fprintf(application->log_file, "Unable to read scores\n");
    
		SDL_DestroyTexture(back_texture);
		SDL_DestroyTexture(next_texture);
		SDL_DestroyTexture(previous_texture);

		return QUIT_SCREEN;
	}


  scores_textures_size = 0;

  scores_textures = malloc(sizeof(*scores_textures));

  if (scores_textures == NULL) {
	  fprintf(application->log_file, "Unable to allocate memory for scores_textures\n");
    
    square_scores_cleanup(scores);

		SDL_DestroyTexture(back_texture);
		SDL_DestroyTexture(next_texture);
		SDL_DestroyTexture(previous_texture);
    
		return QUIT_SCREEN;
	}

  for (i = 0; i < scores->size; i++) {
    fprintf(application->log_file, "High score: %s\n", scores->items[i]);
    fflush(application->log_file);

    if (scores_textures_size > 0) {
      temp_scores_textures = realloc(scores_textures, (scores_textures_size + 1) * sizeof(*scores_textures));

      if (temp_scores_textures == NULL) {
        fprintf(application->log_file, "Unable to re-allocate memory for scores_textures\n");
        fflush(application->log_file);
        
        continue;
      }
      
      scores_textures = temp_scores_textures;
    }
    
    text_surface = TTF_RenderText_Solid(application->font, scores->items[i], score_color);
    
    if (text_surface == NULL) {
      fprintf(application->log_file, "Unable to render score surface! SDL_ttf Error: %s\n", TTF_GetError());
      fflush(application->log_file);
      
      continue;
    }
    
    scores_textures[scores_textures_size] = SDL_CreateTextureFromSurface(application->renderer, text_surface);
    
    SDL_FreeSurface(text_surface);
    
    if (scores_textures[scores_textures_size] == NULL) {
      fprintf(application->log_file, "Unable to create texture from rendered score! SDL Error: %s\n", SDL_GetError());
      fflush(application->log_file);

      continue;
    }
  
    scores_textures_size++;
	}

  
  square_scores_cleanup(scores);


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
				} else if (
          page_stop_index < (scores_textures_size - 1) &&
					next_position.x <= mouse_x &&
					(next_position.x + next_position.w) >= mouse_x &&
					next_position.y <= mouse_y &&
					(next_position.y + next_position.h) >= mouse_y
				) {
					page++;
				} else if (
          page > 1 &&
					previous_position.x <= mouse_x &&
					(previous_position.x + previous_position.w) >= mouse_x &&
					previous_position.y <= mouse_y &&
					(previous_position.y + previous_position.h) >= mouse_y
				) {
					page--;
				}

			}
    }

    SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(application->renderer);

    SDL_SetRenderDrawColor(application->renderer, 0x00, 0x00, 0x00, 0x00); 

		SDL_RenderFillRect(application->renderer, &header);

		SDL_RenderCopy(application->renderer, back_texture, NULL, &back_position);
    
    SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    page_start_index = (page - 1) * PAGE_SIZE;

    page_stop_index = page_start_index + (PAGE_SIZE - 1);

    for (i = page_start_index; i <= page_stop_index && i < scores_textures_size; i++) {
      score_position.y = header.h + 5 + ((i - page_start_index) * score_position.h);

      SDL_RenderCopy(application->renderer, scores_textures[i], NULL, &score_position);
    }
    
    if (page > 1) {
      SDL_RenderCopy(application->renderer, previous_texture, NULL, &previous_position);
    }

    if (page_stop_index < (scores_textures_size - 1)) {
		  SDL_RenderCopy(application->renderer, next_texture, NULL, &next_position);
    }


		SDL_RenderPresent(application->renderer);
  }


  SDL_DestroyTexture(back_texture);
  SDL_DestroyTexture(next_texture);
  SDL_DestroyTexture(previous_texture);

  for (i = 0; i < scores_textures_size; i++) {    
    SDL_DestroyTexture(scores_textures[i]);
  }

  free(scores_textures);

  return next_screen;
}
