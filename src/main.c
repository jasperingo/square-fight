#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int SHOOTER_WIDTH = 20;
const int SHOOTER_HEIGHT = 20;

const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 10;


int main (int argc, char* args[]) {
	Uint32 i;
	
	FILE* log_file;

	SDL_Window* window = NULL;

	TTF_Font* font = NULL;

	SDL_Renderer* renderer = NULL;

	char text_content[50] = { '\0' };

	SDL_Color text_color = { 0, 0, 0 };

	SDL_Surface* text_surface;

	SDL_Texture* text_texture;

	SDL_Rect text_position = { SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, 100 };

	SDL_Event event; 
				
	int should_quit = 0;

	Uint32 start_time = 0;

	Uint32 current_time = 0;
	
	SDL_Rect* bullet_actors = NULL;
	
	Uint32 bullet_actors_size = 0;

	int bullet_actors_created = 0;

	SDL_Rect shooter_actor = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SHOOTER_WIDTH, SHOOTER_HEIGHT };


	log_file = fopen("/logs/logfile.txt", "w");

	if (log_file == NULL) {
		printf("Error opening log file for writing");

		return 1;
	}

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());

		return 1;
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			
		return 1;
	}

	window = SDL_CreateWindow("RockShooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		
		return 1;
	} 

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

		return 1;
	}

	font = TTF_OpenFont("resources/ARIAL.TTF", 24);

	if (font == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());

		return 1;
	}

	bullet_actors = malloc(sizeof(*bullet_actors));

	if (bullet_actors == NULL) {
		printf("Error allocating bullet_actors");

		return 1;
	}

	start_time = SDL_GetTicks();

	
	while (should_quit == 0) { 

		while (SDL_PollEvent(&event) != 0) { 

			if (event.type == SDL_QUIT) {
				should_quit = 1;
			} else if (event.type == SDL_KEYDOWN) {
				
				switch (event.key.keysym.sym) {
					case SDLK_UP:
						shooter_actor.y -= 4;

						if (shooter_actor.y < 0) {
							shooter_actor.y = 0;
						}
						break;

					case SDLK_DOWN:
						shooter_actor.y += 4;

						if ((shooter_actor.y + SHOOTER_HEIGHT) > SCREEN_HEIGHT) {
							shooter_actor.y = SCREEN_HEIGHT - SHOOTER_HEIGHT;
						}
						break;

					case SDLK_LEFT:
						shooter_actor.x -= 4;

						if (shooter_actor.x < 0) {
							shooter_actor.x = 0;
						}
						break;

					case SDLK_RIGHT:
						shooter_actor.x += 4;

						if ((shooter_actor.x + SHOOTER_WIDTH) > SCREEN_WIDTH) {
							shooter_actor.x = SCREEN_WIDTH - SHOOTER_WIDTH;
						}
						break;

					case SDLK_SPACE:
						bullet_actors = realloc(bullet_actors, (bullet_actors_size + 1) * sizeof(*bullet_actors));

						if (bullet_actors == NULL) {
							printf("Error re-allocating bullet_actors");
					
							break;
						}
			
						bullet_actors[bullet_actors_size].y = shooter_actor.y;
						bullet_actors[bullet_actors_size].x = shooter_actor.x + ((SHOOTER_WIDTH / 2) / 2);
						bullet_actors[bullet_actors_size].w = BULLET_WIDTH;
						bullet_actors[bullet_actors_size].h = BULLET_HEIGHT;
			
						bullet_actors_size++;
			
						bullet_actors_created++;
						break;

					default:
						break;
				}
			}
		}


		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); 

		current_time = SDL_GetTicks();

		// fprintf(fptr, "Start time: %d\n", start_time);
		// fprintf(fptr, "Current time: %d\n", current_time);
		// fprintf(fptr, "Diff time: %d\n", current_time - start_time);


		for (i = 0; i < bullet_actors_size; i++) {
			SDL_RenderFillRect(renderer, &bullet_actors[i]);

			if ((current_time - start_time) >= 60) {

				if (bullet_actors[i].y <= 0) {
					bullet_actors_size--;

					memset((void *) bullet_actors, 0, sizeof(*bullet_actors));

					memmove((void *) bullet_actors, (void *) (bullet_actors + 1), bullet_actors_size * sizeof(*bullet_actors));
				} else {
					bullet_actors[i].y -= 4;
				}
			}
		}

		if ((current_time - start_time) >= 60) {
			start_time = current_time;
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);        
		
		SDL_RenderFillRect(renderer, &shooter_actor);
		

		sprintf(text_content, "Bullets: %d", bullet_actors_created);

		text_surface = TTF_RenderText_Solid(font, text_content, text_color);

		if (text_surface == NULL) {
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	
			break;
		}
		
		text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	
		
		SDL_FreeSurface(text_surface);
	
		if (text_texture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	
			break;
		}
		
		SDL_RenderCopy(renderer, text_texture, NULL, &text_position);

		SDL_RenderPresent(renderer);
	}

	free(bullet_actors);

	fclose(log_file);

	TTF_CloseFont(font);

	SDL_DestroyTexture(text_texture);

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	TTF_Quit();

	SDL_Quit();

	return 0;
}
