#include "Bullet.h"
#include "cinder/app/AppNative.h"

#include "Debug.h"
#include "Math.h"
//#include "SoundsManager.h"
#include "Shadow.h"

using namespace ci;
using namespace ci::app;

void Vec3ToAxisAngle(Vec3f* pAx, float& angle, const Vec3f* pV1, const Vec3f* pV2);

const float BULLET_SIZE = 0.2f;

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
Bullet::Bullet() : cnt(0), dir(0.0f, 0.0f, 0.0f), vel(0.0f), mov(0.0f, 0.0f, 0.0f), 
	grav(0.0f, 0.0f, 0.0f), isDeath(false), power(0.f), camera(nullptr), 
	usesCollider(false), drawsCollider(false),type(TYPE_NULL)
{

}

Bullet::~Bullet()
{

}

//=======================================================================================
//�����E�j��
//=======================================================================================
Bullet* Bullet::Create()
{
	Bullet* obj = new Bullet();

	//�g�����X�t�H�[��

	//���f��-------------------------------------------------------------------
	//body
	Vec3f pos_ = { 0.0f, 0.8f, 0.0f };
	Matrix44f rot_ = Matrix44f::identity();
	obj->model.Init("mesh/bullet.obj", "texture/white.png");

	//���C�g-------------------------------------------------------------------------------------------

	// ��	 .
	obj->model.light->setAmbient(Color(0.3f, 0.3f, 1.0f));
	// �g	 .
	obj->model.light->setDiffuse(Color(0.3f, 0.3f, 1.0f));
	// ��	 .
	obj->model.light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	// ��	 .
	obj->model.light->setDirection(Vec3f(0.0f, 1.0f, 0.0f));


	//�}�e���A��---------------------------------------------------------------------------------------
	// ��	 .
	obj->model.material->setAmbient(Color(0.5f, 0.5f, 1.0f));
	// �g	 .
	obj->model.material->setDiffuse(Color(0.5f, 0.5f, 1.0f));
	// ��	 .
	obj->model.material->setSpecular(Color(1.0f, 1.0f, 1.0f));
	// ���ʔ���.�s��
	obj->model.material->setShininess(80.0f);
	// ��	 .
	obj->model.material->setEmission(Color(0.0f, 0.0f, 0.0f));

	//�R���W����---------------------------------------------------------------------------------------
	//JsonTree Collision
	//obj->CreateCollision();

	//�X�e�[�^�X---------------------------------------------------------------------------------------
	obj->power = 10.f;
	obj->type = TYPE_NULL;


	return obj;
}

void Bullet::Destroy( Bullet* obj)
{
	delete obj;
}

Bullet* Bullet::CreateClone( Bullet* obj_)
{
	Bullet* obj = new Bullet();

	//�g�����X�t�H�[��
	obj->trans.scl = obj_->trans.scl;

	//���f��|�C���^�̃R�s�[
	obj->model = obj_->model;

	//�R���W����
	//obj->CreateCollision();

	//�X�e�[�^�X
	obj->power = obj_->power;
	obj->type = obj_->type;

	return obj;
}

BulletSP Bullet::CreateClone(BulletSP obj_)
{
	BulletSP obj = BulletSP(new Bullet());

	//�g�����X�t�H�[��
	obj->trans.scl = obj_->trans.scl;

	//���f��|�C���^�̃R�s�[
	obj->model = obj_->model;

	//�R���W����
	//obj->CreateCollision();

	//�X�e�[�^�X
	obj->grav = obj_->grav;
	obj->power = obj_->power;
	obj->type = obj_->type;

	return obj;
}

BulletSP Bullet::Create(const JsonTree& data)
{

	//Bullet* obj = new Bullet();
	BulletSP obj = BulletSP(new Bullet());

	//�g�����X�t�H�[��
	JsonTree Pos = data["Pos"];
	obj->trans.pos.x = Pos.getValueAtIndex<float>(0);
	obj->trans.pos.y = Pos.getValueAtIndex<float>(1);
	obj->trans.pos.z = Pos.getValueAtIndex<float>(2);

	JsonTree Rot = data["Rot"];
	float x = Rot.getValueAtIndex<float>(0);
	float y = Rot.getValueAtIndex<float>(1);
	float z = Rot.getValueAtIndex<float>(2);
	obj->trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

	JsonTree Scl = data["Scl"];
	obj->trans.scl.x = Scl.getValueAtIndex<float>(0);
	obj->trans.scl.y = Scl.getValueAtIndex<float>(1);
	obj->trans.scl.z = Scl.getValueAtIndex<float>(2);

	//���f��-------------------------------------------------------------------
	//body
	auto meshPath = data["MeshName"].getValue();
	auto texPath = data["TexName"].getValue();
	obj->model.Init(meshPath, texPath);

	//���C�g-------------------------------------------------------------------------------------------
	//Light
	{
		JsonTree Ambient = data["Ambient"];
		float x = Ambient.getValueAtIndex<float>(0);
		float y = Ambient.getValueAtIndex<float>(1);
		float z = Ambient.getValueAtIndex<float>(2);
		obj->model.light->setAmbient(Color(x, y, z));

		JsonTree Diffuse = data["Diffuse"];
		x = Diffuse.getValueAtIndex<float>(0);
		y = Diffuse.getValueAtIndex<float>(1);
		z = Diffuse.getValueAtIndex<float>(2);
		obj->model.light->setDiffuse(Color(x, y, z));

		JsonTree Direction = data["Direction"];
		x = Direction.getValueAtIndex<float>(0);
		y = Direction.getValueAtIndex<float>(1);
		z = Direction.getValueAtIndex<float>(2);
		obj->model.light->setDirection(Vec3f(x, y, z));
	}

	//Material
	{
		JsonTree _Ambient = data["_Ambient"];
		x = _Ambient.getValueAtIndex<float>(0);
		y = _Ambient.getValueAtIndex<float>(1);
		z = _Ambient.getValueAtIndex<float>(2);
		float w = _Ambient.getValueAtIndex<float>(3);
		obj->model.material->setAmbient(ColorA(x, y, z, w));

		JsonTree _Diffuse = data["_Diffuse"];
		x = _Diffuse.getValueAtIndex<float>(0);
		y = _Diffuse.getValueAtIndex<float>(1);
		z = _Diffuse.getValueAtIndex<float>(2);
		w = _Diffuse.getValueAtIndex<float>(3);
		obj->model.material->setDiffuse(ColorA(x, y, z, w));

		JsonTree _Specular = data["_Specular"];
		x = _Specular.getValueAtIndex<float>(0);
		y = _Specular.getValueAtIndex<float>(1);
		z = _Specular.getValueAtIndex<float>(2);
		w = _Specular.getValueAtIndex<float>(3);
		obj->model.material->setSpecular(ColorA(x, y, z, w));

		//JsonTree _Shininess = data["Player._Shininess"];
		//float s = _Shininess.getValueForKey<float>("shininess");
		//JsonTree _Shininess = data["Player"];

		float s = data.getValueForKey<float>("_Shininess");
		obj->model.material->setShininess(s);

		JsonTree _Emission = data["_Emission"];
		x = _Emission.getValueAtIndex<float>(0);
		y = _Emission.getValueAtIndex<float>(1);
		z = _Emission.getValueAtIndex<float>(2);
		w = _Emission.getValueAtIndex<float>(3);
		obj->model.material->setEmission(ColorA(x, y, z, w));
	}

	//�R���W����---------------------------------------------------------------------------------------
	
	obj->CreateCollision(data);

	//�X�e�[�^�X---------------------------------------------------------------------------------------
	JsonTree Grav = data["Grav"];
	x = Grav.getValueAtIndex<float>(0);
	y = Grav.getValueAtIndex<float>(1);
	z = Grav.getValueAtIndex<float>(2);
	obj->grav = Vec3f(x, y, z);

	obj->power = data.getValueForKey<float>("Power");
	obj->type = data.getValueForKey<int>("Type");


	return obj;

}

BulletSP Bullet::CreateClone(const Vec3f& pos, const Vec3f& dir, const float velocity, BulletSP bullet, ci::CameraPersp* camera)
{
	BulletSP obj = Bullet::CreateClone(bullet);

	//�g�����X�t�H�[���̐ݒ�Translation--------------------------------------------------

	obj->trans.pos = pos;
	obj->trans.rot = ToRotation(dir);
	obj->trans.scl = bullet->trans.scl;

	//�J�����̃Z�b�g--------------------------------------------------------------------
	obj->camera = camera;

	//move�֘A--------------------------------------------------------------------------
	//�����x�N�g��
	obj->dir = dir;

	//�x�N�g���傫��
	obj->vel = velocity;

	//�d�͉����x

	//�R���W�����̑傫��-----------------------------------------------------------------
	obj->aabb.whl = bullet->aabb.whl;
	obj->sphere.r = bullet->sphere.r;

	obj->aabb.pos = pos;
	obj->sphere.c = pos;

	obj->drawsCollider = bullet->drawsCollider;
	obj->usesCollider = bullet->usesCollider;

	return obj;
}

//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================
void Bullet::Setup()
{

}

void Bullet::ShutDown()
{

}
	
void Bullet::Update()
{
	//�ړ��֘A
	mov = (vel * dir) + grav;
	trans.pos += mov;
	dir = mov.normalized();

	//�e�̈ړ������Ɍ��� dir , zAxis
	float dot, angle;
	Vec3f cross;
	/*dot  = Vec3f::zAxis().dot(dir);
	cross= Vec3f::zAxis().cross(dir);
		angle = acosf(dot);*/

	Vec3ToAxisAngle(&cross, angle, &(Vec3f(0,0,1)), &dir);

	trans.rot = Matrix44f::createRotation(cross, angle + M_PI);

	//trans->rot = dir;

	//�Փ˔���
	UpdateCollision();

	if (cnt > 120) isDeath = true;

	//�����t���O
	if (trans.pos.y < -10.0f) isDeath = true;

	++cnt;
}

void Bullet::Draw()
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

	//�e
	if(trans.pos.y > 0.0f) Shadow::Draw(*camera, trans.pos, Vec3f(0.2f, 0.2f, 1.0f));


	//----------------------------------------------------------------------------------
	//�R���W�����̕`��
	if (drawsCollider && Debug::drawsColliders) DrawCollision();
}

//=======================================================================================
//�R���W����
//=======================================================================================

void Bullet::CreateCollision(const ci::JsonTree& json)
{
	JsonTree AABB = json["AABB"];
	Vec3f v;
	v.x = AABB.getValueAtIndex<float>(0);
	v.y = AABB.getValueAtIndex<float>(1);
	v.z = AABB.getValueAtIndex<float>(2);
	aabb.pos = trans.pos;
	aabb.whl = v;

	sphere.c = trans.pos;
	sphere.r = json.getValueForKey<float>("Rad");

	usesCollider = true;
	drawsCollider = true;
}

void Bullet::DestroyCollision()
{

}

void Bullet::UpdateCollision()
{
	aabb.pos = trans.pos;
	sphere.c = trans.pos;
}

void Bullet::DrawCollision()
{
	gl::lineWidth(3);

	// �J��������ݒ�
	gl::setMatrices(*camera);

	// ���s�ړ�
	gl::translate(aabb.pos);

	// XYZ�����ꂼ��ł̉�]
	gl::rotate(Matrix44f().identity());

	// �X�P�[�����O
	gl::scale(aabb.whl * 2.0f);

	gl::color(1, 0, 0);
	
	gl::drawStrokedCube(Vec3f(0, 0, 0), Vec3f(1, 1, 1));

	gl::color(1, 1, 1);

	// �J��������ݒ�
	gl::setMatrices(*camera);

	// ���s�ړ�
	gl::translate(sphere.c);

	// XYZ�����ꂼ��ł̉�]
	gl::rotate(Matrix44f().identity());

	// �X�P�[�����O
	gl::scale( Vec3f::one() );

	gl::color(1, 0, 0);

	//gl::drawStrokedCube(Vec3f(0, 0, 0), Vec3f(1, 1, 1));
	gl::drawSphere(Vec3f(0, 0, 0), sphere.r/2);

	gl::color(1, 1, 1);

	gl::lineWidth(1);
}

void Bullet::ResizeCollision(float size)
{

}