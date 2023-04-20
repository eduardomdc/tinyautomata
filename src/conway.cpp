#include "conway.hpp"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>
#include <random>

#define ALIVE_COLOR 255
#define DEAD_COLOR 0

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
    for (int i=0;i<size;i++){
        for (int j=0;j<size;j++){
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
            int wrapX = (*wrapIndex)[line+position.x+1];
            int wrapY = (*wrapIndex)[col+position.y+1];
            addNeighbor(&(*this->table)[wrapX][wrapY]); 
        }
    }
}

void Conway::subNeighbors(pos position){
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (line==0 && col==0) continue;
            int wrapX = (*wrapIndex)[line+position.x+1];
            int wrapY = (*wrapIndex)[col+position.y+1];
            removeNeighbor(&(*this->table)[wrapX][wrapY]); 
        }
    }
}

char Conway::countNeighbors(pos position){
    char neighbors = 0;
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (!(line==0 && col==0)){
                int wrapX = (*wrapIndex)[line+position.x+1];
                int wrapY = (*wrapIndex)[col+position.y+1];
                if (isAlive(&(*this->table)[wrapX][wrapY])) neighbors++;
            }
        }
    }
    return neighbors;
}

Conway::Conway(int size){
    this->size = size;
    this->exit = false;
    this->table = new std::vector<std::vector<char>>();
    this->lastTable = new std::vector<std::vector<char>>();
    this->wrapIndex = new std::vector<int>(size+2);
    //wrapping look up table
    (*this->wrapIndex)[0] = size-1;
    for (int i=1; i<size+1; i++){
        (*this->wrapIndex)[i] = i-1;
    }
    (*wrapIndex)[size+1] = 0;
    //allocate table
    for (int i=0;i<size;i++){
        this->table->push_back(std::vector<char>(size));
    }
    this->randomizeTable();  
    //first time setup neighbors counts
    for (int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            int neighbors = countNeighbors({i,j});
            setNeighbors(&(*this->table)[i][j], neighbors);
        }
    }
    //copy table over to lastTable
    this->lastTable->assign(this->table->begin(), this->table->end());
    std::cout<<"That's life."<<std::endl;
}

void Conway::initSDL(){
    if (SDL_Init(SDL_INIT_VIDEO) == 0){
        this->window = SDL_CreateWindow("conway's game",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                this->size,
                this->size,
                SDL_WINDOW_BORDERLESS
                );
        this->surface = SDL_GetWindowSurface(this->window);
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

void Conway::drawCell(pos position, short color){
    Uint8* pixelptr = (Uint8*)surface->pixels + (position.x * size + position.y)*4 + 2;
    *(pixelptr) = color;
}

void Conway::fullRender(){
    // need to render all cells first so that we 
    // can render only the ones that change later
    Uint8* pixelptr = (Uint8*)surface->pixels;
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            char* cell = &(*this->table)[i][j];
            *(pixelptr + j*4 + 2) = 255*(isAlive(cell));
        }
        pixelptr += size*4;
    }
    SDL_UpdateWindowSurface(this->window);
}

void Conway::update(){
    this->lastTable->assign(this->table->begin(),this->table->end());
    for (int i=0;i<this->size;i++){
        for (int j=0;j<this->size;j++){
            char* lastCell = &(*this->lastTable)[i][j];
            if (*lastCell == 0) continue;
            char* cell = &(*this->table)[i][j];
            char neighbors = getNeighbors(lastCell);
            if (isAlive(lastCell)){
                if (neighbors==2 || neighbors==3){
                    continue;
                }
                subNeighbors({i,j});
                kill(cell);
                drawCell({i,j}, DEAD_COLOR);
                continue;
            } else {
                if (neighbors==3){
                    grow(cell);
                    addNeighbors({i,j});
                    drawCell({i,j}, ALIVE_COLOR);
                    continue;
                }
            } 
        }
    }
    SDL_UpdateWindowSurface(this->window);
}
