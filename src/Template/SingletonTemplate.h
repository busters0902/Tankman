#pragma once

#include "cinder/gl/gl.h"
#include <memory>

#include "Tank.h"

class CSingletonManager;
typedef std::shared_ptr<CSingletonManager> SingletonManagerSP;
typedef std::weak_ptr<CSingletonManager> SingletonManagerWP;

class CSingletonManager
{
private:
	static SingletonManagerSP instance;
	CSingletonManager();
	~CSingletonManager();
public:
	static SingletonManagerSP &GetInstance()
	{
		if (instance.get() == NULL)
			instance.reset(new CSingletonManager(), [](CSingletonManager *p) {delete p; });
		return instance;
	}
	static void DestroyInstance()
	{
		instance.reset();
	}

public:
	ci::CameraPersp* camera;

public:

	void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};
