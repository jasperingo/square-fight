#ifndef __SQUARE_MAIN_MENU_H__
#define __SQUARE_MAIN_MENU_H__
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "application.h"

#define MENU_SIZE 3
#define MENU_ITEM_WIDTH 100
#define MENU_ITEM_HEIGHT 30

typedef struct {
	SDL_Texture* texture;
  SDL_Rect position;
} square_main_menu_item;

typedef struct {
  Uint16 size;
  square_main_menu_item items[MENU_SIZE];
} square_main_menu;

square_main_menu* square_main_menu_init(square_application* application);

void square_main_menu_cleanup(square_main_menu* main_menu, square_application* application);

#endif
