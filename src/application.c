#include "application.h"

square_application* square_application_init(void) {
	square_application* application = malloc(sizeof(*application));

	if (application == NULL) {
		return NULL;
	}

	application->log_file = NULL;
	application->renderer = NULL;
	application->window = NULL;
	application->font = NULL;

	application->log_file = fopen("logs/logfile.txt", "w");

	if (application->log_file == NULL) {
		square_application_cleanup(application);

		return NULL;
	}

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		fprintf(application->log_file, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		
		square_application_cleanup(application);

		return NULL;
	}

	if (TTF_Init() == -1) {
		fprintf(application->log_file, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		
		square_application_cleanup(application);
			
		return NULL;
	}

	application->window = SDL_CreateWindow("SquareFight", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (application->window == NULL) {
		fprintf(application->log_file, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		square_application_cleanup(application);
		
		return NULL;
	} 

	application->renderer = SDL_CreateRenderer(application->window, -1, SDL_RENDERER_ACCELERATED);

	if (application->renderer == NULL) {
		fprintf(application->log_file, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
		
		square_application_cleanup(application);

		return NULL;
	}

	application->font = TTF_OpenFont("resources/ARIAL.TTF", 24);

	if (application->font == NULL) {
		fprintf(application->log_file, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		
		square_application_cleanup(application);

		return NULL;
	}

	return application;
}

void square_application_cleanup(square_application* application) {
	if (application != NULL) {

		if (application->font != NULL) {
			TTF_CloseFont(application->font);
		}
	
		if (application->renderer != NULL) {
			SDL_DestroyRenderer(application->renderer);
		}
	
		if (application->window != NULL) {
			SDL_DestroyWindow(application->window);
		}

		if (application->log_file != NULL) {
			fclose(application->log_file);
		}
	
		TTF_Quit();
	
		SDL_Quit();
		
		free(application);
	}
}
