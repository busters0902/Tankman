#include "SingletonManager.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

//
SingletonManagerSP CSingletonManager::instance;

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
CSingletonManager::CSingletonManager() : camera(nullptr)
{

}

CSingletonManager::~CSingletonManager()
{

}

//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void CSingletonManager::Setup()
{

}

void CSingletonManager::ShutDown()
{
}
	
void CSingletonManager::Update()
{

}

void CSingletonManager::Draw()
{

}

//=======================================================================================
//
//=======================================================================================
