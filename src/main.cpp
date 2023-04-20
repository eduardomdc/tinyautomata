#include "main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

Conway* conway;


int main(){
	conway = new Conway(256);
    conway->initSDL();
    conway->fullRender();
	while(!conway->exit){
        conway->update();
        conway->getInput();
	}
	return 0;
}
