#pragma once
//カメラクラス
//・カメラを操作するための変数を格納

#include "cinder/gl/gl.h"
#include <memory>
#include "cinder/Camera.h"
#include "cinder/Json.h"

#include "Scene/GameManager.h"

typedef std::shared_ptr< ci::CameraPersp > CameraPerspSP;
typedef std::weak_ptr< ci::CameraPersp > CameraPerspWP;

class Camera_;
typedef std::shared_ptr	< Camera_ > CameraSP;
typedef std::weak_ptr	< Camera_ > CameraWP;

class CameraTPS;
typedef std::shared_ptr	< CameraTPS > CameraTPSSP;
typedef std::weak_ptr	< CameraTPS > CameraTPSWP;

//=========================================================
//Camera
//=========================================================
class Camera_
{
public:
	Camera_() : camEyePos(0.f, 2.f, -7.f), camLookVec(0.f, 0.f, 0.f)
	{
		camera = new ci::CameraPersp(GameManager::WindowSize::Width, GameManager::WindowSize::Height, 60.0f, 1.0f, 1000.0f);
	}
	~Camera_() {}

public:

	ci::CameraPersp* camera;
	ci::CameraPersp* GetCamera() { return camera; }

	ci::Vec3f camEyePos;
	ci::Vec3f camLookVec;


	void TranslateCamera(ci::Vec3f vec);
public:
	virtual void Setup();
	virtual void Update();
};

//=========================================================
//CameraTPS
//=========================================================
class CameraTPS : public Camera_
{
private:
	
public:
	CameraTPS(ci::Vec3f* tgt_ = nullptr )
		: tgt(tgt_), tgtFromlookVec(0.f, 2.f, 0.f),
		tgtLen(7.f), tgtRotX(ci::toRadians(30.0f)), tgtRotY(0.f), tgtRotSpd(ci::toRadians(5.f))
	{
		camera = new ci::CameraPersp(GameManager::WindowSize::Width, GameManager::WindowSize::Height, 60.0f, 1.0f, 1000.0f);
		Setup();
	}
	~CameraTPS() {}

public:
	ci::Vec3f* tgt;
	ci::Vec3f tgtFromlookVec;
	float tgtLen;
	float tgtRotX;
	float tgtRotY;
	float tgtRotSpd;
	
	void SetTPSCameraTgt(ci::Vec3f* tgtPtr){tgt = tgtPtr;}
	void SetTPSCameraTgt(ci::Vec3f& tgtPtr) { tgt = &tgtPtr;}

	void Setup(const ci::JsonTree& json);

public:
	virtual void Setup();
	virtual void Update();
};