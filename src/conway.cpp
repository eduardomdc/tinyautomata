#include "conway.hpp"
#include "bitmap.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>
#include <vector>

Conway::Conway(int size){
	this->size = size;
	this->currentTable = new std::vector<std::vector<int>>();
	this->lastTable = new std::vector<std::vector<int>>();
	makeTable(this->currentTable, this->size);
	makeTable(this->lastTable, this->size);
	randomizeTable(this->currentTable);
	printTable(this->currentTable);
}

void Conway::init(){
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		this->window = SDL_CreateWindow("conway's",0,0,this->size*32,this->size*32,SDL_WINDOW_BORDERLESS);
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
	this->renderTable();
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

void Conway::renderTable(){
	for (int i=0; i < this->size*32; i++){
		for (int j=0; j<this->size; j++){
			int n = (*this->currentTable)[i][j];
			for (int k=0; k<32; k++){
				if (n & 1) SDL_RenderDrawPoint(this->renderer, j*32+k, i);
				n = n >> 1;
			}
		}
	}
}
