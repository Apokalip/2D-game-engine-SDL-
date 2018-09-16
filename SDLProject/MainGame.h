#pragma once
#include <Engine2D/Sprite.h>
#include <Engine2D/GLTexture.h>
#include <Engine2D/GLSLProgram.h>
#include <vector>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Engine2D/Window.h>
#include <Engine2D/Camera2D.h>
#include <Engine2D/SpriteBatch.h>
#include <Engine2D/ResourceManager.h>



enum class GameState {PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

	

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();
	void calculateFPS();
	Engine2D::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;

	
	
	Engine2D::GLSLProgram _colorProgram;
	Engine2D::Camera2D _camera;
	Engine2D::SpriteBatch _spriteBatch;
	float _fps;
	float _maxFps;
	float _frameTime;
	float _time;
};

