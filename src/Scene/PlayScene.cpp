#include "PlayScene.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "GameManager.h"
#include "../KeyManager.h"
#include "Scene.h"

#include "cinder/Json.h"
#include "../Debug.h"

#include "../SoundsManager.h"
#include "../Math.h"

using namespace ci;
using namespace ci::app;

Color ToFloatColor(int r, int g, int b);

std::map<std::string, std::shared_ptr<ci::TriMesh>> Model::meshes;

Vec3f GetPos(const JsonTree& json)
{
	JsonTree Pos = json["Pos"];
	Vec3f pos;
	pos.x = Pos.getValueAtIndex<float>(0);
	pos.y = Pos.getValueAtIndex<float>(1);
	pos.z = Pos.getValueAtIndex<float>(2);

	return pos;
}

Vec3f GetVec3(const JsonTree& json, const std::string& name)
{
	JsonTree Vec = json[name];
	Vec3f vec;
	vec.x = Vec.getValueAtIndex<float>(0);
	vec.y = Vec.getValueAtIndex<float>(1);
	vec.z = Vec.getValueAtIndex<float>(2);

	return vec;
}

Matrix44f GetRotEular(const JsonTree& json, const std::string& name)
{
	JsonTree Rot = json[name];
	float x = Rot.getValueAtIndex<float>(0);
	float y = Rot.getValueAtIndex<float>(1);
	float z = Rot.getValueAtIndex<float>(2);
	Matrix44f rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

	return rot;
}

void TransformFromJson(Transform& trans, const JsonTree& json)
{
	JsonTree Pos = json["Pos"];
	trans.pos.x = Pos.getValueAtIndex<float>(0);
	trans.pos.y = Pos.getValueAtIndex<float>(1);
	trans.pos.z = Pos.getValueAtIndex<float>(2);

	JsonTree Rot = json["Rot"];
	float x = Rot.getValueAtIndex<float>(0);
	float y = Rot.getValueAtIndex<float>(1);
	float z = Rot.getValueAtIndex<float>(2);
	trans.rot = Matrix44f::createRotation(Vec3f(toRadians(x), toRadians(y), toRadians(z)));

	JsonTree Scl = json["Scl"];
	trans.scl.x = Scl.getValueAtIndex<float>(0);
	trans.scl.y = Scl.getValueAtIndex<float>(1);
	trans.scl.z = Scl.getValueAtIndex<float>(2);
}

//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
PlayScene::PlayScene() : end(false), cnt(0)
{

	JsonTree SceneData(loadAsset("json/play_scene_data.json"));
	JsonTree Scene_ = SceneData["Scene"];

	// UI�p�J����
	uiCamera = new ci::CameraPersp(GameManager::WindowSize::Width, GameManager::WindowSize::Height, 60.0f, 1.0f, 1000.0f);
	uiCamera->setEyePoint(Vec3f(0.f, 0.f, 0.f));
	uiCamera->setCenterOfInterestPoint(Vec3f(0.f, 0.f, 10.f));


	//�J����
	cameraTgt = GetVec3(Scene_, "Pos");
	camera = CameraTPS(&cameraTgt);
	JsonTree Camera = SceneData["Camera"];
	camera.Setup(Camera);
	
	//camera.Setup(SceneData["Camera"]);	//���f�o�b�N�f�[�^������


	JsonTree Ground = SceneData["Ground"];
	CreateGrounds(Ground);
	CreateObjects();

	JsonTree EnemyData(loadAsset("json/enemies_data.json"));
	JsonTree Enemy = EnemyData["Enemy"];
	CreateEnemies( Enemy );

	//�R�[���o�b�N
	bulletCreate.connect(
		std::bind(&PlayScene::CreateBullet, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	//tank
	JsonTree TankData(loadAsset("json/player_data.json"));
	tank = Tank::Create(TankData);
	tank->SetCamera(camera.camera);
	tank->bullet->SetCamera(camera.camera);
	tank->bulletCreate = &bulletCreate;

	//plane
	JsonTree Plane = SceneData["Plane"];
	planeOrigin = GetVec3(Plane, "Origin");
	planeNormal = GetVec3(Plane, "Normal");

	//UI
	JsonTree UI = SceneData["UIObj"][0];
	uiCursor = UIObject::Create(UI);
	//uiCursor->SetCamera(uiCamera);
	uiCursor->SetCamera(camera.camera);
	//uiCursor->isInvisible = true;

	// ���s���e�J�����̏�����
	cameraOrtho = new CameraOrtho(0, GameManager::Width,        // ��ʍ��̍��W�A��ʉE�̍��W
			GameManager::Height, 0,       // ��ʉ��[�̍��W�A��ʏ�[�̍��W
			1, 10);                 // ��ʎ�O�̍��W�A��ʉ��̍��W

	cameraOrtho->setEyePoint(Vec3f(0.0f, 0.0f, 0.0f));
	cameraOrtho->setCenterOfInterestPoint(Vec3f(0.0f, 0.0f, -1000.0f));

	// �t�H���g���쐬
	font = new Font("Hiragino Maru Gothic ProN W4", 50.0f);
	//font = new Font("Hiragino Mincho ProN", 30.0f);

	//20�b�@20*60 = 1200
	limit = Scene_.getValueForKey<int>("Limit");
	score = 0;

	//shot
	reload = 0;

	end_cnt = 0;

	shiftsTitleScene = false;

	//�`��̃Z�b�e�B���O
	gl::enableDepthRead();
	gl::enableDepthWrite();
	gl::enable(GL_CULL_FACE);

	//PogePlayBGM(PogeBGM_PLAY);

}

PlayScene::~PlayScene()
{
	DestroyGrounds();
	DestroyObjects();
	DestroyEnemies(); 
	DestroyBullets();
	waitingEnemies.clear();
}

//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================

void PlayScene::Update()
{
	if (limit > 0)
	{
		camera.Update();
		if (reload > 0) --reload;

		UpdateGrounds();
		UpdateObjects();
		UpdateEnemies();
		UpdateBullets();
		
		//�C������
		{
			Vec3f r = ToRotationXY(&rayIntersection);
			tank->parts[Tank::GUN_TURRET]->trans.rot = Matrix44f::createRotation(Vec3f::yAxis(), r.y);
			tank->parts[Tank::GUN]->trans.rot = Matrix44f::createRotation(Vec3f::xAxis(), -r.x);
		}

		tank->Update();

		Collide();
		Event();
		UpdateUI();
		--limit;
	}
	else
	{
		end_cnt++;
	}

	cnt++;
}

void PlayScene::Draw() 
{

	gl::clear(ToFloatColor(188, 226, 232));		//��F

	DrawGrounds();
	DrawObjects();
	DrawEnemies();
	DrawBullets();
	tank->Draw();
	DrawUI();

	//�X�R�A�Ǝ���
	{
		DrawString(20, 30, 1.0f, "limit : %d ", limit / 60);
		DrawString(20, 80, 1.0f, "score : %d ", score);
	}

	//debug
	if (Debug::drawsDebugLog)
	{
		//Debug::DrawString(10, 10, 1.0f, " CamX : %f CamZ : %f", camera.camera->getEyePoint().x, camera.camera->getEyePoint().z);
		Debug::DrawString(10, 10, 1.0f, " mdx : %f mdy : %f",
			mouseRay.getDirection().x, mouseRay.getDirection().y);
		Debug::DrawString(10, 40, 1.0f, " inscX : %f", rayIntersection.x);
		Debug::DrawString(10, 70, 1.0f, " inscY : %f", rayIntersection.y);
		Debug::DrawString(10,100, 1.0f, " inscX : %f", rayIntersection.z);
	}

}

void PlayScene::Shift()
{

	if (KeyManager::GetKeyDown('1'))
	{
		Scene::Create<TitleScene>(new TitleScene());
	}

	if (KeyManager::GetKeyDown('2'))
	{
		Scene::Create<PlayScene>(new PlayScene());
	}

	if (shiftsTitleScene)
	{
		Scene::Create<TitleScene>(new TitleScene());
	}

}

void PlayScene::MouseDown(MouseEvent event)
{
	
	if (event.isLeftDown())
	{
		tank->do_shot = -1;
		//uiCursor->isInvisible = false;
		console() << "charge." << std::endl;
	}
	if (event.isLeftDown() && end_cnt >= 120)
	{
		shiftsTitleScene = true;
	}
}

void PlayScene::MouseUp(MouseEvent event) 
{
	if (event.isLeft() && tank->do_shot == -1)
	{
		tank->do_shot = 1;
		//uiCursor->isInvisible = true;
		console() << "shot" << std::endl;
	}
}

void PlayScene::MouseMove(MouseEvent event) 
{
	//mouseRay
	//if (!event.isLeft()) return;
	Vec2f pos = event.getPos();

	// �}�E�X���W��(0,0)~(1,1)�͈̔͂ɐ��K������
	// TIPS:�}�E�X�̍��W��Y�̉��������v���X
	//      �\�����W��Y�̏�������v���X
	//      �������t�Ȃ̂ŁA���̕␳�������Ȃ��Ă�
	float x = pos.x / float(GameManager::Width);
	float y = 1.0f - pos.y / float(GameManager::Height);

	// �}�E�X�ʒu�����ʂ̉��ɂ܂������L�т�Ray�𐶐�
	mouseRay = camera.camera->generateRay(x, y, camera.camera->getAspectRatio());
	float result;
	if (mouseRay.calcPlaneIntersection(planeOrigin, planeNormal, &result))
	{
	}
	rayIntersection = mouseRay.calcPosition(result);

}

void PlayScene::MouseDrag(MouseEvent event) 
{
	//mouseRay
	//if (!event.isLeft()) return;
	Vec2f pos = event.getPos();

	// �}�E�X���W��(0,0)~(1,1)�͈̔͂ɐ��K������
	// TIPS:�}�E�X�̍��W��Y�̉��������v���X
	//      �\�����W��Y�̏�������v���X
	//      �������t�Ȃ̂ŁA���̕␳�������Ȃ��Ă�
	float x = pos.x / float(GameManager::Width);
	float y = 1.0f - pos.y / float(GameManager::Height);

	// �}�E�X�ʒu�����ʂ̉��ɂ܂������L�т�Ray�𐶐�
	mouseRay = camera.camera->generateRay(x, y, camera.camera->getAspectRatio());
	float result;
	if (mouseRay.calcPlaneIntersection(planeOrigin, planeNormal, &result))
	{
	}
	rayIntersection = mouseRay.calcPosition(result);
}

void PlayScene::CreateBullet(ci::Vec3f& pos, ci::Vec3f& dir, float vel)
{
	if (reload > 0) return;
	//BulletSP obj_ = Bullet::CreateClone(pos, dir, vel, tank->bullet, camera.camera);
	//BulletSP obj_ = Bullet::CreateClone(mouseRay.getOrigin(), mouseRay.getDirection(), vel, tank->bullet, camera.camera);
	//BulletSP obj_ = Bullet::CreateClone(pos, mouseRay.getDirection(), vel, tank->bullet, camera.camera);
	BulletSP obj_ = Bullet::CreateClone(pos, (rayIntersection - pos).normalized(), vel, tank->bullet, camera.camera);
	bullets.push_back(obj_);

	//PogePlay(PogeSE::SE_SHOT);

	reload = 20;
}

//=======================================================================================
//Ground
//=======================================================================================
void PlayScene::CreateGrounds(const ci::JsonTree& json)
{

	JsonTree Ground1 = json[0];
	StageObjectSP obj_ = StageObject::Create(Ground1);
	obj_->SetCamera(camera.camera);
	grounds.push_back(obj_);

	/*JsonTree Ground2 = json[1];
	obj_ = StageObject::Create(Ground2);
	obj_->SetCamera(camera.camera);
	grounds.push_back(obj_);*/

#if 0
	Transform unitTrans;

	StageObjectSP obj = StageObject::CreateSP("mesh/plane.obj", "texture/chip3.bmp", unitTrans);
	obj->trans.scl = Vec3f::one() * 10.0f;
	obj->SetCamera(camera.camera);
	grounds.push_back(obj);

	obj = StageObject::CreateSP("mesh/cube.obj", "texture/bricks.jpg", unitTrans);
	obj->trans.rot = Matrix44f::createRotation(Vec3f::zAxis(), -toRadians(90.0f));
	obj->trans.pos = Vec3f(3.0f, 2.0f, 10.0f);
	obj->trans.scl = Vec3f(1.0f, 1.0f, 1.0f);
	obj->SetCamera(camera.camera);
	grounds.push_back(obj);
#endif

}

void PlayScene::DestroyGrounds() 
{
	grounds.clear();
}

void PlayScene::UpdateGrounds() 
{
	for (int i = 0; i < grounds.size(); ++i)
	{
		grounds.at(i)->Update();
	}
}

void PlayScene::DrawGrounds() 
{
	for (int i = 0; i < grounds.size(); ++i)
	{
		grounds.at(i)->Draw();
	}
}

//=======================================================================================
//Object
//=======================================================================================
void PlayScene::CreateObjects() {}
void PlayScene::DestroyObjects() {}
void PlayScene::UpdateObjects() {}
void PlayScene::DrawObjects() {}

//=======================================================================================
//Enemy
//=======================================================================================
void PlayScene::CreateEnemies(const ci::JsonTree& json)
{
	Enemy::SetOrigin();

	int num = json.getNumChildren();
	for (int i = 0; i < num; i++)
	{
		JsonTree Enemy = json[i];
		EnemySP obj = Enemy::Create(Enemy);
		obj->SetCamera(camera.camera);
		if (obj->waitTime > 0) waitingEnemies.push_back(obj);
		else enemies.push_back(obj);
	}


#if 0
	//target
	EnemySP obj = Enemy::CreateSP();
	//obj->trans.rot = Matrix44f::createRotation(Vec3f::xAxis(), toRadians(90.0f));
	obj->trans.pos = Vec3f(-10.0f, 8.0f, 30.0f );
	obj->trans.scl = Vec3f(5.0f, 5.0f, 1.0f);
	obj->SetCamera(camera.camera);
	enemies.push_back(obj);

	//target
	obj = Enemy::CreateSP();
	//obj->trans.rot = Matrix44f::createRotation(Vec3f::xAxis(), toRadians(90.0f));
	obj->trans.pos = Vec3f(0.0f, 8.0f, 30.0f);
	obj->trans.scl = Vec3f(5.0f, 5.0f, 1.0f);
	obj->SetCamera(camera.camera);
	enemies.push_back(obj);

	//target
	obj = Enemy::CreateSP();
	//obj->trans.rot = Matrix44f::createRotation(Vec3f::xAxis(), toRadians(90.0f));
	obj->trans.pos = Vec3f(10.0f, 8.0f, 30.0f);
	obj->trans.scl = Vec3f(5.0f, 5.0f, 1.0f);
	obj->SetCamera(camera.camera);
	enemies.push_back(obj);
#endif
}

void PlayScene::DestroyEnemies() 
{
	enemies.clear();
	enemyRemoveList.clear();
}

void PlayScene::UpdateEnemies() 
{
	auto it = enemies.begin();

	while (it != enemies.end())
	{
		(*it)->Update();
		if ((*it)->isDeath) enemyRemoveList.push_back(*it);
		++it;
	}

	auto dIt = enemyRemoveList.begin();
	while (dIt != enemyRemoveList.end())
	{
		enemies.remove(*dIt);
		dIt = enemyRemoveList.erase(dIt);
	}
}

void PlayScene::DrawEnemies() 
{
	auto it = enemies.begin();

	while (it != enemies.end())
	{
		(*it)->Draw();
		++it;
	}
}


//=======================================================================================
//Bullet
//=======================================================================================
void PlayScene::CreateBullets() 
{
	
}

void PlayScene::DestroyBullets() 
{
	bullets.clear();
	bulletRemoveList.clear();
}

void PlayScene::UpdateBullets()
{
	auto it = bullets.begin();

	while (it != bullets.end())
	{
		(*it)->Update();
		if ((*it)->isDeath)
		{
			bulletRemoveList.push_back(*it);
		}
		++it;
	}

	auto dIt = bulletRemoveList.begin();
	while (dIt != bulletRemoveList.end())
	{
		bullets.remove(*dIt);
		dIt = bulletRemoveList.erase(dIt);
	}
}

void PlayScene::DrawBullets()
{
	auto it = bullets.begin();

	while (it != bullets.end())
	{
		(*it)->Draw();
		++it;
	}
}

//=======================================================================================
//UI
//=======================================================================================
void PlayScene::CreateUI()
{

}

void PlayScene::DestroyUI()
{
	
}

void PlayScene::UpdateUI()
{
	//uiCursor->trans.pos = rayIntersection;

	float result;
	mouseRay.calcPlaneIntersection(Vec3f(0, 0, 1), Vec3f(0, 0, -1), &result);


	Vec3f zOne = mouseRay.calcPosition(result);
	//uiCursor->trans.pos.x = zOne.x * 1.0f;
	//uiCursor->trans.pos.y = zOne.y * 1.0f;
	uiCursor->trans.pos = zOne;

	uiCursor->trans.rot *= Matrix44f::createRotation(Vec3f::zAxis(), toRadians(5.0f));
	uiCursor->Update();
}

void PlayScene::DrawUI()
{
	uiCursor->Draw();
}

//=======================================================================================
//�Փˏ���
//=======================================================================================
void PlayScene::Collide()
{
	//enemy and bullet
	auto bIt = bullets.begin();
	while (bIt != bullets.end())
	{
		auto eIt = enemies.begin();
		/*if (!(*bIt)->usesCollider)
		{
			++bIt;
			continue;
		}
		*/

		while (eIt != enemies.end())
		{


			//Sphere vs Sphere
			if (Collision::CollideSphere((*bIt)->sphere, (*eIt)->sphere))
			//if (true)
			{
				//AABB vs AABB
				if (Collision::CollideAABB((*bIt)->aabb, (*eIt)->aabb))
				{
					//�������q�b�g�������
					//�e�������A�G�������A����炷�A�X�R�A���Z
					(*bIt)->isDeath = true;
					(*eIt)->isDeath = true;
					//SoundsManager::Play(PogeSE_EXPLOSION);
					AddScore(1);

				}
			}

			
			++eIt;
		}
		++bIt;
	}

}

//=======================================================================================
//
//=======================================================================================
Color ToFloatColor(int r, int g, int b)
{
	Color c = { (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f };
	return c;
}

void PlayScene::DrawString(float x, float y, float size, const std::string format, ...)
{
	Vec2f p = Vec2f(x, y);

	//������̍쐬-----------------------------------------
	char cstr[256] = u8"";
	va_list args;
	va_start(args, format);
	vsprintf(cstr, format.c_str(), args);
	va_end(args);
	std::string str = cstr;
	//-----------------------------------------------------
	// ���s���e�J�����ŕ`��
	gl::setMatrices(*cameraOrtho);
	
	gl::scale(size, size, 1.0f);
	gl::translate(0.0f, 0.0f, -5.0f);

	gl::drawString(str, p, Color(0.0f,0.0f, 0.0f), *font);

}

void PlayScene::Event()
{
	auto it_ = waitingEnemies.begin();
	while (it_ != waitingEnemies.end())
	{
		if ((*it_)->waitTime > 0)
		{
			--(*it_)->waitTime;
			it_++;
		}
		else
		{
			enemies.push_back(*it_);
			it_ = waitingEnemies.erase(it_);
		}
	}
}