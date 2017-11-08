#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "KeyManager.h"

using namespace ci;

bool KeyManager::key[MAX_KEY];
bool KeyManager::prevKey[MAX_KEY];

//=======================================================================================
//初期化、終了
//=======================================================================================
void KeyManager::Initialize()
{

	for (int i = 0; i < MAX_KEY; i++)
	{
		key[i] = false;
		prevKey[i] = false;
	}

}


//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void KeyManager::Update()
{
	for (int i = 0; i < MAX_KEY; i++)
	{
		prevKey[i] = key[i];
		key[i] = (bool)GetAsyncKeyState(i);
	}
}

bool KeyManager::GetKeyDown(int id) 
{
	if (!key[id])	return false;
	if (key[id] == prevKey[id])	return false;

	return true;
}

bool KeyManager::GetKeyUp(int id)  
{
	if (key[id])	return false;
	if (key[id] == prevKey[id]) return false;

	return true;
}