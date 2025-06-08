#include "application.h"
#include "main_menu.h"

int home_screen(square_application* application) {
  Uint8 i;

	int selected = 0;

	int next_screen = QUIT_SCREEN;

	int mouse_x = 0;

	int mouse_y = 0;

	square_main_menu* main_menu = NULL;

	SDL_Color text_color = { 0, 0, 0 };

	SDL_Surface* title_surface = NULL;

	SDL_Texture* title_texture = NULL;

	SDL_Rect title_position = { 10, 10, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8 };

	SDL_Event event; 

	SDL_Rect red_square = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 200, 200 };

	SDL_Rect blue_square = { (SCREEN_WIDTH / 4) + 150, (SCREEN_HEIGHT / 4) - 25, 150, 150 };


	main_menu = square_main_menu_init(application);

	if (main_menu == NULL) {
		square_application_cleanup(application);

		return 1;
	}

	title_surface = TTF_RenderText_Solid(application->font, APPLICATION_NAME, text_color);

	if (title_surface == NULL) {
		square_main_menu_cleanup(main_menu);

		square_application_cleanup(application);

		fprintf(application->log_file, "Error creating title text surface: %s\n", TTF_GetError());

		return 1;
	}
	
	title_texture = SDL_CreateTextureFromSurface(application->renderer, title_surface);

	SDL_FreeSurface(title_surface);

	if (title_texture == NULL) {
		square_main_menu_cleanup(main_menu);

		square_application_cleanup(application);

		fprintf(application->log_file, "Error creating title texture: %s\n", SDL_GetError());

		return 1;
	}

	SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	SDL_RenderClear(application->renderer);

	SDL_SetRenderDrawColor(application->renderer, 0x00, 0x00, 0xFF, 0xFF);     

	SDL_RenderCopy(application->renderer, title_texture, NULL, &title_position);
	
	SDL_RenderFillRect(application->renderer, &blue_square);

	SDL_SetRenderDrawColor(application->renderer, 0xFF, 0x00, 0x00, 0xFF);        
		
	SDL_RenderFillRect(application->renderer, &red_square);

	for (i = 0; i < main_menu->size; i++) {
		SDL_RenderCopy(application->renderer, main_menu->items[i].texture, NULL, &main_menu->items[i].position);
	}

	SDL_RenderPresent(application->renderer);

	while (selected == 0) { 

		while (SDL_PollEvent(&event) != 0) { 

			if (event.type == SDL_QUIT) {
				selected = 1;
			} else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mouse_x, &mouse_y);

				for (i = 0; i < main_menu->size; i++) {
					if (
						main_menu->items[i].position.x <= mouse_x &&
						(main_menu->items[i].position.x + main_menu->items[i].position.w) >= mouse_x &&
						main_menu->items[i].position.y <= mouse_y &&
						(main_menu->items[i].position.y + main_menu->items[i].position.h) >= mouse_y
					) {
            selected = 1;
            next_screen = main_menu->items[i].screen;
						break;
					}
				}
			}

		}

	}

	square_main_menu_cleanup(main_menu);

  return next_screen;
}
