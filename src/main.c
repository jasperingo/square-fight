#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "application.h"
#include "main_menu.h"

const int SHOOTER_WIDTH = 20;
const int SHOOTER_HEIGHT = 20;

const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 10;


int main(int argc, char* args[]) {
	Uint8 i;

	int should_quit = 0;
	
	square_application* application = NULL;

	square_main_menu* main_menu = NULL;

	SDL_Color text_color = { 0, 0, 0 };

	SDL_Event event; 

	SDL_Rect red_square = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 200, 200 };

	SDL_Rect blue_square = { (SCREEN_WIDTH / 4) + 150, SCREEN_HEIGHT / 4, 150, 150 };


	application = square_application_init();

	if (application == NULL) {
		return 1;
	}

	main_menu = square_main_menu_init(application);

	if (main_menu == NULL) {
		return 1;
	}

	SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_RenderClear(application->renderer);

	SDL_SetRenderDrawColor(application->renderer, 0x00, 0x00, 0xFF, 0xFF);        
		
	SDL_RenderFillRect(application->renderer, &blue_square);

	SDL_SetRenderDrawColor(application->renderer, 0xFF, 0x00, 0x00, 0xFF);        
		
	SDL_RenderFillRect(application->renderer, &red_square);

	for (i = 0; i < main_menu->size; i++) {
		SDL_RenderCopy(application->renderer, main_menu->items[i].texture, NULL, &main_menu->items[i].position);
	}

	SDL_RenderPresent(application->renderer);

	
	while (should_quit == 0) { 

		while (SDL_PollEvent(&event) != 0) { 

			if (event.type == SDL_QUIT) {
				should_quit = 1;
			} else if (event.type == SDL_KEYDOWN) {
				
				switch (event.key.keysym.sym) {
					case SDLK_UP:
						
						break;

					case SDLK_DOWN:
						
						break;

					case SDLK_LEFT:
						
						break;

					case SDLK_RIGHT:
						
						break;

					case SDLK_SPACE:

						break;

					default:
						break;
				}
			}
		}


		// SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		// SDL_RenderClear(application->renderer);
		
		// SDL_SetRenderDrawColor(application->renderer, 0x00, 0xFF, 0x00, 0xFF); 
	

		// sprintf(text_content, "Bullets: %d", 1);

		// text_surface = TTF_RenderText_Solid(application->font, text_content, text_color);

		// if (text_surface == NULL) {
		// 	fprintf(application->log_file, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	
		// 	break;
		// }
		
		// text_texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);
	
		
		// SDL_FreeSurface(text_surface);
	
		// if (text_texture == NULL) {
		// 	fprintf(application->log_file, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	
		// 	break;
		// }
		
		// SDL_RenderCopy(application->renderer, text_texture, NULL, &text_position);

		// SDL_RenderPresent(application->renderer);
	}

	square_main_menu_cleanup(main_menu, application);

	square_application_cleanup(application);


	return 0;
}
