//Main

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "KeyManager.h"
#include "Scene/GameManager.h"
#include "Debug.h"

using namespace ci;
using namespace ci::app;


class GameApp : public AppNative
{

public:
	void prepareSettings(Settings* settings);

	void setup();
	void shutdown();
	void update();
	void draw();

	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseMove(MouseEvent event);
	void mouseDrag(MouseEvent event);
};

void GameApp::prepareSettings(Settings* settings)
{
	settings->setWindowSize(GameManager::WindowSize::Width, GameManager::WindowSize::Height);
}

void GameApp::setup()
{
	GameManager::GetInstance();
	GameManager::GetInstance()->Setup();
}

void GameApp::shutdown()
{
	GameManager::GetInstance()->Shutdown();
}

void GameApp::update()
{
	//ƒQ[ƒ€I—¹
	if (KeyManager::GetKeyDown(VK_ESCAPE)) quit();

	GameManager::GetInstance()->Update();
}

void GameApp::draw()
{
	gl::clear(Color(0, 0, 0));

	GameManager::GetInstance()->Draw();
}

void GameApp::mouseDown(MouseEvent event)
{
	GameManager::GetInstance()->MouseDown(event);
}

void  GameApp::mouseUp(MouseEvent event)
{
	GameManager::GetInstance()->MouseUp(event);
}

void  GameApp::mouseMove(MouseEvent event)
{
	GameManager::GetInstance()->MouseMove(event);
}

void  GameApp::mouseDrag(MouseEvent event)
{
	GameManager::GetInstance()->MouseDrag(event);
}

CINDER_APP_NATIVE(GameApp, RendererGl)
