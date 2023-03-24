#ifndef CONWAY
#define CONWAY

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class Conway {
public:
	Conway(int size);
	bool exit = false;
	void init();
	void render();
	void clean();
	void getInput();
	void renderTable();
	int size; // size*32 width and height amount cells
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event currentEvent;
	std::vector<std::vector<int>>* currentTable;
	std::vector<std::vector<int>>* lastTable;
};

#endif
