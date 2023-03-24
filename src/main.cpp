#include "main.hpp"
#include <SDL2/SDL_events.h>

Conway* conway;


int main(){
	conway = new Conway();
	conway->init();
	while(!conway->exit){
		conway->getInput();
		if (!conway->exit){
			conway->render();
		}
	}
	return 0;
}
