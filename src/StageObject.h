#pragma once
//よく使うクラスのテンプレ
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

	bool isDeath;		//死亡フラグ
	int cnt;			//経過時間

public : //Collision
	//bool usesCollider;	//コリジョンを使用するか
	//AABB aabb;		//判定
	//ci::Vec3f colSize;		//コリジョン用のオブジェのサイズ
	//Rect2DSP rect;		//xz平面の判定


	bool drawsCollider;
	void UpdateCollision();	//自身のコリジョンを新しくする
	void CreateCollision( ObjectType type ); //自身にコリジョンを作る
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
// detect collisions 複数の衝突を検出する
// update collider 判定の更新