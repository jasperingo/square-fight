#include "main_menu.h"

square_main_menu* square_main_menu_init(square_application* application) {
	Uint8 i;

	SDL_Surface* text_surface = NULL;

	SDL_Color text_color = { 0, 0, 0 };

	char* contents[MENU_SIZE] = { "Play", "Scores", "Quit" };

	int screens[MENU_SIZE] = { GAME_SCREEN, SCORES_SCREEN, QUIT_SCREEN };

	SDL_Rect positions[MENU_SIZE] = {
		{ SCREEN_WIDTH - MENU_ITEM_WIDTH - 20, SCREEN_HEIGHT - (MENU_ITEM_HEIGHT * 3) - 30, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT },
		{ SCREEN_WIDTH - MENU_ITEM_WIDTH - 20, SCREEN_HEIGHT - (MENU_ITEM_HEIGHT * 2) - 20, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT },
		{ SCREEN_WIDTH - MENU_ITEM_WIDTH - 20, SCREEN_HEIGHT - (MENU_ITEM_HEIGHT * 1) - 10, MENU_ITEM_WIDTH, MENU_ITEM_HEIGHT },
	};

	square_main_menu* main_menu = malloc(MENU_SIZE * sizeof(*main_menu));

	if (main_menu == NULL) {
		fprintf(application->log_file, "Failed to allocate memory for main menu\n");

		return NULL;
	}

	main_menu->size = 0;

	for (i = 0; i < MENU_SIZE; i++) {
		text_surface = TTF_RenderText_Solid(application->font, contents[i], text_color);

		if (text_surface == NULL) {
			square_main_menu_cleanup(main_menu);

			fprintf(application->log_file, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	
			return NULL;
		}
		
		main_menu->items[i].texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);
	
		SDL_FreeSurface(text_surface);
	
		if (main_menu->items[i].texture == NULL) {
			square_main_menu_cleanup(main_menu);

			fprintf(application->log_file, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	
			return NULL;
		}

		main_menu->items[i].screen = screens[i];

		main_menu->items[i].position = positions[i];

		main_menu->size++;
	}

	return main_menu;
}

void square_main_menu_cleanup(square_main_menu* main_menu) {
	Uint8 i;

	for (i = 0; i < main_menu->size; i++) {
		SDL_DestroyTexture(main_menu->items[i].texture);
	}

	free(main_menu);
}
