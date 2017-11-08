#pragma once
//��ԓ����ł��o���e
//
//

#include "cinder/gl/gl.h"
#include <memory>

#include "Model.h"
#include "Collision.h"
#include "cinder/Json.h"

class Bullet;
typedef std::shared_ptr<Bullet> BulletSP;
typedef std::weak_ptr<Bullet> BulletWP;

class Bullet
{ 

public:
	Bullet();
	~Bullet();

	static Bullet* Create();
	static BulletSP CreateSP() 
	{ 
		BulletSP obj = BulletSP(Create()); return obj;
	}

	static void Destroy( Bullet* obj_);

	//�摜�A���f��̂݃R�s�[(�������x����)
	static Bullet* CreateClone(Bullet* obj);
	static BulletSP CreateClone(BulletSP obj);
	static BulletSP Create(const ci::JsonTree& data);

	static BulletSP CreateClone(
		const ci::Vec3f& pos,
		const ci::Vec3f& dir,
		const float velocity,
		BulletSP bullet,
		ci::CameraPersp* cmaera);


public:
	//���ƂŏC��
	ci::CameraPersp* camera;
	void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

	Transform trans;
	Model	model;

	//�^��
	ci::Vec3f dir;		//�ړ�����
	float vel;			//�ړ����x
	ci::Vec3f mov;		//�ړ���
	ci::Vec3f grav;		//�d�͉����x

public :
	enum Type
	{
		TYPE_NULL,	
		PLAYER,		//�v���[���[�̂��
		ENEMY,		//�G�l�~�[�̂��
		TYPE_NUM,
	};
	int type;

	//bool 
	bool isDeath;		//���S�t���O

	//�U����
	float power;

	int cnt;			//�o�ߎ���

public: //�R���W����
	Collision::AABB aabb;
	Collision::Sphere sphere;

	bool usesCollider;		//�R���W�������g�p���邩
	bool drawsCollider;

	void CreateCollision(const ci::JsonTree& json);
	void DestroyCollision();
	void UpdateCollision();
	void DrawCollision();

	void ResizeCollision(float size);
public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};