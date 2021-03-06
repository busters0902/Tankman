#include "cinder/app/AppNative.h"
#include "Loader.h"

using namespace ci;

std::map<std::string, _data*> Loader::values;

//=======================================================================================
//初期化処理、終了処理
//=======================================================================================
void Loader::Initialize()
{

}

void Loader::Finalize()
{
	RemoveAll();
}

bool Loader::Remove(const std::string& path)
{
	auto it = values.find(path);

	//探し物がない場合
	if (it == values.end())
	{
		return false;
	}

	//deleteしてerase
	delete it->second;
	values.erase(path);

	return true;
}

void Loader::RemoveAll()
{
	auto it = values.begin();
	while (it != values.end()) 
	{
		delete (it->second);
		values.erase(it++);		//???
	}
}

_data* Loader::Load(const std::string& path)
{
	_data* obj = nullptr;

	std::map<std::string, _data*>::iterator it = values.find(path);

	//登録されてる場合はそれを返す
	if (it != values.end())
	{
		obj = it->second;
		return obj;
	}
	
	//新しく登録する場合
	obj = new _data();

	//登録
	values.insert(make_pair(path, obj));

	return obj;
}