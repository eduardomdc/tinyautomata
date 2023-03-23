#include "conway.hpp"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdlib>

Conway::Conway(){
	
}

void Conway::init(){
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		this->window = SDL_CreateWindow("conway's",0,0,100,100,SDL_WINDOW_BORDERLESS);
		this->renderer = SDL_CreateRenderer(this->window, -1, 0);
		if (this->renderer){
			SDL_SetRenderDrawColor(this->renderer, 0,255,0,255);
			std::cout << "Renderer Created!"<<std::endl;
		}

	}
}

void Conway::render(){
	SDL_RenderClear(this->renderer);
	SDL_RenderPresent(this->renderer);
}

void Conway::clean(){
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
	SDL_Quit();
	std::cout << "Cleaned" << std::endl;
}

void Conway::getInput(){
	
}
