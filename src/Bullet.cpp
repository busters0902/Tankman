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
//コンストラクタ、ディストラクタ
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
//生成・破棄
//=======================================================================================
Bullet* Bullet::Create()
{
	Bullet* obj = new Bullet();

	//トランスフォーム

	//モデる-------------------------------------------------------------------
	//body
	Vec3f pos_ = { 0.0f, 0.8f, 0.0f };
	Matrix44f rot_ = Matrix44f::identity();
	obj->model.Init("mesh/bullet.obj", "texture/white.png");

	//ライト-------------------------------------------------------------------------------------------

	// 環	 .
	obj->model.light->setAmbient(Color(0.3f, 0.3f, 1.0f));
	// 拡	 .
	obj->model.light->setDiffuse(Color(0.3f, 0.3f, 1.0f));
	// 鏡	 .
	obj->model.light->setSpecular(Color(1.0f, 1.0f, 1.0f));
	// 光	 .
	obj->model.light->setDirection(Vec3f(0.0f, 1.0f, 0.0f));


	//マテリアル---------------------------------------------------------------------------------------
	// 環	 .
	obj->model.material->setAmbient(Color(0.5f, 0.5f, 1.0f));
	// 拡	 .
	obj->model.material->setDiffuse(Color(0.5f, 0.5f, 1.0f));
	// 鏡	 .
	obj->model.material->setSpecular(Color(1.0f, 1.0f, 1.0f));
	// 鏡面反射.鋭さ
	obj->model.material->setShininess(80.0f);
	// 光	 .
	obj->model.material->setEmission(Color(0.0f, 0.0f, 0.0f));

	//コリジョン---------------------------------------------------------------------------------------
	//JsonTree Collision
	//obj->CreateCollision();

	//ステータス---------------------------------------------------------------------------------------
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

	//トランスフォーム
	obj->trans.scl = obj_->trans.scl;

	//モデるポインタのコピー
	obj->model = obj_->model;

	//コリジョン
	//obj->CreateCollision();

	//ステータス
	obj->power = obj_->power;
	obj->type = obj_->type;

	return obj;
}

BulletSP Bullet::CreateClone(BulletSP obj_)
{
	BulletSP obj = BulletSP(new Bullet());

	//トランスフォーム
	obj->trans.scl = obj_->trans.scl;

	//モデるポインタのコピー
	obj->model = obj_->model;

	//コリジョン
	//obj->CreateCollision();

	//ステータス
	obj->grav = obj_->grav;
	obj->power = obj_->power;
	obj->type = obj_->type;

	return obj;
}

BulletSP Bullet::Create(const JsonTree& data)
{

	//Bullet* obj = new Bullet();
	BulletSP obj = BulletSP(new Bullet());

	//トランスフォーム
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

	//モデる-------------------------------------------------------------------
	//body
	auto meshPath = data["MeshName"].getValue();
	auto texPath = data["TexName"].getValue();
	obj->model.Init(meshPath, texPath);

	//ライト-------------------------------------------------------------------------------------------
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

	//コリジョン---------------------------------------------------------------------------------------
	
	obj->CreateCollision(data);

	//ステータス---------------------------------------------------------------------------------------
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

	//トランスフォームの設定Translation--------------------------------------------------

	obj->trans.pos = pos;
	obj->trans.rot = ToRotation(dir);
	obj->trans.scl = bullet->trans.scl;

	//カメラのセット--------------------------------------------------------------------
	obj->camera = camera;

	//move関連--------------------------------------------------------------------------
	//方向ベクトル
	obj->dir = dir;

	//ベクトル大きさ
	obj->vel = velocity;

	//重力加速度

	//コリジョンの大きさ-----------------------------------------------------------------
	obj->aabb.whl = bullet->aabb.whl;
	obj->sphere.r = bullet->sphere.r;

	obj->aabb.pos = pos;
	obj->sphere.c = pos;

	obj->drawsCollider = bullet->drawsCollider;
	obj->usesCollider = bullet->usesCollider;

	return obj;
}

//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void Bullet::Setup()
{

}

void Bullet::ShutDown()
{

}
	
void Bullet::Update()
{
	//移動関連
	mov = (vel * dir) + grav;
	trans.pos += mov;
	dir = mov.normalized();

	//弾の移動方向に向く dir , zAxis
	float dot, angle;
	Vec3f cross;
	/*dot  = Vec3f::zAxis().dot(dir);
	cross= Vec3f::zAxis().cross(dir);
		angle = acosf(dot);*/

	Vec3ToAxisAngle(&cross, angle, &(Vec3f(0,0,1)), &dir);

	trans.rot = Matrix44f::createRotation(cross, angle + M_PI);

	//trans->rot = dir;

	//衝突判定
	UpdateCollision();

	if (cnt > 120) isDeath = true;

	//消すフラグ
	if (trans.pos.y < -10.0f) isDeath = true;

	++cnt;
}

void Bullet::Draw()
{
	// ライトを有効にする
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

	//ライトを無効
	model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);

	//影
	if(trans.pos.y > 0.0f) Shadow::Draw(*camera, trans.pos, Vec3f(0.2f, 0.2f, 1.0f));


	//----------------------------------------------------------------------------------
	//コリジョンの描画
	if (drawsCollider && Debug::drawsColliders) DrawCollision();
}

//=======================================================================================
//コリジョン
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

	// カメラから設定
	gl::setMatrices(*camera);

	// 平行移動
	gl::translate(aabb.pos);

	// XYZ軸それぞれでの回転
	gl::rotate(Matrix44f().identity());

	// スケーリング
	gl::scale(aabb.whl * 2.0f);

	gl::color(1, 0, 0);
	
	gl::drawStrokedCube(Vec3f(0, 0, 0), Vec3f(1, 1, 1));

	gl::color(1, 1, 1);

	// カメラから設定
	gl::setMatrices(*camera);

	// 平行移動
	gl::translate(sphere.c);

	// XYZ軸それぞれでの回転
	gl::rotate(Matrix44f().identity());

	// スケーリング
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