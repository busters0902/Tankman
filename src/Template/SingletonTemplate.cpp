#include "SingletonManager.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;

//
SingletonManagerSP CSingletonManager::instance;

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
CSingletonManager::CSingletonManager() : camera(nullptr)
{

}

CSingletonManager::~CSingletonManager()
{

}

//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
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
