
#OBJS specifies which files to compile as part of the project
OBJS = ./src/main.c ./src/application.c ./src/main_menu.c ./src/score_recorder.c ./src/home_screen.c ./src/game_screen.c ./src/scores_screen.c

#CC specifies which compiler we're using
CC = gcc

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I./includes -I./libs/SDL2-2.30.3/include/SDL2 -I./libs/SDL2_ttf-2.24.0/include/SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L./libs/SDL2-2.30.3/lib -L./libs/SDL2_ttf-2.24.0/lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -w -Wl,-subsystem,windows

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./build/SquareFight

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
