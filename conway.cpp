#include "conway.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>
#include <random>



/*
 *      mapping of char bits
 *      u - unused
 *      n - neighbors count b0000 to b1000(8)
 *      a - alive or dead
 *      char(8bits)-> |uuunnnna|
 *
 * */

void grow(char* cell){
   *cell |= 1; 
}

void kill(char* cell){
    *cell &= ~1;
}

bool isAlive(char* cell){
    return (*cell)&1;
}

void addNeighbor(char* cell){
    *cell += (char)2; // + b|0000|0010
}

void removeNeighbor(char* cell){
    *cell -= (char)2;
}

void setNeighbors(char* cell, int neighbors){
    *cell += (char)(neighbors<<1);
}

char getNeighbors(char* cell){
    return (*cell)>>1;
}

void Conway::randomizeTable(){ 
    for (int i=1;i<size+1;i++){
        for (int j=1;j<size+1;j++){
            if(rand()%2 == 0){
                (*this->table)[i][j] = 1;
            }
            else (*this->table)[i][j] = 0;
        } 
    }
}

void Conway::addNeighbors(pos position){
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (line==0 && col==0) continue;
            int wrapX = (*wrapIndex)[line+position.x];
            int wrapY = (*wrapIndex)[col+position.y];
            addNeighbor(&(*this->table)[wrapX][wrapY]); 
        }
    }
}

void Conway::subNeighbors(pos position){
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (line==0 && col==0) continue;
            int wrapX = (*wrapIndex)[line+position.x];
            int wrapY = (*wrapIndex)[col+position.y];
            removeNeighbor(&(*this->table)[wrapX][wrapY]); 
        }
    }
}

char Conway::countNeighbors(pos position){
    char neighbors = 0;
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (!(line==0 && col==0)){
                int wrapX = (*wrapIndex)[line+position.x];
                int wrapY = (*wrapIndex)[col+position.y];
                if (isAlive(&(*this->table)[wrapX][wrapY])) neighbors++;
            }
        }
    }
    return neighbors;
}


/* PPPPP
 * PCCCP
 * PCCCP
 * PCCCP
 * PPPPP
 *  012
 *  123
 *  padding is not nescessary anymore
 * */

Conway::Conway(int size){
    this->size = size;
    this->exit = false;
    this->table = new std::vector<std::vector<char>>();
    this->lastTable = new std::vector<std::vector<char>>();
    this->wrapIndex = new std::vector<int>(size+2);
    //wrapping look up table
    (*this->wrapIndex)[0] = size;
    for (int i=1; i<size+1; i++){
        (*this->wrapIndex)[i] = i;
    }
    (*wrapIndex)[size+1] = 1;
    //allocate table with padding!!
    for (int i=0;i<size+2;i++){
        this->table->push_back(std::vector<char>(size+2));
    }
    this->randomizeTable();  
    //first time setup neighbors counts
    for (int i=1;i<size+1;i++){
        for(int j=1;j<size+1;j++){
            int neighbors = countNeighbors({i,j});
            setNeighbors(&(*this->table)[i][j], neighbors);
        }
    }
    //copy table over to lastTable
    this->lastTable->assign(this->table->begin(), this->table->end());
    std::cout<<"That's life."<<std::endl;
}

void Conway::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        this->window = SDL_CreateWindow("conway's game",
                0,
                0,
                this->size,
                this->size,
                SDL_WINDOW_BORDERLESS
                );
        this->renderer = SDL_CreateRenderer(this->window, -1, 0);
    }
    else return;
    if (this->renderer){
        SDL_SetRenderDrawColor(this->renderer, 0,0,0,255);
        std::cout << "Renderer Created!"<<std::endl;
    }
}

void Conway::closeSDL(){
    SDL_DestroyWindow(this->window);
    SDL_DestroyRenderer(this->renderer);
    SDL_Quit();
    std::cout << "SDL cleaned out." << std::endl;
}

void Conway::getInput(){
    while(SDL_PollEvent(&this->currentEvent)){
        switch(this->currentEvent.type){
        case SDL_QUIT:
            this->closeSDL();
            this->exit = true;
            return;
        }
    }
}

void Conway::copyTable(){
    this->lastTable->assign(this->table->begin(),this->table->end());
}

void Conway::render(){ 
    SDL_RenderClear(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
    for (int i=0;i<this->size+2;i++){
        for (int j=0;j<this->size+2;j++){
            char* cell = &(*this->table)[i][j];
            if (isAlive(cell)) SDL_RenderDrawPoint(this->renderer, i-1, j-1);
            else {
                char* lastCell = &(*this->lastTable)[i][j];
                if (isAlive(lastCell)){
                    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
                    SDL_RenderDrawPoint(this->renderer, i-1, j-1);
                    SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
                }
            }
        }
    }
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer); 
}

void Conway::update(){ 
    for (int i=1;i<this->size+1;i++){
        for (int j=1;j<this->size+1;j++){
            char* lastCell = &(*this->lastTable)[i][j];
            char* cell = &(*this->table)[i][j];
            char neighbors = getNeighbors(lastCell);
            if (isAlive(lastCell)){
                if (neighbors==2 || neighbors==3){
                    continue;
                }
                subNeighbors({i,j});
                kill(cell);
                continue;
            } else {
                if (neighbors==3){
                    grow(cell);
                    addNeighbors({i,j});
                    continue;
                }
            } 
        }
    }
}
