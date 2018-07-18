#-mwindows

all:
	 g++ src/*.cpp src/*.c -o Comms.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -static-libgcc -static-libstdc++ -mwindows
