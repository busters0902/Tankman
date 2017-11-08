#pragma once


#include "cinder/gl/gl.h"
#include <memory>

#include "Transform.h"
#include "Model.h"

class Parts;
typedef std::shared_ptr<Parts> PartsSP;
typedef std::weak_ptr<Parts> PartsWP;

class Parts
{

public:
	Parts();
	~Parts();

	static Parts* Create();
	static Parts* Create(
		const std::string& meshPath,
		const std::string& texturePath,
		const ci::Vec3f& pos,
		const ci::Matrix44f& rot);
	static Parts* Create(
		const std::string& meshPath,
		const std::string& texturePath);

	static PartsSP CreateSP(
		const std::string& meshPath,
		const std::string& texturePath);

	static Parts* CreateMeshAndTexture(
		const std::string& meshPath,
		const std::string& texturePath);

	static PartsSP CreateMeshAndTextureSP(
		const std::string& meshPath,
		const std::string& texturePath);


	static void Destroy(Parts* obj);

public:
	Transform trans;
	Model		model;

	ci::CameraPersp* camera;

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};

