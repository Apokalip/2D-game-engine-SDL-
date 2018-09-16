#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Engine2D {

	TextureCache::TextureCache()
	{
	}


	TextureCache::~TextureCache()
	{
	}
	GLTexture TextureCache::getTexture(std::string texturePath) {
		//look up texture
		auto mit = _textureMap.find(texturePath);
		//check if it is not in the map
		if (mit == _textureMap.end()) {
			//load texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);
			//insert into map
			_textureMap.insert(make_pair(texturePath, newTexture));
			//std::cout << "Used Cached texture!\n";
			return newTexture;
		}
		//return the texture ( 1-key/2-value)
	//	std::cout << "loaded  texture!\n";
		return mit->second;
	}

}