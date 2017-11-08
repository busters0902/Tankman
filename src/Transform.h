#pragma once
//translate, rotation, scaling‚Ì—v‘f‚ğ‚ÂƒNƒ‰ƒX
//
//

#include "cinder/gl/gl.h"
#include <memory>

class Transform;
typedef std::shared_ptr<Transform> TransformSP;
typedef std::weak_ptr<Transform> TransformWP;

class Transform
{

public:
	Transform();
	~Transform();

	static Transform* Create();
	static void Destroy(Transform* obj);

public:
	//translate
	ci::Vec3f pos;
	//rotation
	ci::Matrix44f rot;
	//scaling
	ci::Vec3f scl;

	ci::Matrix44f GetPosition() { return ci::Matrix44f::createTranslation(pos); }
public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};

