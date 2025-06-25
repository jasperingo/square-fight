#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <SDL.h>
#include <SDL_ttf.h>
#include "application.h"
#include "score_recorder.h"

#define SHOOTER_WIDTH 20
#define SHOOTER_HEIGHT 20

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 10

#define MOVE_ACTORS_TIME_LIMIT 60

#define CREATE_ENEMY_ACTOR_TIME_LIMIT 600

#define ENEMY_X_POSITIONS_SIZE 32

void remove_actor(SDL_Rect** actors, Uint32 size, Uint32 index) {
    
	SDL_Rect* temp = malloc((size - 1) * sizeof(**actors));

	if (index != 0) {
		memmove((void *) temp, (void *) (*actors), index * sizeof(**actors)); // copy everything BEFORE the index
	}

	if (index != (size - 1)) {
		memmove((void *) (temp + index), (void *) ((*actors) + index + 1), (size - index - 1) * sizeof(**actors)); // copy everything AFTER the index
	}

	free(*actors);

	*actors = temp;
}

int game_screen(square_application* application) {
	Uint32 i, j;

	int selected = 0;

	int next_screen = HOME_SCREEN;

	Uint8 game_is_over = 0;
	
	Uint64 high_socre = 0;

	char text_content[50] = { '\0' };

	SDL_Color text_color = { 0xFF, 0xFF, 0xFF };

	SDL_Rect header = { 0, 0, SCREEN_WIDTH, 60 };

	SDL_Surface* score_surface;

	SDL_Texture* score_texture;

	SDL_Rect score_position = { SCREEN_WIDTH - (SCREEN_WIDTH / 3), 0, SCREEN_WIDTH / 3, 50 };

	SDL_Surface* back_surface;

	SDL_Texture* back_texture;

	SDL_Rect back_position = { 0, 0, SCREEN_WIDTH / 3, 50 };

	SDL_Surface* game_over_surface;

	SDL_Texture* game_over_texture;

	SDL_Color game_over_text_color = { 0x00, 0x00, 0x00 };

	SDL_Rect game_over_position = { (SCREEN_WIDTH / 2) - ((SCREEN_WIDTH / 4) / 2), SCREEN_HEIGHT / 4, SCREEN_WIDTH / 4, 50 };

	SDL_Event event;

	int mouse_x = 0;

	int mouse_y = 0;
	
	Uint32 move_actors_start_time = 0;

	Uint32 move_actors_current_time = 0;
	
	SDL_Rect* bullet_actors = NULL;

	SDL_Rect* temp_bullet_actors = NULL;
	
	Uint32 bullet_actors_size = 0;

	SDL_Rect shooter_actor = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - (SHOOTER_HEIGHT * 2), SHOOTER_WIDTH, SHOOTER_HEIGHT };

	int selected_enemy_actor;

	unsigned int enemy_actor_x_positions[ENEMY_X_POSITIONS_SIZE] = { 
		0, 20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 
		320, 340, 360, 380, 400, 420, 440, 460, 480, 500, 520, 540, 560, 580, 600, 620,
	};

	Uint8 enemy_killed = 0;

	SDL_Rect* enemy_actors = NULL;

	SDL_Rect* temp_enemy_actors = NULL;
	
	Uint32 enemy_actors_size = 0;

	Uint32 create_enemy_actor_start_time = 0;

	Uint32 create_enemy_actor_current_time = 0;


	srand(time(NULL));

	bullet_actors = malloc(sizeof(*bullet_actors));

	if (bullet_actors == NULL) {
		fprintf(application->log_file, "Error allocating bullet_actors");

		return QUIT_SCREEN;
	}

	enemy_actors = malloc(sizeof(*enemy_actors));

	if (enemy_actors == NULL) {
		fprintf(application->log_file, "Error allocating enemy_actors");

		free(bullet_actors);

		return QUIT_SCREEN;
	}


	back_surface = TTF_RenderText_Solid(application->font, "< Back", text_color);

	if (back_surface == NULL) {
		fprintf(application->log_file, "Unable to render back surface! SDL_ttf Error: %s\n", TTF_GetError());

		free(bullet_actors);
		free(enemy_actors);

		return QUIT_SCREEN;
	}
	
	back_texture = SDL_CreateTextureFromSurface(application->renderer, back_surface);

	SDL_FreeSurface(back_surface);

	if (back_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered back! SDL Error: %s\n", SDL_GetError());

		free(bullet_actors);
		free(enemy_actors);

		return QUIT_SCREEN;
	}


	game_over_surface = TTF_RenderText_Solid(application->font, "Game Over!!!", game_over_text_color);

	if (game_over_surface == NULL) {
		fprintf(application->log_file, "Unable to render game over surface! SDL_ttf Error: %s\n", TTF_GetError());

		free(bullet_actors);
		free(enemy_actors);
		
		SDL_DestroyTexture(back_texture);

		return QUIT_SCREEN;
	}
	
	game_over_texture = SDL_CreateTextureFromSurface(application->renderer, game_over_surface);

	SDL_FreeSurface(game_over_surface);

	if (game_over_texture == NULL) {
		fprintf(application->log_file, "Unable to create texture from rendered game over! SDL Error: %s\n", SDL_GetError());

		free(bullet_actors);
		free(enemy_actors);
		
		SDL_DestroyTexture(back_texture);

		return QUIT_SCREEN;
	}


	move_actors_start_time = SDL_GetTicks();

	create_enemy_actor_start_time = SDL_GetTicks();

	
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

			} else if (event.type == SDL_KEYDOWN) {

				if (game_is_over == 1) {
					continue;
				}

				switch (event.key.keysym.sym) {
					case SDLK_UP:
						shooter_actor.y -= 4;

						if (shooter_actor.y < header.h) {
							shooter_actor.y = header.h;
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
						temp_bullet_actors = realloc(bullet_actors, (bullet_actors_size + 1) * sizeof(*bullet_actors));

						if (temp_bullet_actors == NULL) {
							fprintf(application->log_file, "Error re-allocating bullet_actors");
					
							break;
						}

						bullet_actors = temp_bullet_actors;
			
						bullet_actors[bullet_actors_size].y = shooter_actor.y;
						bullet_actors[bullet_actors_size].x = shooter_actor.x + ((SHOOTER_WIDTH / 2) / 2);
						bullet_actors[bullet_actors_size].w = BULLET_WIDTH;
						bullet_actors[bullet_actors_size].h = BULLET_HEIGHT;
			
						bullet_actors_size++;
						break;

					default:
						break;
				}
			}
		}

		if (game_is_over == 1) {
			continue;
		}


		create_enemy_actor_current_time = SDL_GetTicks();

		if ((create_enemy_actor_current_time - create_enemy_actor_start_time) >= CREATE_ENEMY_ACTOR_TIME_LIMIT) {
			temp_enemy_actors = realloc(enemy_actors, (enemy_actors_size + 1) * sizeof(*enemy_actors));

			if (temp_enemy_actors == NULL) {
				fprintf(application->log_file, "Error re-allocating enemy_actors");
			} else {
				enemy_actors = temp_enemy_actors;

				selected_enemy_actor = rand() % (ENEMY_X_POSITIONS_SIZE - 0) + 0;

				enemy_actors[enemy_actors_size].y = header.h;
				enemy_actors[enemy_actors_size].w = SHOOTER_WIDTH;
				enemy_actors[enemy_actors_size].h = SHOOTER_HEIGHT;
				enemy_actors[enemy_actors_size].x = enemy_actor_x_positions[selected_enemy_actor];

				enemy_actors_size++;

				// fprintf(application->log_file, "Enemy selected: %d & Index: %d\n", enemy_actors[enemy_actors_size - 1].x, selected_enemy_actor);
				// fflush(application->log_file);
			}
		}

		if ((create_enemy_actor_current_time - create_enemy_actor_start_time) >= CREATE_ENEMY_ACTOR_TIME_LIMIT) {
			create_enemy_actor_start_time = create_enemy_actor_current_time;
		}


		SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		SDL_RenderClear(application->renderer);
		
		SDL_SetRenderDrawColor(application->renderer, 0x00, 0xFF, 0x00, 0xFF); 


		move_actors_current_time = SDL_GetTicks();


		for (i = 0; i < bullet_actors_size; i++) {
			SDL_RenderFillRect(application->renderer, &bullet_actors[i]);

			if ((move_actors_current_time - move_actors_start_time) >= MOVE_ACTORS_TIME_LIMIT) {

				if (bullet_actors[i].y <= 0) {
					remove_actor(&bullet_actors, bullet_actors_size, i);

					bullet_actors_size--;
				} else {
					bullet_actors[i].y -= 4;
				}
			}
		}


		SDL_SetRenderDrawColor(application->renderer, 0xFF, 0xAB, 0x1D, 0xFF);   

		for (i = 0; i < enemy_actors_size; i++) {
			SDL_RenderFillRect(application->renderer, &enemy_actors[i]);

			enemy_killed = 0;

			if ((move_actors_current_time - move_actors_start_time) >= MOVE_ACTORS_TIME_LIMIT) {

				for (j = 0; j < bullet_actors_size; j++) {
					if (
						enemy_actors[i].x < (bullet_actors[j].x + bullet_actors[j].w) &&
						(enemy_actors[i].x + enemy_actors[i].w) > bullet_actors[j].x &&
						enemy_actors[i].y < (bullet_actors[j].y + bullet_actors[j].h) &&
						(enemy_actors[i].y + enemy_actors[i].h) > bullet_actors[j].y
					) {
						enemy_killed = 1;

						high_socre++;

						remove_actor(&enemy_actors, enemy_actors_size, i);
						enemy_actors_size--;

						remove_actor(&bullet_actors, bullet_actors_size, j);
						bullet_actors_size--;
					}
				}

				if (enemy_killed == 0) {
					if (enemy_actors[i].y >= SCREEN_HEIGHT) {
						remove_actor(&enemy_actors, enemy_actors_size, i);

						enemy_actors_size--;
					} else {
						enemy_actors[i].y += 4;
					}
				}
			}

			if (
				enemy_killed == 0 &&
				enemy_actors[i].x < (shooter_actor.x + shooter_actor.w) &&
				(enemy_actors[i].x + enemy_actors[i].w) > shooter_actor.x &&
				enemy_actors[i].y < (shooter_actor.y + shooter_actor.h) &&
				(enemy_actors[i].y + enemy_actors[i].h) > shooter_actor.y
			) {
				game_is_over = 1;
			}
		}

		if ((move_actors_current_time - move_actors_start_time) >= MOVE_ACTORS_TIME_LIMIT) {
			move_actors_start_time = move_actors_current_time;
		}

		SDL_SetRenderDrawColor(application->renderer, 0xFF, 0x00, 0x00, 0xFF);        
		
		SDL_RenderFillRect(application->renderer, &shooter_actor);
		
		SDL_SetRenderDrawColor(application->renderer, 0x00, 0x00, 0x00, 0x00);   

		SDL_RenderFillRect(application->renderer, &header);
		

		sprintf(text_content, "Score: %d", high_socre);

		score_surface = TTF_RenderText_Solid(application->font, text_content, text_color);

		if (score_surface == NULL) {
			fprintf(application->log_file, "Unable to render score surface! SDL_ttf Error: %s\n", TTF_GetError());
	
			next_screen = QUIT_SCREEN;

			break;
		}
		
		score_texture = SDL_CreateTextureFromSurface(application->renderer, score_surface);
	
		
		SDL_FreeSurface(score_surface);
	
		if (score_texture == NULL) {
			fprintf(application->log_file, "Unable to create texture from rendered score! SDL Error: %s\n", SDL_GetError());
	
			next_screen = QUIT_SCREEN;

			break;
		}
		
		SDL_RenderCopy(application->renderer, score_texture, NULL, &score_position);

		
		SDL_RenderCopy(application->renderer, back_texture, NULL, &back_position);

		if (game_is_over == 1) {
			SDL_RenderCopy(application->renderer, game_over_texture, NULL, &game_over_position);
			
			square_record_score(application, high_socre);
		}


		SDL_RenderPresent(application->renderer);
	}


	free(bullet_actors);
	free(enemy_actors);

	SDL_DestroyTexture(score_texture);
	SDL_DestroyTexture(back_texture);
	SDL_DestroyTexture(game_over_texture);

	return next_screen;
}
