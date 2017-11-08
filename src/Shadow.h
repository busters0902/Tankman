#pragma once
//êÌé‘ìôÇ™ë≈ÇøèoÇ∑íe
//
//

#include "cinder/gl/gl.h"
#include <memory>

#include "Model.h"
#include "Collision.h"
#include "cinder/Json.h"

class Shadow;
typedef std::shared_ptr<Shadow> ShadowSP;
typedef std::weak_ptr<Shadow> ShadowWP;

class Shadow
{ 

public:
	Shadow();
	~Shadow();

	static void Initialize();
	static void Finalize();

	static ShadowSP shadow;
	static void Draw(ci::CameraPersp& camera, 
		const ci::Vec3f& pos, 
		const ci::Vec3f& size);

	static void Destroy( Shadow* obj_);


public:
	/*ci::CameraPersp* camera;
	void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }*/

	Transform trans;
	Model	model;

public :
	enum Type
	{
		NORMAL,
	};
	int type;

	bool draws;

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};