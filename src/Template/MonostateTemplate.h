#pragma once

//・モノステート

#include <memory>
#include "cinder/gl/gl.h"

class CUIManager;
typedef std::shared_ptr<CUIManager> UIManagerSP;
typedef std::weak_ptr<CUIManager> UIManagerWP;

class CUIManager
{
private:
	CUIManager() {}
	~CUIManager() {}

public:
	static void Initialize();
	static void Finalize();
public:
	static ci::CameraPersp* camera;

public:

public:
	static void Update();
	static void Draw();

};

