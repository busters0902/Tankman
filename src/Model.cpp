#include "Model.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"


using namespace ci;
using namespace ci::app;

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
Model::Model() : mode(NORMAL), light(nullptr), material(nullptr){}

Model::~Model()
{
	
}

Model::Model( const std::string& meshPath, const std::string& texturePath)
	: mode(NORMAL)
{
	//テクスチャー-----------------------------------------------------------------------
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//メッシュ---------------------------------------------------------------------------
	// OBJ形式を読み込む準備
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// 読み込んでTriMeshに変換
	mesh = std::make_shared<TriMesh>();
	loader.load(mesh.get(), boost::logic::indeterminate, boost::logic::indeterminate, false);

	//マテリアル
	CreateMaterial();

	//ライト
	//CreateLight();
	light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));

	light->setAmbient(Color(1.0f, 1.0f, 1.0f));
	light->setDiffuse(Color(0.0f, 0.0f, 0.0f));
	light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

}


//=======================================================================================
//生成・破棄
//=======================================================================================
Model* Model::Create()
{
	Model* obj = nullptr;
	return obj;
}

Model* Model::Create( const std::string& meshPath, const std::string& texturePath)
{
	Model* obj = new Model();

	//テクスチャー-----------------------------------------------------------------------
	obj->texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//メッシュ---------------------------------------------------------------------------
	// OBJ形式を読み込む準備
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// 読み込んでTriMeshに変換
	loader.load(obj->mesh.get(), boost::logic::indeterminate, boost::logic::indeterminate, false);

	//ライト-----------------------------------------------------
	obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
	// 環
	obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
	// 拡
	obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// 鏡
	obj->light->setSpecular(Color(0.8f, 0.8f, 0.8f));
	// 光
	obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

	//マテリアル-------------------------------------------------
	obj->material = MaterialSP(new gl::Material());
	// 環
	obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
	// 拡
	obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// 鏡
	obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
	// 鏡面反射の鋭さ
	obj->material->setShininess(80.0f);
	// 光
	obj->material->setEmission(Color(0.0f, -1.0f, 0.0f));

	return obj;
}

Model* Model::Create(Mode m)
{
	Model* obj = new Model();
	obj->mode = m;
	
	//デフォルト設定  各オブジェクトのコンストラクタにて書き直すべし
	if (m == LIGHT_ONLY)
	{	
		obj->Create();
		obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// 環
		obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// 拡
		obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// 鏡
		obj->light->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// 光
		obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
	}	
	else if (m == LIGHT_AND_MATERIAL)
	{
		obj->Create();
		//ライト-----------------------------------------------------
		obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// 環
		obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// 拡
		obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// 鏡
		obj->light->setSpecular(Color(0.8f, 0.8f, 0.8f));
		// 光
		obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

		//マテリアル-------------------------------------------------
		obj->material = MaterialSP(new gl::Material());
		// 環
		obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// 拡
		obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// 鏡
		obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// 鏡面反射の鋭さ
		obj->material->setShininess(80.0f);
		// 光
		obj->material->setEmission(Color(0.0f, -1.0f, 0.0f));

		//-----------------------------------------------------------
	}
	else if (m == MATERIAL_ONLY)
	{
		//マテリアル-------------------------------------------------
		obj->material = MaterialSP(new gl::Material());
		// 環
		obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// 拡
		obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// 鏡
		obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// 鏡面反射の鋭さ
		obj->material->setShininess(80.0f);
		// 光
		obj->material->setEmission(Color(0.0f, -1.0f, 0.0f));
	}


	return obj;
}

ModelSP Model::CreateSP(const std::string& meshPath, const std::string& texturePath)
{
	ModelSP obj = ModelSP(Create(meshPath, texturePath));
	return obj;
}

void Model::Destroy( Model* obj)
{
	delete obj;
}

//=======================================================================================
//デフォると　マテリアル・ライトの生成
//=======================================================================================
void Model::CreateTexture(const std::string& texturePath)
{
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));
}


void Model::CreateMeshAndTexture(const std::string& meshPath, const std::string& texturePath)
{
	//テクスチャー-----------------------------------------------------------------------
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//メッシュ---------------------------------------------------------------------------
	// OBJ形式を読み込む準備
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// 読み込んでTriMeshに変換
	mesh = std::make_shared<TriMesh>();
	loader.load(mesh.get(), boost::logic::indeterminate, boost::logic::indeterminate, false);
}



void Model::CreateLight()
{
	light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
	
	light->setAmbient(Color(1.0f, 1.0f, 1.0f));
	light->setDiffuse(Color(0.0f, 0.0f, 0.0f));
	light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
}

void Model::CreateMaterial()
{
	//マテリアル-------------------------------------------------
	material = MaterialSP(new gl::Material());
	
	material->setAmbient(Color(1.f, 1.f, 1.f));
	material->setDiffuse(Color(1.f, 1.f, 1.f));
	material->setSpecular(Color(1.0f, 1.0f, 1.0f));
	material->setShininess(80.0f);
	material->setEmission(Color(0.0f, 0.0f, 0.0f));

}

ModelSP Model::CreateSP(Mode m)
{
	ModelSP obj = ModelSP( Create( m ));
	return obj;
}


void Model::Init(const std::string& meshPath, const std::string& texturePath)
{
	mode = NORMAL;

	CreateMeshAndTexture(meshPath, texturePath);
	CreateMaterial();
	CreateLight();

}

void Model::Init(Mode m)
{
	mode = m;

	if (m == LIGHT_AND_MATERIAL)
	{
		//ライト-----------------------------------------------------
		light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// 環
		light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		
		light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		
		light->setSpecular(Color(0.8f, 0.8f, 0.8f));
		
		light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
		
		//リアル-------------------------------------------------
		material = MaterialSP(new gl::Material());
		
		material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		
		material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		
		material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		//面反射の鋭さ
		material->setShininess(80.0f);
		
		material->setEmission(Color(0.0f, -1.0f, 0.0f));
		
		//--------------------------------------------------------
	}
}

void Model::InitMeshAndTexture(const std::string& meshPath, const std::string& texturePath)
{
	CreateMeshAndTexture(meshPath, texturePath);
}

void Model::InitTexture(const std::string& texturePath)
{
	mode = TEXTURE;

	CreateTexture(texturePath);
	CreateMaterial();
	CreateLight();

}


//=======================================================================================
//Load	//リソースのニジュウ読み込みを防ぐ
//=======================================================================================


//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void Model::Setup()
{

}

void Model::ShutDown()
{

}
	
void Model::Update()
{

}

void Model::Draw()
{

}

