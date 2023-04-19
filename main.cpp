#include "main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

Conway* conway;


int main(){
	conway = new Conway(256);
    conway->initSDL();
	while(!conway->exit){
		conway->render();
        conway->copyTable();
        conway->update();
        conway->getInput();
        //SDL_Delay(20);
	}
	return 0;
}
