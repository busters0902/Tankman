#pragma once
//タイトルシーンで使う3Dオブジェクト

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

	//これ使う
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
	Model	model;	//モデルの中のメッシュは使わん
	

	bool isDeath;		//死亡フラグ
	int cnt;			//経過時間

	//サイズ

	//※UI タップ範囲
	//Rectf
	//RECT rect;

	//タップされてるかどうか
	//範囲と判定を決めてその範囲をタップしているか
	bool taps;
	
	//UIの点滅
	bool flashes;				//点滅する状態か
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