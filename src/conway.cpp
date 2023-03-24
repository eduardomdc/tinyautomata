#include "conway.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <vector>

Conway::Conway(){
	this->size = 2;
	currentTable = new std::vector<std::vector<int>>();
	lastTable = new std::vector<std::vector<int>>();
}

void Conway::init(){
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		this->window = SDL_CreateWindow("conway's",0,0,100,100,SDL_WINDOW_BORDERLESS);
		this->renderer = SDL_CreateRenderer(this->window, -1, 0);
		if (this->renderer){
			SDL_SetRenderDrawColor(this->renderer, 0,0,0,255);
			std::cout << "Renderer Created!"<<std::endl;
		}

	}
}

void Conway::render(){
	SDL_RenderClear(this->renderer);
	SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
	SDL_RenderDrawPoint(this->renderer, 50, 50);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_RenderPresent(this->renderer);
}

void Conway::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Cleaned" << std::endl;
}

void Conway::getInput(){
	while(SDL_PollEvent(&this->currentEvent)){
		switch(this->currentEvent.type){
		case SDL_QUIT:
			this->clean();
			this->exit = true;
			return;
		}
	}
}
