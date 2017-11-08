#pragma once
//�^�C�g���V�[���Ŏg��3D�I�u�W�F�N�g

#include "cinder/gl/gl.h"
#include <memory>

#include "Transform.h"
#include "Model.h"
#include "Camera.h"

class ci::JsonTree;

class UIObject;
typedef std::shared_ptr<UIObject> UIObjectSP;
typedef std::weak_ptr<UIObject> UIObjectWP;

class UIObject
{

public:
	UIObject();
	~UIObject();

	static UIObject* Create();
	static void Destroy(UIObject* obj);

	static UIObject* Create( std::string const& modelName,
		std::string const& texName, Transform const& trans);

	//����g��
	static UIObjectSP CreateSP(	std::string const& modelName,
									std::string const& texName, 
									Transform const& trans);

	static UIObjectSP Create(const ci::JsonTree& json);



public:
	ci::CameraPersp* camera_;
	ci::CameraOrtho* camera;
	//virtual void SetCamera(ci::CameraOrtho* camera_) { camera = camera_; }
	virtual void SetCamera(ci::CameraPersp* camera__) { camera_ = camera__; }

	Transform trans;
	Model	model;	//���f���̒��̃��b�V���͎g���
	

	bool isDeath;		//���S�t���O
	int cnt;			//�o�ߎ���

	//�T�C�Y

	//��UI �^�b�v�͈�
	//Rectf
	//RECT rect;

	//�^�b�v����Ă邩�ǂ���
	//�͈͂Ɣ�������߂Ă��͈̔͂��^�b�v���Ă��邩
	bool taps;
	
	//UI�̓_��
	bool flashes;				//�_�ł����Ԃ�
	bool isInvisible;			
	int invisibleCount;			
	int invisibleInterval;		
	void Flash();			

	void EnableFlashing();		
	void DisableFlashing();		


public : //Collision

public:
	virtual void Setup();
	virtual void ShutDown();
	virtual void Update();
	virtual void Draw();

};