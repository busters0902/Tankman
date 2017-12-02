#include "GameManager.h"
#include "cinder/app/AppNative.h"

#include "../KeyManager.h"
#include "../Debug.h"

#include "Scene.h"

#include "../SoundsManager.h"
#include "../Shadow.h"

using namespace ci;
using namespace ci::app;

//�V���O���g��
GameManagerSP GameManager::instance;

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
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

	//���C���`��
	if(Scene::Get() != nullptr) Scene::Get()->Draw();

	//�g�����W�V�����Ȃ�

}

//=======================================================================================
//�V�[���ړ�
//=======================================================================================
void GameManager::Shift()
{
	
}

void GameManager::CreateScene()
{

	//�^�C�g������
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