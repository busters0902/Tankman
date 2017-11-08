#include "cinder/app/AppNative.h"
#include "Loader.h"

using namespace ci;

std::map<std::string, _data*> Loader::values;

//=======================================================================================
//‰Šú‰»ˆ—AI—¹ˆ—
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

	//’T‚µ•¨‚ª‚È‚¢ê‡
	if (it == values.end())
	{
		return false;
	}

	//delete‚µ‚Äerase
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

	//“o˜^‚³‚ê‚Ä‚éê‡‚Í‚»‚ê‚ğ•Ô‚·
	if (it != values.end())
	{
		obj = it->second;
		return obj;
	}
	
	//V‚µ‚­“o˜^‚·‚éê‡
	obj = new _data();

	//“o˜^
	values.insert(make_pair(path, obj));

	return obj;
}