#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "application.h"
#include "main_menu.h"

int main (int argc, char* args[]) {
	int screen = HOME_SCREEN;
	
	square_application* application = NULL;

	application = square_application_init();

	if (application == NULL) {
		return 1;
	}

	while (screen != QUIT_SCREEN) { 

		switch (screen) {
			case HOME_SCREEN:
				screen = home_screen(application);
				break;
			case GAME_SCREEN:
				screen = game_screen(application);
				break;
			case SCORES_SCREEN:
				screen = scores_screen(application);
				break;
			default:
				screen = QUIT_SCREEN;
		}

	}

	square_application_cleanup(application);

	return 0;
}
