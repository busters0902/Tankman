#pragma once
//std::mapをして多重作成を禁止する
//・モノステート
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
	static bool Remove(const std::string& path);	//削除出来たらtrue
	static void RemoveAll();

private:
	static std::map<std::string, _data*> values;

};

