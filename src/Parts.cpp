#include "Parts.h"
#include "cinder/app/AppNative.h"

using namespace ci;
using namespace ci::app;


//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
Parts::Parts() : camera(nullptr)
{

}

Parts::~Parts()
{

}

//=======================================================================================
//生成・破棄
//=======================================================================================
Parts* Parts::Create()
{
	Parts* obj = nullptr;

	return obj;
}

Parts* Parts::Create( const std::string& meshPath, const std::string& texturePath,
	const ci::Vec3f& pos, const ci::Matrix44f& rot)
{
	Parts* obj = new Parts();

	//Transform
	obj->trans.pos = pos;
	obj->trans.rot = rot;

	//Model
	//obj->model.reset(Model::Create(meshPath, texturePath));
	obj->model.Init(meshPath, texturePath);

	return obj;
}

Parts* Parts::Create(const std::string& meshPath, const std::string& texturePath)
{
	Parts* obj = new Parts();

	//Transform
	//obj->trans.reset(new Transform());

	//Model
	//obj->model.reset(Model::Create(meshPath, texturePath));
	obj->model.Init(meshPath, texturePath);

	return obj;
}

PartsSP Parts::CreateSP(const std::string& meshPath, const std::string& texturePath)
{
	PartsSP obj(Create(meshPath, texturePath));
	return obj;
}

Parts* Parts::CreateMeshAndTexture( const std::string& meshPath, const std::string& texturePath)
{
	Parts* obj = new Parts();

	//Model
	obj->model.InitMeshAndTexture(meshPath, texturePath);

	return obj;
}

PartsSP Parts::CreateMeshAndTextureSP(const std::string& meshPath, const std::string& texturePath)
{
	PartsSP obj(CreateMeshAndTexture(meshPath, texturePath));
	return obj;
}


void Parts::Destroy( Parts* obj)
{
	delete obj;
}


//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void Parts::Setup()
{

}

void Parts::ShutDown()
{

}
	
void Parts::Update()
{

}

void Parts::Draw()
{
	model.material->apply();
	gl::enable(GL_CULL_FACE);

	if (camera == nullptr) return;
	gl::setMatrices(*camera);

	// ライトを有効にする
	gl::enable(GL_LIGHTING);
	gl::enable(GL_NORMALIZE);
	model.light->enable();

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::scale(trans.scl);

	model.texture->enableAndBind();
	gl::draw(*model.mesh);
	model.texture->disable();

	//ライトを無効
	model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);
}

