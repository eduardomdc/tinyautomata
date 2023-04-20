#include "main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

Conway* conway;

Uint32 lastUpdateTime;
Uint32 updateTime;
Uint32 currentGeneration;
Uint32 generationsPerSecond;

int main(){
	conway = new Conway(256);
    conway->initSDL();
    conway->fullRender();
	while(!conway->exit){
        updateTime = SDL_GetTicks();
        conway->update();
        conway->getInput();
        currentGeneration++;
        generationsPerSecond++;
        if (updateTime-lastUpdateTime > 1000){
            lastUpdateTime = updateTime;
            std::cout<< "Current generation: "<<currentGeneration<<std::endl;
            std::cout<< "Gens/second:"<< generationsPerSecond << std::endl;
            generationsPerSecond = 0;
        }
	}
    delete conway;
	return 0;
}
