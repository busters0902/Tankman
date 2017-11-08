#pragma once

//・モノステート

#include <memory>
#include "cinder/gl/gl.h"

#include "Camera.h"

class Debug
{
	enum WindowSize
	{
		WINDOW_WIDTH = 800,
		WINDOW_HEIGHT = 600,
	};
private:
	Debug() {}
	~Debug() {}

public:
	static void Initialize();
	static void Finalize();
public:
	static ci::CameraPersp* camera;
	static bool drawsDebugLog;
	static bool usesDebugMode;
	static bool drawsColliders;
	static bool drawsStringHit;
	static int coll;

	static ci::Vec2f closest;

public:
	static ci::CameraOrtho* cameraOrtho;	//平行投影
	static ci::Font* font;					//フォント
	static ci::Vec3f* pV;
	static CameraTPS* pCam;

public:
	static void DrawString(float x, float y, float size, const std::string format, ...);

	static void ChangeDebugDraw();

public:
	static void Update();
	static void Draw();

};

