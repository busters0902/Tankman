#pragma once
//std::map�����đ��d�쐬���֎~����
//�E���m�X�e�[�g
#include <memory>
#include <map>
#include "cinder/gl/gl.h"




struct _data
{
	int a;
	int b;
};

class Loader
{
private:
	Loader();
	~Loader();

public:
	static void Initialize();
	static void Finalize();
	static _data* Load(const std::string& path);
	static bool Remove(const std::string& path);	//�폜�o������true
	static void RemoveAll();

private:
	static std::map<std::string, _data*> values;

};

