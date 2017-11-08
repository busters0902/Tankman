#pragma once
//2016/05/31
//よく使うクラスのテンプレ
//
//

#include "cinder/gl/gl.h"
#include <memory>

class CTemp;
typedef std::shared_ptr<CTemp> TempSP;
typedef std::weak_ptr<CTemp> TempWP;

class CTemp
{

public:
	CTemp();
	~CTemp();

	static CTemp* Create();
	static void Destroy(CTemp* obj);

public:


public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};

