#include "main.hpp"
#include <SDL2/SDL_events.h>

Conway* conway;


int main(){
	conway = new Conway(4);
	conway->init();
	while(!conway->exit){
		conway->getInput();
		if (!conway->exit){
			conway->render();
		}
	}
	return 0;
}
