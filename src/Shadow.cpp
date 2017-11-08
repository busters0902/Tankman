#include "Shadow.h"
#include "cinder/app/AppNative.h"

#include "Debug.h"
#include "Math.h"

using namespace ci;
using namespace ci::app;

void Vec3ToAxisAngle(Vec3f* pAx, float& angle, const Vec3f* pV1, const Vec3f* pV2);

ShadowSP Shadow::shadow;


//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
Shadow::Shadow()
{

}

Shadow::~Shadow()
{

}

//=======================================================================================
//生成・破棄
//=======================================================================================

void Shadow::Initialize() 
{
	shadow = ShadowSP(new Shadow());
	shadow->model.Init("mesh/shadow.obj","texture/white.png");

	//ライト-----------------------------------------------------
	// 環
	shadow->model.light->setAmbient(Color(0.0f, 0.0f, 0.0f));
	// 拡
	shadow->model.light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// 鏡
	shadow->model.light->setSpecular(Color(0.8f, 0.8f, 0.8f));
	// 光
	shadow->model.light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
	
	//マテリアル-------------------------------------------------
	// 環
	shadow->model.material->setAmbient(Color(0.2f, 0.2f, 0.2f));
	// 拡
	shadow->model.material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// 鏡
	shadow->model.material->setSpecular(Color(0.6f, 0.6f, 0.6f));
	// 鏡面反射の鋭さ
	shadow->model.material->setShininess(80.0f);
	// 光
	shadow->model.material->setEmission(Color(0.0f, -1.0f, 0.0f));


	//各パラメーター
	shadow->type = NORMAL;
	shadow->draws = true;

}

void Shadow::Finalize()
{
}

void Shadow::Draw(ci::CameraPersp& camera, const ci::Vec3f& pos, const Vec3f& size)
{

	// ライトを有効にする
	gl::enable(GL_LIGHTING);
	gl::enable(GL_NORMALIZE);

	shadow->model.light->enable();

	gl::setMatrices(camera);

	Vec3f pos_ = pos;
	pos_.y = 0;

	gl::translate(pos_);
	gl::rotate(shadow->trans.rot);
	gl::scale(size);

	shadow->model.material->apply();
	gl::enable(GL_CULL_FACE);

	shadow->model.texture->enableAndBind();
	gl::draw(*shadow->model.mesh);
	shadow->model.texture->disable();

	//ライトを無効
	shadow->model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);
}

void Shadow::Destroy( Shadow* obj)
{
	delete obj;
}