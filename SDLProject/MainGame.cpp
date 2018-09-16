#include "MainGame.h"
#include <Engine2D/Engine2D.h>
#include <Engine2D/Errors.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <stdlib.h>


MainGame::MainGame() :
	_screenWidth(1280),
	_screenHeight(800),
	_time(0.0f),
	_gameState(GameState::PLAY),
	_maxFps(120.0f)

{
	//initilize the camera
	_camera.init(_screenWidth,_screenHeight);
}


MainGame::~MainGame()
{
}

void MainGame::run() {
	initSystems();

	gameLoop();
}

void MainGame::initSystems() {

	Engine2D::init();
	_window.create("Game Engine",_screenWidth,_screenHeight,0);
	initShaders();
	_spriteBatch.init();
	
}
void MainGame::initShaders() {
	_colorProgram.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
	
}
void MainGame::gameLoop()
{
	while (_gameState!=GameState::EXIT) {
		_time += 0.01f;
		//time measuring
		static int frameCounter = 0;
		float startTicks = SDL_GetTicks();
		
		//events
		processInput();

		_camera.update();

		//draw
		drawGame();
		
		//fps counter
				calculateFPS();
				if (frameCounter == 100)
				{

					std::cout << _fps << std::endl;
					frameCounter = 0;
				}
				//delay
				float frameTicks = SDL_GetTicks()-startTicks;
				if (1000.0f / _maxFps>frameTicks) {
					SDL_Delay(1000.0f/_maxFps-frameTicks);
				}
				frameCounter++;
			}
}

void MainGame::processInput()
{	
	SDL_Event evnt;
	const float CAMERA_SPEED = 20.0f;
	const float SCALE_SPEED = 0.1f;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//mouse test
			//system("cls");
			//std::cout<<evnt.motion.x<<std::endl;
		//	std::cout << evnt.motion.y<<std::endl;

			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym) {
			case SDLK_w:
				_camera.setPosition(_camera.getPosition()+glm::vec2(0.0f, CAMERA_SPEED));
				break;
			case SDLK_s:
				_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
				break;
			case SDLK_d:
				_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
				break;
			case SDLK_a:
				_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;
			case SDLK_q:
				_camera.setScale(_camera.getScale() + SCALE_SPEED);
				break;
			case SDLK_e:
				_camera.setScale(_camera.getScale() - SCALE_SPEED);
				break;
			}
		}
	}
}

void MainGame::drawGame() {

	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_colorProgram.use();
	//bind player texture (shader)
	glActiveTexture(GL_TEXTURE0);
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0);
	//modify time in shader
	GLuint timeLocation = _colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, _time);
	//set the cameraMatrix
	GLuint pLocation = _colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));



	//draw
	_spriteBatch.begin();
	//test glyph ( sprite)
	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Engine2D::GLTexture texture = Engine2D::ResourceManager::getTexture("Textures/JimmyJumpPack/PNG/CharacterRight_Standing.png");
	Engine2D::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	for (int i = 0; i < 1000; i++) {
		_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	_spriteBatch.draw(pos + glm::vec4(50.0f, 0.0f, 0.0f, 0.0f), uv, texture.id, 0.0f, color);
}
	_spriteBatch.end();
	_spriteBatch.renderBatch();




	glBindTexture(GL_TEXTURE_2D, 0);
	//unuse the shader
	_colorProgram.unUse();
	
	_window.swapBuffer();
	
}
void MainGame::calculateFPS() {
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;

	static float prevTicks = SDL_GetTicks();
	float currentTicks = SDL_GetTicks();
	_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

	prevTicks = currentTicks;

	int count;
	
	currentFrame++;
	if (currentFrame < NUM_SAMPLES) {
		count = currentFrame;
	}
	else {
		count = NUM_SAMPLES;
	}
	float frameTimeAvarage = 0;
	for (int i = 0; i < count; i++) {
		frameTimeAvarage = +frameTimes[i];
	}
	frameTimeAvarage /= count;
	if (frameTimeAvarage > 0) {
		_fps = 1000.0f / frameTimeAvarage/count;
	}
	else {
		_fps = 0;
	}
	
}