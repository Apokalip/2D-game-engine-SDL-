#include <SDL/SDL.h>
#include <GL/glew.h>
#include "Engine2D.h"
namespace Engine2D {

	int init() {
		//Initialize SDL

		SDL_Init(SDL_INIT_EVERYTHING);
		//one clears,while the other draws
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		
		return 0;
	}

}