# SquareFight

An SDL based game of a squares shooting squares at other squares 🟥❎🟨🚀😎

## Build

`gcc ./src/main.c ./src/application.c ./src/main_menu.c ./src/home_screen.c ./src/game_screen.c ./src/scores_screen.c -I./includes -I./libs/SDL2-2.30.3/include/SDL2 -I./libs/SDL2_ttf-2.24.0/include/SDL2 -L./libs/SDL2-2.30.3/lib -L./libs/SDL2_ttf-2.24.0/lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -o ./build/SquareFight`

## Run

`.\build\SquareFight.exe`
