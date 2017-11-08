#include "UIObject.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"

#include "GenericFunctions.h"

using namespace ci;
using namespace ci::app;

void TransformFromJson(Transform& trans, const JsonTree& json);

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
UIObject::UIObject() : cnt(0), isDeath(false), taps(false), 
flashes(false), isInvisible(false), invisibleCount(0), invisibleInterval(30)
{

}

UIObject::~UIObject()
{

}

//=======================================================================================
//�����E�j��
//=======================================================================================
UIObject* UIObject::Create()
{
	UIObject* obj = new UIObject();

	return obj;
}

UIObject* UIObject::Create(std::string const& modelName,  std::string const& texName, const Transform& trans)
{
	UIObject* obj = new UIObject();

	//���f��-------------------------------------------------------------------------------------------
	//obj->model = Model(modelName, texName);		���R�s�[�R���X�g���N�^�̈������̂Q
	obj->model.Init(modelName, texName);

	//Transform----------------------------------------------------------------------------------------
	obj->trans.pos = trans.pos;
	obj->trans.rot = trans.rot;
	obj->trans.scl = trans.scl;

	return obj;
}

UIObjectSP UIObject::Create(const ci::JsonTree& json)
{
	UIObjectSP obj = UIObjectSP(new UIObject());

	//texture���f��
	std::string name = json["TexName"].getValue();
	//std::string name = json.getValueForKey<std::string>("TexName");
	obj->model.InitTexture(name);

	//transfrom
	TransformFromJson(obj->trans, json);


	return obj;
}


UIObjectSP UIObject::CreateSP(std::string const& modelName, std::string const& texName, Transform const& trans)
{
	UIObjectSP obj = UIObjectSP(Create(modelName, texName, trans));
	return obj;
}

void UIObject::Destroy( UIObject* obj)
{
	delete obj;
}


//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================
void UIObject::Setup()
{

}

void UIObject::ShutDown()
{

}
	
void UIObject::Update()
{
	if (flashes) Flash();
	++cnt;
}

void UIObject::Draw()
{
	//�`�悵�Ȃ����
	if (isInvisible) return;

	/*model.material->apply();
	gl::enable(GL_CULL_FACE);*/

	gl::setMatrices(*camera_);

	gl::disable(GL_CULL_FACE);

	//// ���C�g��L���ɂ���
	//gl::enable(GL_LIGHTING);
	//gl::enable(GL_NORMALIZE);
	//model.light->enable();

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::scale(trans.scl);

	model.texture->enableAndBind();
	//gl::draw(*model.mesh);
	gl::drawSolidRect(Rectf(0.5f, 0.5f, -0.5f, -0.5f));	//���s���e
	//gl::drawCube(Vec3f::zero(), Vec3f::one());
	model.texture->disable();

	

	////���C�g�𖳌�
	//model.light->disable();
	//gl::disable(GL_NORMALIZE);
	//gl::disable(GL_LIGHTING);

}

//======================================================================================

//======================================================================================

void UIObject::EnableFlashing()
{
	flashes = true;

	//������
	isInvisible = false;
	invisibleCount = 0;

}

void UIObject::DisableFlashing()
{
	flashes = false;
}

void UIObject::Flash()
{

	if (invisibleCount >= invisibleInterval)
	{
		if (isInvisible) isInvisible = false;
		else isInvisible = true;

		invisibleCount = 0;
	}

	++invisibleCount;
}


