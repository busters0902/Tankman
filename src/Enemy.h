#pragma once
//敵
//
//

#include "cinder/gl/gl.h"
#include <memory>

#include "Camera.h"
#include "Transform.h"
#include "Model.h"
#include "Parts.h"
//#include "Bullet.h"
#include "Collision.h"
//


class Enemy;
typedef std::shared_ptr<Enemy> EnemySP;
typedef std::weak_ptr<Enemy> EnemyWP;

class Enemy
{

public:
	Enemy();
	~Enemy();

	static EnemySP origin;
	static void SetOrigin();

	static Enemy* Create();
	static void Destroy(Enemy* obj);
	static EnemySP CreateSP() { return EnemySP(Create()); }
	static EnemySP Create(const ci::JsonTree& json);

public:
	ci::CameraPersp* camera;
	virtual void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

	Transform trans;
	Model	  model;

	//BulletSP bullet;

public:
	bool isDeath;		//死亡フラグ
	int cnt;			//経過時間
	int waitTime;

public: //基本ステータス
	int type;
	float hp; //ライフ💛

	//移動方向と速度
	ci::Vec3f dir;
	float spd;

	//活動限界時間
	int limit;

public: //その他の関数
	void SetRedBullet(); //バレットのマテリアルとライトの設定

public: //コリジョン
	Collision::AABB aabb;
	Collision::Sphere sphere;

	bool usesCollider;
	bool drawsCollider;

public:
	void Setup();
	void ShutDown();
	virtual void Update() ;
	virtual void Draw() ;

	virtual void CreateCollision(const ci::JsonTree& json);
	virtual void DestroyCollision();
	virtual void UpdateCollision();
	virtual void DrawCollision();
};

/*
生成時にすべきこと
初期位置、初期ベクトル、ベクトルに合わせ回転
★カメラのセット、終了じかんの設定
*/
