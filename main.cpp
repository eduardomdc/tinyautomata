#include "main.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>

Conway* conway;


int main(){
	conway = new Conway(4);
	conway->init();
	while(!conway->exit){
		conway->getInput();
		if (!conway->exit){
			conway->render();
            conway->update();
		}
        SDL_Delay(1000);
	}
	return 0;
}
