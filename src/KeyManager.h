#pragma once
//�E�����̃J�����𐧌䂷��N���X
//�E���m�X�e�[�g
//

#include <memory>

class KeyManager
{
private:
	KeyManager() {}
	~KeyManager() {}

public:
	static void Initialize();

private:
	static const int MAX_KEY = 256;
	static bool key[MAX_KEY];
	static  bool prevKey[MAX_KEY];

public:
	static bool GetKey(int id) { return key[id]; } 
	static bool GetKeyDown(int id) ;
	static bool GetKeyUp(int id) ;

public:
	static void Update();
};

