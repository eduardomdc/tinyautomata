#ifndef CONWAY_HPP
#define CONWAY_HPP

#include <SDL2/SDL.h>
#include <iostream>

class Conway {
public:
	Conway();
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event currentEvent;
	void init();
	void render();
	void clean();
	void getInput();
};

#endif
