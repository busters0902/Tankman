#include "StageObject.h"

#include "cinder/app/AppNative.h"
#include "cinder/imageIo.h"

//#include "Debug.h"
//#include "StageManager.h"
//#include "CollisionsManager.h"

using namespace ci;
using namespace ci::app;

void TransformFromJson(Transform& trans, const JsonTree& json);

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
StageObject::StageObject() : cnt(0), isDeath(false), drawsCollider(false)
{

}

StageObject::~StageObject()
{

}

//=======================================================================================
//�����E�j��
//=======================================================================================
StageObject* StageObject::Create()
{
	StageObject* obj = new StageObject();

	return obj;
}

StageObject* StageObject::Create(std::string const& modelName,  std::string const& texName, const Transform& trans)
{
	StageObject* obj = new StageObject();

	//���f��-------------------------------------------------------------------------------------------
	//obj->model = Model(modelName, texName);		���R�s�[�R���X�g���N�^�̈������̂Q
	obj->model.Init(modelName, texName);

	//Transform----------------------------------------------------------------------------------------

	obj->trans.pos = trans.pos;
	obj->trans.rot = trans.rot;
	obj->trans.scl = trans.scl;


	return obj;
}

StageObjectSP StageObject::CreateSP(std::string const& modelName, std::string const& texName, const Transform& trans)
{
	StageObjectSP obj = StageObjectSP(Create(modelName, texName, trans));
	return obj;
}

StageObjectSP StageObject::Create(const JsonTree& json)
{
	StageObjectSP obj = StageObjectSP(new StageObject());

	//
	auto meshName = json["MeshName"].getValue();
 	auto texName = json["TexName"].getValue();
	obj->model.Init(meshName, texName);

	//
	TransformFromJson(obj->trans, json);
	

	return obj;
}

void StageObject::Destroy( StageObject* obj)
{
	delete obj;
}


//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================
void StageObject::Setup()
{

}

void StageObject::ShutDown()
{

}
	
void StageObject::Update()
{
	++cnt;
}

void StageObject::Draw()
{
	// ���C�g��L���ɂ���
	gl::enable(GL_LIGHTING);
	gl::enable(GL_NORMALIZE);
	
	model.light->enable();

	gl::setMatrices(*camera);

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::scale(trans.scl);

	model.material->apply();
	gl::enable(GL_CULL_FACE);

	model.texture->enableAndBind();
	gl::draw(*model.mesh);
	model.texture->disable();

	//���C�g�𖳌�
	model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);

	//if (drawsCollider && CDebug::drawsColliders ) DrawCollision();

}

//======================================================================================
//�R���W�����p�̊֐� AABB�̍X�V
//======================================================================================

void StageObject::UpdateCollision()
{
	/*aabb->pos = trans->pos;
	aabb->whl = colSize;

	*rect = Collision::ToRect2DAABB(*aabb);*/
}

void StageObject::CreateCollision(ObjectType type)
{
	switch (type)
	{
		case FLOOR :
			CreateCollisionFloor(); break;

		case WALL :
			CreateCollisionWall(); break;

		case BOX :
			CreateCollisionBox(); break;
	}
}

void StageObject::CreateCollisionFloor()
{

}

void StageObject::CreateCollisionWall()
{

}

void StageObject::CreateCollisionBox()
{
	
}

void StageObject::DestroyCollision()
{
	/*aabb.reset();*/
}

void StageObject::DrawCollision()
{
	
	// ���f���r���[�s��ƃv���W�F�N�V�����s���
	// �J��������ݒ�
	//gl::setMatrices(*camera);

	// ���s�ړ�
	//gl::translate(aabb->pos);

	// XYZ�����ꂼ��ł̉�]
	//gl::rotate(trans->rot);
	//gl::rotate(Matrix44f::identity());

	// �X�P�[�����O
	//gl::scale(aabb->whl * 2);

	/*gl::color(1, 0, 0);
	gl::lineWidth(3);
	gl::drawStrokedCube(Vec3f(0, 0, 0), Vec3f(1, 1, 1));

	gl::color(1, 1, 1);
	gl::lineWidth(1);*/
	
}