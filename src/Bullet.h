#pragma once
//戦車等が打ち出す弾
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

	//画像、モデるのみコピー(処理速度向上)
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
	//あとで修正
	ci::CameraPersp* camera;
	void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

	Transform trans;
	Model	model;

	//運動
	ci::Vec3f dir;		//移動方向
	float vel;			//移動速度
	ci::Vec3f mov;		//移動量
	ci::Vec3f grav;		//重力加速度

public :
	enum Type
	{
		TYPE_NULL,	
		PLAYER,		//プレーヤーのやつ
		ENEMY,		//エネミーのやつ
		TYPE_NUM,
	};
	int type;

	//bool 
	bool isDeath;		//死亡フラグ

	//攻撃力
	float power;

	int cnt;			//経過時間

public: //コリジョン
	Collision::AABB aabb;
	Collision::Sphere sphere;

	bool usesCollider;		//コリジョンを使用するか
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