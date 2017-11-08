#include "TitleObject2.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"

using namespace ci;
using namespace ci::app;

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
TitleObject2::TitleObject2()
{

}

TitleObject2::~TitleObject2()
{
	models.clear();
}

//=======================================================================================
//生成・破棄
//=======================================================================================
TitleObject2* TitleObject2::Create()
{
	TitleObject2* obj = new TitleObject2();

	return obj;
}

TitleObject2* TitleObject2::Create(std::string const& modelName,  std::string const& texName, const Transform& trans)
{
	TitleObject2* obj = new TitleObject2();

	//モデル-------------------------------------------------------------------------------------------
	//obj->model = Model(modelName, texName);		※コピーコンストラクタの悪夢その２
	obj->model.Init(modelName, texName);

	//パーツ
	obj->InitParts();

	//Transform----------------------------------------------------------------------------------------
	obj->trans.pos = trans.pos;
	obj->trans.rot = trans.rot;
	obj->trans.scl = trans.scl;

	
	//ライト-------------------------------------------------------------------------------------------

	//マテリアる---------------------------------------------------------------------------------------

	return obj;
}

TitleObject2SP TitleObject2::CreateSP(std::string const& modelName, std::string const& texName, Transform const& trans)
{
	TitleObject2SP obj = TitleObject2SP(Create(modelName, texName, trans));
	return obj;
}


void TitleObject2::Destroy( TitleObject2* obj)
{
	delete obj;
}


//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void TitleObject2::Setup()
{

}

void TitleObject2::ShutDown()
{

}
	
void TitleObject2::Update()
{
	float ang = 0.1f;
	Matrix44f left_rot = Matrix44f::createRotation(Vec3f::zAxis(), -ang);
	Matrix44f right_rot = Matrix44f::createRotation(Vec3f::zAxis(), -ang);

	for (int i = 0; i < 4; i++)
	{
		models.at(3+2*i)->trans.rot *= left_rot;
		models.at(4+2*i)->trans.rot *= right_rot;
	}





	++cnt;
}

void TitleObject2::Draw()
{
	

	for (size_t i = 0; i < models.size(); ++i)
	{
		models.at(i)->Draw();
	}


}

//======================================================================================
//コリジョン用の関数 AABBの更新
//======================================================================================

void TitleObject2::InitParts()
{
	//light->setAmbient(Color(1.0f, 1.0f, 1.0f));
	//light->setDiffuse(Color(0.0f, 0.0f, 0.0f));
	//light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	//light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

	//body
	PartsSP obj = Parts::CreateSP("mesh/tank/body.obj", "texture/white.png");
	obj->trans.pos = { 0.0f, 0.8f, 0.0f };
	obj->trans.rot = Matrix44f::identity();
	obj->model.light->setAmbient(Color(0.5f, 0.5f, 0.5f));
	obj->model.light->setDiffuse(Color(0.5f, 0.5f, 0.5f));
	models.push_back(obj);

	//gunTureent
	obj = Parts::CreateSP("mesh/tank/gun_turret.obj", "texture/white.png");
	obj->trans.pos = { 0.0f, 1.1f, 0.2f };
	obj->trans.rot = Matrix44f::identity();
	obj->model.light->setAmbient(Color(0.6f, 0.6f, 0.6f));
	models.push_back(obj);

	//gun
	obj = Parts::CreateSP("mesh/tank/gun.obj", "texture/white.png");
	obj->trans.pos = { 0.0f, 1.4f, -0.8f };
	obj->trans.rot = Matrix44f::identity();
	obj->trans.scl = { 1.0f, 1.f, 0.7f };
	obj->model.light->setAmbient(Color(0.6f, 0.6f, 0.6f));
	models.push_back(obj);

	//tire------------------------------------------------------------------------
	
	const int TIRE_NUM = 4;
	float width = 1.0f;
	PartsSP lTire, rTire;
	Matrix44f rot_ = Matrix44f::createRotation(Vec3f::yAxis(), toRadians(90.0f));

	for (int i = 0; i < TIRE_NUM; i++)
	{

		lTire = Parts::CreateSP("mesh/tank/tire.obj", "texture/tire.png");
		rTire = Parts::CreateSP("mesh/tank/tire.obj", "texture/tire.png");

		float v = 0.5f * float(i);
		Vec3f pos_ = { 0.6f, 0.3f, v - 0.7f };

		//l
		lTire->trans.pos = pos_;
		lTire->trans.rot = rot_;

		pos_.x = -pos_.x;

		//r
		rTire->trans.pos = pos_;
		rTire->trans.rot = rot_;

		models.push_back(lTire);
		models.push_back(rTire);

	}


	//------------------------------------------------------------------------


}

void TitleObject2::SetCamera(ci::CameraPersp* camera_)
{
	for (size_t i = 0; i < models.size(); ++i)
	{
		models.at(i)->camera = camera_;
	}
}