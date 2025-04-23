#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <application.h>

const int SHOOTER_WIDTH = 20;
const int SHOOTER_HEIGHT = 20;

const int BULLET_WIDTH = 10;
const int BULLET_HEIGHT = 10;


int game(square_application* application) {
	Uint32 i;

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

	bullet_actors = malloc(sizeof(*bullet_actors));

	if (bullet_actors == NULL) {
		fprintf(application->log_file, "Error allocating bullet_actors");

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
							fprintf(application->log_file, "Error re-allocating bullet_actors");
					
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


		SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(application->renderer);
		
		SDL_SetRenderDrawColor(application->renderer, 0x00, 0xFF, 0x00, 0xFF); 

		current_time = SDL_GetTicks();


		for (i = 0; i < bullet_actors_size; i++) {
			SDL_RenderFillRect(application->renderer, &bullet_actors[i]);

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

		SDL_SetRenderDrawColor(application->renderer, 0xFF, 0x00, 0x00, 0xFF);        
		
		SDL_RenderFillRect(application->renderer, &shooter_actor);
		

		sprintf(text_content, "Bullets: %d", bullet_actors_created);

		text_surface = TTF_RenderText_Solid(application->font, text_content, text_color);

		if (text_surface == NULL) {
			fprintf(application->log_file, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	
			break;
		}
		
		text_texture = SDL_CreateTextureFromSurface(application->renderer, text_surface);
	
		
		SDL_FreeSurface(text_surface);
	
		if (text_texture == NULL) {
			fprintf(application->log_file, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	
			break;
		}
		
		SDL_RenderCopy(application->renderer, text_texture, NULL, &text_position);

		SDL_RenderPresent(application->renderer);
	}


	free(bullet_actors);

	SDL_DestroyTexture(text_texture);

	return should_quit;
}
