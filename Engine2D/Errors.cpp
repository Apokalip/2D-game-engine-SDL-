#include "Errors.h"
#include <iostream>
#include <SDL/SDL.h>
#include <cstdlib>

namespace Engine2D {

	//prints out error
	void fatalError(std::string errorString) {
		std::cout << errorString << std::endl;
		std::cout << "Enter any key to quit..." << std::endl;
		int tmp;
		std::cin >> tmp;
		SDL_Quit();
		exit(69);
	}

}