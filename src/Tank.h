#pragma once
//プレイヤー他が使う戦車クラス
//・行列を使い、砲塔やタイヤなど分離して管理する
//
//

#include "cinder/gl/gl.h"
#include <memory>
#include <boost/signals2.hpp>


#include "Parts.h"
#include "Bullet.h"

#include "cinder/Json.h"

class Tank;
typedef std::shared_ptr<Tank> TankSP;
typedef std::weak_ptr<Tank> TankWP;

class Tank
{
public:
	enum Tire
	{
		TIRE_0,
		TIRE_1,
		TIRE_2,
		TIRE_3,
		TIRE_NUM,
	};


public:
	Tank();
	~Tank();

	static Tank* Create();
	static Tank* Create(	const ci::Vec3f& pos, 
							const std::string& texturePath, 
							const std::string& meshPath );
	static Tank* Create(const ci::Vec3f& pos);
	static TankSP CreateSP(const ci::Vec3f& pos);
	//出来ればjsonTreeをぶっ込めるやつ
	static TankSP Create(const ci::JsonTree& data);

	static void Destroy(Tank* obj);

public:
	void SetCamera(ci::CameraPersp* camera_) { camera = camera_; }

public:
	Transform trans;
	Model	model;	//※仮

	ci::CameraPersp* camera;

	//コピー元
	BulletSP bullet;

	//狙いを教えもらう
	ci::Vec3f tgtPos;

public:
	boost::signals2::signal<void(ci::Vec3f&, ci::Vec3f&, float)>* bulletCreate;


public:	//パーツ
	enum MyParts
	{
		BODY,
		GUN_TURRET,
		GUN,
		LEFT_TIRE_1,
		LEFT_TIRE_2,
		LEFT_TIRE_3,
		LEFT_TIRE_4,
		RIGHT_TIRE_1,
		RIGHT_TIRE_2,
		RIGHT_TIRE_3,
		RIGHT_TIRE_4,
		PARTS_NUM,
	};
	//PartsSP parts[PARTS_NUM];
	std::vector<PartsSP> parts;

	//移動方向に合わせた回転
	ci::Vec3f rot;	//オイラー角

	//移動
	float mov;	//移動量
	float spd;	//移動速度
	ci::Vec3f movDir;		//移動方向
	ci::Vec3f force;		//慣性…主に重力

	//タイヤの回転量	
	float leftTireRot;
	float rightTireRot;
	//砲塔の回転量
	float gunTurretRot;
	float gunRot;

	bool usesDebugMode;

	int cnt;
	const float MAX_HP = 100.f;
	float hp; //ライフ
	bool isDeath;
	bool isInvincible;
	bool can_shot;
	int do_shot;	//downで　-1 up で 1 
	//bool do_shot;

	float bulSpd;

public : //その他の関数
	void DrawTank();
	void SetupStatus();
	void Shot();
	void UseDebugMode();

	//タイヤの回転
	void RotateRightTire(float ang);
	void RotateLeftTire(float ang);

public:	//コリジョン
	bool usesCollider;		//コリジョンを使用するか
	bool drawsCollider;

	//void CreateCollision();		
	//void DestroyCollision();	
	//void UpdateCollision();		
	//void DrawCollision();
	//void DrawCollisionQuad();

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};

/*

Tank - model
	 - parts[n] - model

Tank::modelの　mesh, textureは使わず　light, materialは使う
Tank:: parts[n]::modelの light, materialは使わず　mesh, textureは使う

*/
