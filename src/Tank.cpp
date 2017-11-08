#include "Tank.h"

#include "cinder/app/AppNative.h"
#include "cinder/Camera.h"

#include "KeyManager.h"
#include "Math.h"

using namespace ci;
using namespace ci::app;

void TransformFromJson(Transform& trans, const JsonTree& json);
Vec3f GetVec3(const JsonTree& json, const std::string& name);

//=======================================================================================
//コンストラクタ、ディストラクタ
//=======================================================================================
Tank::Tank() : cnt(0), leftTireRot(0.0f), rightTireRot(0.0f), gunRot(0.0f), gunTurretRot(0.0f),
	mov(0.0f), spd(0.f), movDir(0.f, 0.f, 0.f), force(0.f, 0.f, 0.f), rot(0,0,0),
	hp(0.f), isDeath(false),isInvincible(false),can_shot(false), do_shot(0), bulSpd(1.0f)
{

}

Tank::~Tank()
{

}

//=======================================================================================
//生成・破棄
//=======================================================================================
Tank* Tank::Create()
{
	Tank* obj = nullptr;

	return obj;
}

//×
Tank* Tank::Create(const ci::Vec3f& pos, const std::string& texturePath, const std::string& meshPath)
{
	Tank* obj = new Tank();

	//Transform
	obj->trans.pos = pos;

	//modelのLightとmaterialを生成
	obj->model.Init(Model::LIGHT_AND_MATERIAL);

	//Model
	//bullet
	return obj;
}

Tank* Tank::Create(const ci::Vec3f& pos)
{
	Tank* obj = new Tank();

	//Transform
	obj->trans = Transform();
	obj->trans.pos = pos;
	obj->trans.rot = Matrix44f::createRotation(Vec3f::yAxis(), toRadians(30.0f));

	//モデル
	//obj->model = Model("mesh/tank.obj", "texture/white.png");
	obj->model.Init(Model::LIGHT_AND_MATERIAL);

	//ライト-------------------------------------------------------------------------------------------
	// 環
	obj->model.light->setAmbient(Color(0.0f, 0.0f, 1.0f));
	// 拡
	obj->model.light->setDiffuse(Color(0.6f, 0.6f, 0.6f));
	// 鏡	 .
	obj->model.light->setSpecular(Color(0.9f, 0.9f, 0.9f));
	// 光	 .
	obj->model.light->setDirection(Vec3f(0.0f, -1.0f, 0.0f));

	//マテリアル---------------------------------------------------------------------------------------
	// 環
	obj->model.material->setAmbient(Color(0.2f, 0.2f, 0.8f));
	// 拡	 .
	obj->model.material->setDiffuse(Color(0.4f, 0.4f, 0.4f));
	// 鏡	 .
	obj->model.material->setSpecular(Color(1.0f, 1.0f, 1.0f));
	// 鏡面反射の鋭さ
	obj->model.material->setShininess(0.0f);
	// 光
	obj->model.material->setEmission(Color(0.1f, 0.1f, 0.1f));


	//Parts-------------------------------------------------------------------------------------------

	// 	BODY,
	PartsSP parts_ = Parts::CreateMeshAndTextureSP("mesh/tank/body.obj", "texture/white.png");
	parts_->trans.pos = { 0.0f, 0.8f, 0.0f };
	parts_->trans.rot = Matrix44f::identity();
	parts_->trans.scl = Vec3f(1, 1, 1);
	obj->parts.push_back(parts_);

	// 	GUN_TURRET,
	parts_ = Parts::CreateMeshAndTextureSP("mesh/tank/gun_turret.obj", "texture/white.png");
	parts_->trans.pos = { 0.0f, 1.1f, 0.2f };
	parts_->trans.rot = Matrix44f::identity();
	parts_->trans.scl = Vec3f(1, 1, 1);
	obj->parts.push_back(parts_);

	// 	GUN,
	parts_ = Parts::CreateMeshAndTextureSP("mesh/tank/gun.obj", "texture/white.png");
	parts_->trans.pos = { 0.0f, 0.2f, -0.8f };
	parts_->trans.rot = Matrix44f::identity();
	parts_->trans.scl = Vec3f(1, 1, 1);
	obj->parts.push_back(parts_);

	//-----------------------------------------------------------------------------------
	//tire
	//float width = 1.0f;

	// 	LEFT_TIRE
	for (int i = LEFT_TIRE_1; i <= LEFT_TIRE_4; ++i)
	{
		float v = 0.5f * float(i-3);
		parts_ = Parts::CreateMeshAndTextureSP("mesh/tire.obj", "texture/tire.png");
		parts_->trans.pos = { 0.6f, 0.3f, v - 0.7f };
		parts_->trans.rot = Matrix44f::createRotation(Vec3f::yAxis(), toRadians(90.0f));
		parts_->trans.scl = Vec3f(1, 1, 1);
		obj->parts.push_back(parts_);
	}

	for (int i = RIGHT_TIRE_1; i <= RIGHT_TIRE_4; ++i)
	{
		float v = 0.5f * float(i-7);
		parts_ = Parts::CreateMeshAndTextureSP("mesh/tire.obj", "texture/tire.png");
		parts_->trans.pos = { -0.6f, 0.3f, v - 0.7f };
		parts_->trans.rot = Matrix44f::createRotation(Vec3f::yAxis(), toRadians(90.0f));
		parts_->trans.scl = Vec3f(1, 1, 1);
		obj->parts.push_back(parts_);
	}


	//bullet-------------------------------------------------------------------------------------------
	obj->bullet = Bullet::CreateSP();
	obj->bullet->type = Bullet::PLAYER;
	obj->bullet->power = 10.0f;

	//コリジョン------------------------------------------------------------------------------------------------

	//初期ステータス-------------------------------------------------------------------------------------------------
	obj->SetupStatus();

	return obj;
}

//モデるファイルとファイルパス
TankSP Tank::CreateSP(const ci::Vec3f& pos)
{
	TankSP tank = TankSP(Create(pos));
	return tank;
}

TankSP Tank::Create(const JsonTree& data)
{
	TankSP obj = TankSP(new Tank());


	//Transform==========================================================================
	{
		/*JsonTree Pos = data["Player.Pos"];
		obj->trans.pos.x = Pos.getValueAtIndex<float>(0);
		obj->trans.pos.y = Pos.getValueAtIndex<float>(1);
		obj->trans.pos.z = Pos.getValueAtIndex<float>(2);

		JsonTree Rot = data["Player.Rot"];
		float x = Rot.getValueAtIndex<float>(0);
		float y = Rot.getValueAtIndex<float>(1);
		float z = Rot.getValueAtIndex<float>(2);
		obj->trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

		JsonTree Scl = data["Player.Scl"];
		obj->trans.scl.x = Scl.getValueAtIndex<float>(0);
		obj->trans.scl.y = Scl.getValueAtIndex<float>(1);
		obj->trans.scl.z = Scl.getValueAtIndex<float>(2);*/

		JsonTree data2 = data["Player"];
		JsonTree Pos = data2["Pos"];
		obj->trans.pos.x = Pos.getValueAtIndex<float>(0);
		obj->trans.pos.y = Pos.getValueAtIndex<float>(1);
		obj->trans.pos.z = Pos.getValueAtIndex<float>(2);

		JsonTree Rot = data2["Rot"];
		float x = Rot.getValueAtIndex<float>(0);
		float y = Rot.getValueAtIndex<float>(1);
		float z = Rot.getValueAtIndex<float>(2);
		obj->trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

		JsonTree Scl = data2["Scl"];
		obj->trans.scl.x = Scl.getValueAtIndex<float>(0);
		obj->trans.scl.y = Scl.getValueAtIndex<float>(1);
		obj->trans.scl.z = Scl.getValueAtIndex<float>(2);
	}
	//Model==========================================================================
	{
		obj->model.Init(Model::LIGHT_AND_MATERIAL);

		//Light
		JsonTree Ambient = data["Player.Ambient"];
		float x = Ambient.getValueAtIndex<float>(0);
		float y = Ambient.getValueAtIndex<float>(1);
		float z = Ambient.getValueAtIndex<float>(2);
		obj->model.light->setAmbient(Color(x, y, z));

		JsonTree Diffuse = data["Player.Diffuse"];
		x = Diffuse.getValueAtIndex<float>(0);
		y = Diffuse.getValueAtIndex<float>(1);
		z = Diffuse.getValueAtIndex<float>(2);
		obj->model.light->setDiffuse(Color(x, y, z));

		JsonTree Direction = data["Player.Direction"];
		x = Direction.getValueAtIndex<float>(0);
		y = Direction.getValueAtIndex<float>(1);
		z = Direction.getValueAtIndex<float>(2);
		obj->model.light->setDirection(Vec3f(x, y, z));

		//Material
		JsonTree _Ambient = data["Player._Ambient"];
		x = _Ambient.getValueAtIndex<float>(0);
		y = _Ambient.getValueAtIndex<float>(1);
		z = _Ambient.getValueAtIndex<float>(2);
		float w = _Ambient.getValueAtIndex<float>(3);
		obj->model.material->setAmbient(ColorA(x, y, z, w));

		JsonTree _Diffuse = data["Player._Diffuse"];
		x = _Diffuse.getValueAtIndex<float>(0);
		y = _Diffuse.getValueAtIndex<float>(1);
		z = _Diffuse.getValueAtIndex<float>(2);
		w = _Diffuse.getValueAtIndex<float>(3);
		obj->model.material->setDiffuse(ColorA(x, y, z, w));

		JsonTree _Specular = data["Player._Specular"];
		x = _Specular.getValueAtIndex<float>(0);
		y = _Specular.getValueAtIndex<float>(1);
		z = _Specular.getValueAtIndex<float>(2);
		w = _Specular.getValueAtIndex<float>(3);
		obj->model.material->setSpecular(ColorA(x, y, z, w));

		//JsonTree _Shininess = data["Player._Shininess"];
		//float s = _Shininess.getValueForKey<float>("shininess");
		JsonTree _Shininess = data["Player"];
		float s = _Shininess.getValueForKey<float>("_Shininess");
		obj->model.material->setShininess(s);

		JsonTree _Emission = data["Player._Emission"];
		x = _Emission.getValueAtIndex<float>(0);
		y = _Emission.getValueAtIndex<float>(1);
		z = _Emission.getValueAtIndex<float>(2);
		w = _Emission.getValueAtIndex<float>(3);
		obj->model.material->setEmission(ColorA(x, y, z, w));
	}

	//Parts==============================================================================
	// 	BODY
	{
		JsonTree Body = data["Parts"][0];
		auto meshPath = Body["MeshName"].getValue();
		auto texPath = Body["TexName"].getValue();
		PartsSP parts_ = Parts::CreateMeshAndTextureSP( meshPath, texPath);

		parts_->trans.pos.x = Body["Pos"].getValueAtIndex<float>(0);
		parts_->trans.pos.y = Body["Pos"].getValueAtIndex<float>(1);
		parts_->trans.pos.z = Body["Pos"].getValueAtIndex<float>(2);

		//params["material"][0]["ambient"]
		/*float x = data["Parts"][0]["Pos"].getValueAtIndex<float>(0);
		float y = data["Parts"][0]["Pos"].getValueAtIndex<float>(1);
		float z = data["Parts"][0]["Pos"].getValueAtIndex<float>(2);*/

		float x = Body["Rot"].getValueAtIndex<float>(0);
		float y = Body["Rot"].getValueAtIndex<float>(1);
		float z = Body["Rot"].getValueAtIndex<float>(2);
		parts_->trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

		parts_->trans.scl.x = Body["Scl"].getValueAtIndex<float>(0);
		parts_->trans.scl.y = Body["Scl"].getValueAtIndex<float>(1);
		parts_->trans.scl.z = Body["Scl"].getValueAtIndex<float>(2);

		obj->parts.push_back(parts_);
	}
	// GunTurret
	{
		JsonTree GunTurret = data["Parts"][1];
		auto meshPath = GunTurret["MeshName"].getValue();
		auto texPath = GunTurret["TexName"].getValue();
		PartsSP parts_ = Parts::CreateMeshAndTextureSP(meshPath, texPath);

		parts_->trans.pos.x = GunTurret["Pos"].getValueAtIndex<float>(0);
		parts_->trans.pos.y = GunTurret["Pos"].getValueAtIndex<float>(1);
		parts_->trans.pos.z = GunTurret["Pos"].getValueAtIndex<float>(2);

		float x = GunTurret["Rot"].getValueAtIndex<float>(0);
		float y = GunTurret["Rot"].getValueAtIndex<float>(1);
		float z = GunTurret["Rot"].getValueAtIndex<float>(2);
		parts_->trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

		parts_->trans.scl.x = GunTurret["Scl"].getValueAtIndex<float>(0);
		parts_->trans.scl.y = GunTurret["Scl"].getValueAtIndex<float>(1);
		parts_->trans.scl.z = GunTurret["Scl"].getValueAtIndex<float>(2);

		obj->parts.push_back(parts_);
	}
	// Gun
	{
		JsonTree Gun = data["Parts"][2];
		auto meshPath = Gun["MeshName"].getValue();
		auto texPath = Gun["TexName"].getValue();
		PartsSP parts_ = Parts::CreateMeshAndTextureSP(meshPath, texPath);

		parts_->trans.pos.x = Gun["Pos"].getValueAtIndex<float>(0);
		parts_->trans.pos.y = Gun["Pos"].getValueAtIndex<float>(1);
		parts_->trans.pos.z = Gun["Pos"].getValueAtIndex<float>(2);

		float x = Gun["Rot"].getValueAtIndex<float>(0);
		float y = Gun["Rot"].getValueAtIndex<float>(1);
		float z = Gun["Rot"].getValueAtIndex<float>(2);
		parts_->trans.rot = Matrix44f::createRotation( Vec3f(toRadians(x), toRadians(y), toRadians(z) ));

		parts_->trans.scl.x = Gun["Scl"].getValueAtIndex<float>(0);
		parts_->trans.scl.y = Gun["Scl"].getValueAtIndex<float>(1);
		parts_->trans.scl.z = Gun["Scl"].getValueAtIndex<float>(2);

		obj->parts.push_back(parts_);
	}

	{
		//Left Tire
		JsonTree Tire = data["Parts"][3];
		auto meshPath = Tire["MeshName"].getValue();
		auto texPath = Tire["TexName"].getValue();

		Vec3f pos, scl;

		pos.x = Tire["Pos"].getValueAtIndex<float>(0);
		pos.y = Tire["Pos"].getValueAtIndex<float>(1);
		pos.z = Tire["Pos"].getValueAtIndex<float>(2);

		float x = Tire["Rot"].getValueAtIndex<float>(0);
		float y = Tire["Rot"].getValueAtIndex<float>(1);
		float z = Tire["Rot"].getValueAtIndex<float>(2);
		Matrix44f rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

		scl.x = Tire["Scl"].getValueAtIndex<float>(0);
		scl.y = Tire["Scl"].getValueAtIndex<float>(1);
		scl.z = Tire["Scl"].getValueAtIndex<float>(2);

		//_Shininess.getValueForKey<float>("_Shininess");
		float width = Tire.getValueForKey<float>("Width");

		for (int i = LEFT_TIRE_1; i <= LEFT_TIRE_4; ++i)
		{
			float v = width * float(i - 3);
			pos.z = v - 0.7f;
			PartsSP parts_ = Parts::CreateMeshAndTextureSP(meshPath, texPath);
			parts_->trans.pos = pos;
			parts_->trans.rot = rot;
			parts_->trans.scl = scl;
			obj->parts.push_back(parts_);
		}

		pos.x = -pos.x;
		for (int i = RIGHT_TIRE_1; i <= RIGHT_TIRE_4; ++i)
		{
			float v = 0.5f * float(i - 7);
			pos.z = v - 0.7f;
			PartsSP parts_ = Parts::CreateMeshAndTextureSP(meshPath, texPath);
			parts_->trans.pos = pos;
			parts_->trans.rot = rot;
			parts_->trans.scl = scl;
			obj->parts.push_back(parts_);
		}
	}

	//==================================================================================
	//その他のパラメータ
	obj->bulSpd = data.getValueForKey<float>("Player.BulSpd");

	//==================================================================================
	//Bullet生成
	//JsonTree Bullet = ;
	obj->bullet = Bullet::Create(data["Bullet"]);

	return obj;
}


void Tank::Destroy( Tank* obj)
{
}


//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================
void Tank::Setup()
{

}

void Tank::ShutDown()
{

}
	
void Tank::Update()
{

	//※とりえず動く
	//trans.pos.z += 0.01f;
	//trans.rot *= Matrix44f::createRotation(Vec3f::yAxis(), toRadians(1.0f));

	//RotateLeftTire(toRadians(3.0f));
	//RotateRightTire(toRadians(-3.0f));


	//debugモード-----------------------------------------------------------------------

	//速度変更



	//----------------------------------------------------------------------------------

	//移動------------------------------------------------------------------------------
	float mov_spd = 0.2f;
	Vec3f local_dir = { 0,0,0 };
	bool isMoved = false;

	//mov_dir, pos, cameragetEyepoint(),mov.dir normalize() mov_spd

	if (KeyManager::GetKey('W'))
	{
		local_dir.z += 1.0f;
	}
	if (KeyManager::GetKey('S'))
	{
		local_dir.z += -1.0f;
	}
	if (KeyManager::GetKey('A'))
	{
		local_dir.x += -1.0f;
	}
	if (KeyManager::GetKey('D'))
	{
		local_dir.x += 1.0f;
	}

	//Player 砲塔旋回

	//回転------------------------------------------------------------------------------
	//砲塔回転
	//parts[GUN_TURRET]->trans.rot *= Matrix44f::createRotation(Vec3f::yAxis(), toRadians(0.4f));

	//射撃------------------------------------------------------------------------------
	Shot();

	//衝突判定と補正-------------------------------------------------------------------
	//衝突判定の更新

	++cnt;
}

void Tank::Draw()
{

	//自身の表示
	DrawTank();
	
	//bullet->Draw();

}

void Tank::DrawTank()
{

	//マテリアル
	model.material->apply();
	gl::enable(GL_CULL_FACE);

	//カメラ
	//body-----------------------------------------------------------
	gl::setMatrices(*camera);


	// ライトを有効にする
	gl::enable(GL_LIGHTING);
	gl::enable(GL_NORMALIZE);
	model.light->enable();

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::translate(parts[BODY]->trans.pos);
	gl::rotate(parts[BODY]->trans.rot);

	gl::scale(trans.scl);

	parts[BODY]->model.texture->enableAndBind();
	gl::draw(*parts[BODY]->model.mesh);
	parts[BODY]->model.texture->disable();

	//gunTurret------------------------------------------------------
	gl::setMatrices(*camera);

	gl::translate(trans.pos);
	gl::rotate(trans.rot);

	gl::translate(parts[GUN_TURRET]->trans.pos);
	gl::rotate(parts[GUN_TURRET]->trans.rot);

	gl::scale(trans.scl);

	parts[GUN_TURRET]->model.texture->enableAndBind();
	gl::draw(*parts[GUN_TURRET]->model.mesh);
	parts[GUN_TURRET]->model.texture->disable();

	//gun -----------------------------------------------------------
	gl::setMatrices(*camera);

	gl::translate(trans.pos);
	gl::rotate(trans.rot);
	gl::translate(parts[GUN_TURRET]->trans.pos);
	gl::rotate(parts[GUN_TURRET]->trans.rot);
	gl::translate(parts[GUN]->trans.pos);
	gl::rotate(parts[GUN]->trans.rot);

	gl::scale(trans.scl);

	parts[GUN]->model.texture->enableAndBind();
	gl::draw(*parts[GUN]->model.mesh);
	parts[GUN]->model.texture->disable();

	//ライトを無効
	model.light->disable();
	gl::disable(GL_NORMALIZE);
	gl::disable(GL_LIGHTING);


	//tire------------------------------------------------------------
	for (int i = LEFT_TIRE_1; i <= LEFT_TIRE_4; ++i)
	{
		//left tire
		gl::setMatrices(*camera);

		gl::translate(trans.pos);
		gl::rotate(trans.rot);
		gl::scale(trans.scl);
		gl::translate(parts[i]->trans.pos);
		gl::rotate(parts[i]->trans.rot);


		parts[i]->model.texture->enableAndBind();
		gl::draw(*parts[i]->model.mesh);
		parts[i]->model.texture->disable();
	}

	for (int i = RIGHT_TIRE_1; i <= RIGHT_TIRE_4; ++i)
	{
		//right tire
		gl::setMatrices(*camera);

		gl::translate(trans.pos);
		gl::rotate(trans.rot);
		gl::translate(parts[i]->trans.pos);
		gl::rotate(parts[i]->trans.rot);

		gl::scale(trans.scl);

		parts[i]->model.texture->enableAndBind();
		gl::draw(*parts[i]->model.mesh);
		parts[i]->model.texture->disable();
	}
	//---------------------------------------------------------------

}

//=======================================================================================
//コリジョン
//=======================================================================================


//=======================================================================================
//その他
//=======================================================================================

void Tank::SetupStatus()
{

	mov = 0.0f;
	spd = 0.2f;
	movDir = Vec3f::zAxis();

	hp = MAX_HP;

}

void Tank::Shot()
{

	if (do_shot != 1 ) return;

	//弾の移動方向
	Vec3f v = -ToVector((trans.rot * parts[GUN_TURRET]->trans.rot * parts[GUN]->trans.rot));

	//弾の発射座標
	Matrix44f m = Matrix44f::createTranslation(trans.pos) * trans.rot
		* Matrix44f::createTranslation(parts[GUN_TURRET]->trans.pos) * parts[GUN_TURRET]->trans.rot
		* Matrix44f::createTranslation(parts[GUN]->trans.pos) * parts[GUN]->trans.rot;

	//auto bullet_ = Bullet::CreateClone(m.transformPointAffine(Vec3f(0.0f, 0.0f, -2.0f)), v, 0.5f, bullet, camera);
	//push backできない;

	(*bulletCreate)(m.transformPointAffine(Vec3f(0.0f, 0.0f, -2.0f)), v, bulSpd);

	do_shot = 0;

}

void Tank::RotateLeftTire(float ang)
{
	Matrix44f rot = Matrix44f::createRotation(Vec3f::zAxis(), -ang);

	for (int i = LEFT_TIRE_1; i <= LEFT_TIRE_4; ++i)
	{
		parts[i]->trans.rot *= rot;
	}
}

void Tank::RotateRightTire(float ang)
{
	Matrix44f rot = Matrix44f::createRotation(Vec3f::zAxis(), -ang);

	for (int i = RIGHT_TIRE_1; i <= RIGHT_TIRE_4; ++i)
	{
		parts[i]->trans.rot *= rot;
	}
}