#include "TitleObject.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"

using namespace ci;
using namespace ci::app;

void TransformFromJson(Transform& trans, const JsonTree& json);

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
TitleObject::TitleObject() : cnt(0), isDeath(false)
{

}

TitleObject::~TitleObject()
{

}

//=======================================================================================
//�����E�j��
//=======================================================================================
TitleObject* TitleObject::Create()
{
	TitleObject* obj = new TitleObject();

	return obj;
}

TitleObject* TitleObject::Create(std::string const& modelName,  std::string const& texName, const Transform& trans)
{
	TitleObject* obj = new TitleObject();

	//���f��-------------------------------------------------------------------------------------------
	//obj->model = Model(modelName, texName);		���R�s�[�R���X�g���N�^�̈������̂Q
	obj->model.Init(modelName, texName);

	//Transform----------------------------------------------------------------------------------------
	obj->trans.pos = trans.pos;
	obj->trans.rot = trans.rot;
	obj->trans.scl = trans.scl;

	
	//���C�g-------------------------------------------------------------------------------------------

	//�}�e���A��---------------------------------------------------------------------------------------

	return obj;
}

TitleObjectSP TitleObject::CreateSP(std::string const& modelName, std::string const& texName, Transform const& trans)
{
	TitleObjectSP obj = TitleObjectSP(Create(modelName, texName, trans));
	return obj;
}
 
TitleObjectSP TitleObject::Create(const ci::JsonTree& json)
{
	TitleObjectSP obj = TitleObjectSP(new TitleObject());

	
	auto meshName = json["MeshName"].getValue();
	auto texName = json["TexName"].getValue();
	obj->model.Init(meshName, texName);

	//
	TransformFromJson(obj->trans, json);


	return obj;
	
}


void TitleObject::Destroy( TitleObject* obj)
{
	delete obj;
}


//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================
void TitleObject::Setup()
{

}

void TitleObject::ShutDown()
{

}
	
void TitleObject::Update()
{
	++cnt;
}

void TitleObject::Draw()
{
	/*model.light->setDirection(Vec3f(0, -1, 0));
	model.material->setDiffuse(Color(0,0,0));*/

	model.material->apply();
	gl::enable(GL_CULL_FACE);

	gl::setMatrices(*camera);

	// ���C�g��L���ɂ���
	gl::enable(GL_LIGHTING);
	gl::enable(GL_NORMALIZE);
	model.light->enable();

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::scale(trans.scl);

	model.texture->enableAndBind();
	gl::draw(*model.mesh);
	model.texture->disable();

	//���C�g�𖳌�
	model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);

}

//======================================================================================
//�R���W�����p�̊֐� AABB�̍X�V
//======================================================================================
