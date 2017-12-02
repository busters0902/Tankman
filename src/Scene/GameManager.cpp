#include "GameManager.h"
#include "cinder/app/AppNative.h"

#include "../KeyManager.h"
#include "../Debug.h"

#include "Scene.h"

#include "../SoundsManager.h"
#include "../Shadow.h"

using namespace ci;
using namespace ci::app;

//シングルトン
GameManagerSP GameManager::instance;

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void GameManager::Setup()
{
	
	KeyManager::Initialize();

	SoundsManager::Initialize();

	Debug::Initialize();

	Shadow::Initialize();

	CreateScene();

}

void GameManager::Shutdown()
{

	SoundsManager::Finalize();

	Shadow::Finalize();

	Debug::Finalize();
}
	
void GameManager::Update()
{
	KeyManager::Update();
	
	Debug::ChangeDebugDraw();

	Debug::Update();

	Scene::Get()->Shift();

	Scene::Get()->Update();
}

void GameManager::Draw()
{

	//メイン描画
	if(Scene::Get() != nullptr) Scene::Get()->Draw();

	//トランジションなど

}

//=======================================================================================
//シーン移動
//=======================================================================================
void GameManager::Shift()
{
	
}

void GameManager::CreateScene()
{

	//タイトル生成
	Scene::Create<TitleScene>(new TitleScene());

}

void GameManager::MouseDown(MouseEvent event)
{
	Scene::Get()->MouseDown(event);
}

void GameManager::MouseUp(MouseEvent event)
{
	Scene::Get()->MouseUp(event);
}

void GameManager::MouseMove(MouseEvent event)
{
	Scene::Get()->MouseMove(event);
}

void GameManager::MouseDrag(MouseEvent event)
{
	Scene::Get()->MouseDrag(event);
}