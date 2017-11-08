#include "Enemy.h"
#include "cinder/app/AppNative.h"

#include "Shadow.h"
#include "Debug.h"
using namespace ci;
using namespace ci::app;

EnemySP Enemy::origin;

void Enemy::SetOrigin()
{
	origin = EnemySP(new Enemy());
	origin->model.Init("mesh/tire.obj", "texture/target4.png");
}

void Vec3ToAxisAngle(Vec3f* pAx, float& angle, const Vec3f* pV1, const Vec3f* pV2);
void TransformFromJson(Transform& trans, const JsonTree& json);
Vec3f GetVec3(const JsonTree& json, const std::string& name);

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
Enemy::Enemy() : cnt(0), isDeath(false), camera(nullptr),
	usesCollider(false), drawsCollider(false), dir(0.f, 0.f, 0.f), spd(0.f),
	hp(1.f), type(0), limit(1000)
{}

Enemy::~Enemy(){}
	
//=======================================================================================
//生成・破棄
//=======================================================================================
Enemy* Enemy::Create()
{
	Enemy* obj = new Enemy();

	//mesh and texture
	obj->model.Init("mesh/tire.obj", "texture/target4.png");

	//ライト-------------------------------------------------------------------------------------------
	//マテリアル---------------------------------------------------------------------------------------

	return obj;
}

EnemySP Enemy::Create(const ci::JsonTree& json)
{
	
	EnemySP obj = EnemySP(new Enemy());

	//Transform
	TransformFromJson(obj->trans, json);

	//Model
	/*auto meshPath = json["MeshName"].getValue();
	auto texPath = json["TexName"].getValue();
	obj->model.Init(meshPath, texPath);*/
	obj->model = origin->model;

	//Collision
	JsonTree AABB = json["AABB"];
	float x = AABB.getValueAtIndex<float>(0);
	float y = AABB.getValueAtIndex<float>(1);
	float z = AABB.getValueAtIndex<float>(2);
	obj->aabb.pos = obj->trans.pos;
	obj->aabb.whl = Vec3f(x, y, z);

	obj->sphere.c = obj->trans.pos;
	obj->sphere.r = json.getValueForKey<float>("Rad");

	obj->usesCollider = true;
	obj->drawsCollider = true;

	obj->type = json.getValueForKey<int>("Type");
	obj->waitTime = json.getValueForKey<int>("Time");

	obj->dir = GetVec3(json, "Dir");
	obj->spd = json.getValueForKey<float>("Spd");

	obj->limit = json.getValueForKey<float>("Limit");

	return obj;
}

void Enemy::Destroy( Enemy* obj)
{
	delete obj;
}

//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void Enemy::Setup()
{
}

void Enemy::ShutDown()
{
}
	
void Enemy::Update()
{
	//試し
	//trans.rot *= Matrix44f::createRotation(Vec3f::xAxis(), toRadians(-3.0f));
	trans.pos += dir * spd;

	if (cnt > limit) isDeath = true;

	UpdateCollision();
	cnt++;
}

void Enemy::Draw()
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

	Vec3f sSize = Vec3f(1.5f, 1.5f, 1.0f);

	//影
	Shadow::Draw(*camera, trans.pos, sSize);

	//コリジョン
	if (drawsCollider && Debug::drawsColliders) DrawCollision();
}

//=======================================================================================
//その他
//=======================================================================================
void Enemy::SetRedBullet()
{
	
}

//=======================================================================================
//コリジョン
//=======================================================================================
void Enemy::CreateCollision(const ci::JsonTree& json)
{

}

void Enemy::DestroyCollision() 
{

}

void Enemy::UpdateCollision() 
{
	aabb.pos = trans.pos;
	sphere.c = trans.pos;
}

void Enemy::DrawCollision() 
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
	gl::scale(Vec3f::one());

	gl::color(1, 0, 0);

	//gl::drawStrokedCube(Vec3f(0, 0, 0), Vec3f(1, 1, 1));
	gl::drawSphere(Vec3f(0, 0, 0), sphere.r);

	gl::color(1, 1, 1);

	gl::lineWidth(1);
}

