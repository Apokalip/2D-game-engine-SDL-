#pragma once
#include "GLTexture.h"
#include <string>

namespace Engine2D {

	class ImageLoader
	{
	public:
		static GLTexture loadPNG(std::string filePath);
	};

}