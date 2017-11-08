#pragma once
//�悭�g���N���X�̃e���v��
//
//

#include "cinder/gl/gl.h"
#include <memory>

#include "Transform.h"
#include "Model.h"
#include "Camera.h"
#include "cinder/Json.h"
//#include "Collision.h"

class StageObject;
typedef std::shared_ptr<StageObject> StageObjectSP;
typedef std::weak_ptr<StageObject> StageObjectWP;

class StageObject
{
	enum ObjectType
	{
		FLOOR,
		WALL,
		BOX,
	};

public:
	StageObject();
	~StageObject();

	static StageObject* Create();
	static void Destroy(StageObject* obj);

	static StageObject* Create( std::string const& modelName,
								std::string const& texName, 
								Transform const& trans );
	static StageObjectSP CreateSP(	std::string const& modelName,
									std::string const& texName, 
									Transform const& trans );

	static StageObjectSP Create(const ci::JsonTree& json);

public:
	ci::CameraPersp* camera;
	virtual void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

	Transform trans;
	Model	model;

	bool isDeath;		//���S�t���O
	int cnt;			//�o�ߎ���

public : //Collision
	//bool usesCollider;	//�R���W�������g�p���邩
	//AABB aabb;		//����
	//ci::Vec3f colSize;		//�R���W�����p�̃I�u�W�F�̃T�C�Y
	//Rect2DSP rect;		//xz���ʂ̔���


	bool drawsCollider;
	void UpdateCollision();	//���g�̃R���W������V��������
	void CreateCollision( ObjectType type ); //���g�ɃR���W���������
	void CreateCollisionFloor();
	void CreateCollisionWall();
	void CreateCollisionBox();
	void DestroyCollision();

	void DrawCollision();
public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};
// detect collisions �����̏Փ˂����o����
// update collider ����̍X�V