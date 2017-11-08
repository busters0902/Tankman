#include "Model.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"


using namespace ci;
using namespace ci::app;

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
Model::Model() : mode(NORMAL), light(nullptr), material(nullptr){}

Model::~Model()
{
	
}

Model::Model( const std::string& meshPath, const std::string& texturePath)
	: mode(NORMAL)
{
	//�e�N�X�`���[-----------------------------------------------------------------------
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//���b�V��---------------------------------------------------------------------------
	// OBJ�`����ǂݍ��ޏ���
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// �ǂݍ����TriMesh�ɕϊ�
	mesh = std::make_shared<TriMesh>();
	loader.load(mesh.get(), boost::logic::indeterminate, boost::logic::indeterminate, false);

	//�}�e���A��
	CreateMaterial();

	//���C�g
	//CreateLight();
	light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));

	light->setAmbient(Color(1.0f, 1.0f, 1.0f));
	light->setDiffuse(Color(0.0f, 0.0f, 0.0f));
	light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

}


//=======================================================================================
//�����E�j��
//=======================================================================================
Model* Model::Create()
{
	Model* obj = nullptr;
	return obj;
}

Model* Model::Create( const std::string& meshPath, const std::string& texturePath)
{
	Model* obj = new Model();

	//�e�N�X�`���[-----------------------------------------------------------------------
	obj->texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//���b�V��---------------------------------------------------------------------------
	// OBJ�`����ǂݍ��ޏ���
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// �ǂݍ����TriMesh�ɕϊ�
	loader.load(obj->mesh.get(), boost::logic::indeterminate, boost::logic::indeterminate, false);

	//���C�g-----------------------------------------------------
	obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
	// ��
	obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
	// �g
	obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// ��
	obj->light->setSpecular(Color(0.8f, 0.8f, 0.8f));
	// ��
	obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

	//�}�e���A��-------------------------------------------------
	obj->material = MaterialSP(new gl::Material());
	// ��
	obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
	// �g
	obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// ��
	obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
	// ���ʔ��˂̉s��
	obj->material->setShininess(80.0f);
	// ��
	obj->material->setEmission(Color(0.0f, -1.0f, 0.0f));

	return obj;
}

Model* Model::Create(Mode m)
{
	Model* obj = new Model();
	obj->mode = m;
	
	//�f�t�H���g�ݒ�  �e�I�u�W�F�N�g�̃R���X�g���N�^�ɂď��������ׂ�
	if (m == LIGHT_ONLY)
	{	
		obj->Create();
		obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// ��
		obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// �g
		obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// ��
		obj->light->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// ��
		obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
	}	
	else if (m == LIGHT_AND_MATERIAL)
	{
		obj->Create();
		//���C�g-----------------------------------------------------
		obj->light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// ��
		obj->light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// �g
		obj->light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// ��
		obj->light->setSpecular(Color(0.8f, 0.8f, 0.8f));
		// ��
		obj->light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

		//�}�e���A��-------------------------------------------------
		obj->material = MaterialSP(new gl::Material());
		// ��
		obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// �g
		obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// ��
		obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// ���ʔ��˂̉s��
		obj->material->setShininess(80.0f);
		// ��
		obj->material->setEmission(Color(0.0f, -1.0f, 0.0f));

		//-----------------------------------------------------------
	}
	else if (m == MATERIAL_ONLY)
	{
		//�}�e���A��-------------------------------------------------
		obj->material = MaterialSP(new gl::Material());
		// ��
		obj->material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		// �g
		obj->material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		// ��
		obj->material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		// ���ʔ��˂̉s��
		obj->material->setShininess(80.0f);
		// ��
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
//�f�t�H��Ɓ@�}�e���A���E���C�g�̐���
//=======================================================================================
void Model::CreateTexture(const std::string& texturePath)
{
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));
}


void Model::CreateMeshAndTexture(const std::string& meshPath, const std::string& texturePath)
{
	//�e�N�X�`���[-----------------------------------------------------------------------
	texture = gl::Texture::create(loadImage(loadAsset(texturePath.c_str())));

	//���b�V��---------------------------------------------------------------------------
	// OBJ�`����ǂݍ��ޏ���
	ObjLoader loader = ObjLoader(loadAsset(meshPath.c_str()));

	// �ǂݍ����TriMesh�ɕϊ�
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
	//�}�e���A��-------------------------------------------------
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
		//���C�g-----------------------------------------------------
		light = LightSP(new gl::Light(gl::Light::DIRECTIONAL, 0));
		// ��
		light->setAmbient(Color(0.2f, 0.2f, 0.2f));
		
		light->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		
		light->setSpecular(Color(0.8f, 0.8f, 0.8f));
		
		light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));
		
		//���A��-------------------------------------------------
		material = MaterialSP(new gl::Material());
		
		material->setAmbient(Color(0.2f, 0.2f, 0.2f));
		
		material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
		
		material->setSpecular(Color(0.6f, 0.6f, 0.6f));
		//�ʔ��˂̉s��
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
//Load	//���\�[�X�̃j�W���E�ǂݍ��݂�h��
//=======================================================================================


//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
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

