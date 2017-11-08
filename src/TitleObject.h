#pragma once
//�^�C�g���V�[���Ŏg��3D�I�u�W�F�N�g

#include "cinder/gl/gl.h"
#include <memory>

#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "cinder/Json.h"
//#include "Collision.h"

class TitleObject;
typedef std::shared_ptr<TitleObject> TitleObjectSP;
typedef std::weak_ptr<TitleObject> TitleObjectWP;

class TitleObject
{

public:
	TitleObject();
	~TitleObject();

	static TitleObject* Create();
	static void Destroy(TitleObject* obj);

	static TitleObject* Create( std::string const& modelName,
		std::string const& texName, Transform const& trans);

	//����g��
	static TitleObjectSP CreateSP(	std::string const& modelName,
									std::string const& texName, 
									Transform const& trans);

	static TitleObjectSP Create(const ci::JsonTree& json);


public:
	ci::CameraPersp* camera;
	virtual void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

	Transform trans;
	Model	model;

	bool isDeath;		//���S�t���O
	int cnt;			//�o�ߎ���

public : //Collision

public:
	virtual void Setup();
	virtual void ShutDown();
	virtual void Update();
	virtual void Draw();

};