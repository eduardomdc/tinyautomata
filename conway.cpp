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
            addNeighbor(&(*this->table)[line+position.x][col+position.y]); 
        }
    }
}

void Conway::subNeighbors(pos position){
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (line==0 && col==0) continue;
            removeNeighbor(&(*this->table)[line+position.x][col+position.y]); 
        }
    }
}

char countNeighbors(std::vector<std::vector<char>>* table, pos cellPos){
    char neighbors = 0;
    for (int line=-1;line<2;line++){
        for (int col=-1;col<2;col++){
            if (!(line==0 && col==0)){
                if (isAlive(&(*table)[cellPos.x+line][cellPos.y+col])) neighbors++;
            }
        }
    }
    return neighbors;
}

void Conway::updatePadding(){
   for (int j=1;j<size+1;j++){
        //top line to bottom padding
        (*this->table)[size+1][j] = (*this->table)[1][j];
        //bottom line to top padding
        (*this->table)[0][j] = (*this->table)[size][j];
        //left collum to right padding
        (*this->table)[j][size+1] = (*this->table)[j][1];
        //right collum to left padding
        (*this->table)[j][0] = (*this->table)[j][size];
    }
    //update diagonals
    (*this->table)[0][0] = (*this->table)[size][size];
    (*this->table)[size+1][size+1] = (*this->table)[1][1];
    (*this->table)[0][size+1] = (*this->table)[size][1];
    (*this->table)[size+1][0] = (*this->table)[1][size];
}
/* PPPPP
 * PCCCP
 * PCCCP
 * PCCCP
 * PPPPP
 *  012
 *  123
 * */

Conway::Conway(int size){
    this->size = size;
    this->exit = false;
    this->table = new std::vector<std::vector<char>>();
    this->lastTable = new std::vector<std::vector<char>>();
    //allocate table with padding!!
    for (int i=0;i<size+2;i++){
        this->table->push_back(std::vector<char>(size+2));
    }
    this->randomizeTable();
    //place a glider to test toroidality
    (*this->table)[30][30] = 1;
    (*this->table)[31][31] = 1;
    (*this->table)[31][32] = 1;
    (*this->table)[30][32] = 1;
    (*this->table)[29][32] = 1;
    //first time padding setup
    this->updatePadding();
    //first time setup neighbors counts
    for (int i=1;i<size+1;i++){
        for(int j=1;j<size+1;j++){
            int neighbors = countNeighbors(this->table, {i,j});
            setNeighbors(&(*this->table)[i][j], neighbors);
        }
    }
    //second time padding setup
    this->updatePadding();
    //copy table over to lastTable
    this->lastTable->assign(this->table->begin(), this->table->end());
}

void Conway::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        this->window = SDL_CreateWindow("conway's game",
                0,
                0,
                this->size+2,
                this->size+2,
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
            if (i==0 || j ==0 || i==size+1 || j==size+1){
                    SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
                    SDL_RenderDrawPoint(this->renderer, i, j);
                    SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
            }
            if (isAlive(cell)) SDL_RenderDrawPoint(this->renderer, i, j);
            else {
                char* lastCell = &(*this->lastTable)[i][j];
                if (isAlive(lastCell)){
                    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
                    SDL_RenderDrawPoint(this->renderer, i, j);
                    SDL_SetRenderDrawColor(this->renderer, 0, 255, 0, 255);
                }
            }
        }
    }
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
    SDL_RenderPresent(this->renderer); 
}

void Conway::update(){
    //add new neighbors around padding
    for (int j=1;j<size+1;j++){
        char neighbors;
        neighbors = countNeighbors(this->lastTable, {1,j});
        setNeighbors(&(*this->lastTable)[1][j], neighbors);
        neighbors = countNeighbors(this->lastTable, {size,j});
        setNeighbors(&(*this->lastTable)[size][j], neighbors);
        neighbors = countNeighbors(this->lastTable, {j,1});
        setNeighbors(&(*this->lastTable)[j][1], neighbors);
        neighbors = countNeighbors(this->lastTable, {j,size});
        setNeighbors(&(*this->lastTable)[j][size], neighbors);
    }

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
    updatePadding();
}
