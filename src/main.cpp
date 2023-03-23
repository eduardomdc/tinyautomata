#include "main.hpp"
#include <SDL2/SDL_events.h>

Conway* conway;


int main(){
	conway = new Conway();
	conway->init();
	while(true){
		while(SDL_PollEvent(&conway->currentEvent)){
			switch(conway->currentEvent.type){
			case SDL_QUIT:
				conway->clean();
				return 0;
				break;
			}
		}
		conway->render();
	}
	return 0;
}
