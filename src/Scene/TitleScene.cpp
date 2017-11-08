#include "TitleScene.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "../KeyManager.h"
#include "../Debug.h"
#include "GameManager.h"
#include "Scene.h"

#include "../TitleObject2.h"
#include "../SoundsManager.h"

using namespace ci;
using namespace ci::app;

Color ToFloatColor(int r, int g, int b);



//=======================================================================================
//�R���X�g���N�^�A�f�B�X�g���N�^
//=======================================================================================
TitleScene::TitleScene() : shiftsPlayScene(false)
{

	JsonTree SceneData(loadAsset("json/title_scene_data.json"));
	JsonTree Scene_ = SceneData["Scene"];

	//Camera
	camera = CameraTPS();
	camera.Setup();
	camera.SetTPSCameraTgt(tgt);
	JsonTree Camera = SceneData["Camera"];
	camera.Setup(Camera);

	// ���s���e�J�����̏�����
	cameraOrtho = new CameraOrtho(	0, GameManager::WindowSize::Width,        // ��ʍ��̍��W�A��ʉE�̍��W
									GameManager::WindowSize::Height, 0,       // ��ʉ��[�̍��W�A��ʏ�[�̍��W
									1, 10);                 // ��ʎ�O�̍��W�A��ʉ��̍��W
	
	cameraOrtho->setEyePoint(Vec3f(0.0f, 0.0f, -1.0f));
	cameraOrtho->setCenterOfInterestPoint(Vec3f(0.0f, 0.0f, 1000.0f));

	// UI�p�J����
	cameraUI = new ci::CameraPersp(GameManager::WindowSize::Width, GameManager::WindowSize::Height, 60.0f, 1.0f, 1000.0f);
	cameraUI->setEyePoint(Vec3f(0.f, 0.f, 0.f));
	cameraUI->setCenterOfInterestPoint(Vec3f(0.f, 0.f, 10.f));

	//�I�u�W�F�N�g�̍쐬
	JsonTree Obj = SceneData["Object"];
	CreateObjects(Obj);

	//UI�̍쐬
	JsonTree UIObj = SceneData["UIObj"];
	CreateUI(UIObj);

	// �[�x�o�b�t�@
	gl::enableDepthRead();
	gl::enableDepthWrite();

	// �J�����O
	gl::enable(GL_CULL_FACE);

	// ���ߕ����̓��ߏ���
	gl::enableAlphaBlending();

	//SoundsManager::PlayBGM(SoundsManager::BGM_TITLE);
}

//�Ă΂�Ȃ���ΐ���
TitleScene::TitleScene(const TitleScene &obj)
{
	shiftsPlayScene = false;

	camera = obj.camera;
	camera.tgt = &tgt; 
	objects = obj.objects;
	objectUI = obj.objectUI;
	uiObjects = obj.uiObjects;
}

TitleScene::~TitleScene()
{
	DestroyObjects();
	DestroyUI();
}

//=======================================================================================
//��{����(�����������A�I�������A���W�b�N�����A�`�揈��)
//=======================================================================================
void TitleScene::Update()
{

	camera.Update();
   	camera.tgtRotY += 0.01f;

	UpdateObjects();
	UpdateUI();

}

void TitleScene::Draw() 
{
	//R:160 G:216 B:239
	gl::clear(ToFloatColor(160, 216, 239));

	//���f���̕`��--------------------------------------------------
	DrawObjects();

	//UI�̕`��--------------------------------------------------
	DrawUI();

	//debug
	if(Debug::drawsDebugLog)
	{
		Debug::DrawString(10,10, 1.0f, " CamX : %f CamZ : %f" , camera.camera->getEyePoint().x, camera.camera->getEyePoint().z);
		Debug::DrawString(10, 40, 1.0f, " tgtX : %f tgtZ : %f", tgt.x, tgt.z);
	}
}

void TitleScene::Shift()
{

	if (KeyManager::GetKeyDown('1'))
	{
		Scene::Create<TitleScene>(new TitleScene());
	}

	if (KeyManager::GetKeyDown('2'))
	{
		Scene::Create<PlayScene>(new PlayScene());
	}


	if (shiftsPlayScene)
	{
		Scene::Create<PlayScene>(new PlayScene());
	}

}

//=======================================================================================
//
//=======================================================================================

void TitleScene::CreateObjects(const ci::JsonTree& json)
{
	Transform unitTrans;

	JsonTree Obj = json[0];
	TitleObjectSP obj1 = TitleObject::Create(Obj);
	obj1->SetCamera(camera.camera);
	objects.push_back(obj1);

#if 0
	TitleObjectSP obj1 = TitleObject::CreateSP("mesh/plane.obj", "texture/rock.png", unitTrans);
	obj1->trans.scl *= 10.f;
	obj1->SetCamera(camera.camera);
	objects.push_back(obj1);
#endif


	TitleObject2SP obj2 = TitleObject2::CreateSP("mesh/tank_.obj", "texture/white.png", unitTrans);
	obj2->SetCamera(camera.camera);
	objects.push_back(obj2);

}

void TitleScene::DestroyObjects()
{
	objects.clear();
}

void TitleScene::UpdateObjects()
{
	auto it = objects.begin();
	while (it != objects.end())
	{
		(*it)->Update();
		++it;
	}
}

void TitleScene::DrawObjects()
{
	auto it = objects.begin();
	while (it != objects.end())
	{
		(*it)->Draw();
		++it;
	}
}

//=================================================================================================

void TitleScene::CreateUI(const ci::JsonTree& json)
{

	JsonTree UI1 = json[0];
	UIObjectSP obj = UIObject::Create(UI1);
	obj->SetCamera(cameraUI);
	uiObjects.push_back(obj);
	objectUI = obj;

	JsonTree UI2 = json[1];
	obj = UIObject::Create(UI2);
	obj->SetCamera(cameraUI);
	obj->EnableFlashing();
	uiObjects.push_back(obj);


#if 0
	Transform unitTrans;

	UIObjectSP obj = UIObject::CreateSP("mesh/plane.obj", "texture/limit_tank.png", unitTrans);
	obj->SetCamera(cameraUI);
	//�����s�ړ��A�g��k��
	/*obj->trans.pos = { 0.f, 2.f, 5.f };
	obj->trans.scl = { 6.f, 1.f, 1.f };*/
	obj->trans.pos = { 0.f, 2.f, 5.f };
	obj->trans.scl = { 2.f, 0.5f, 1.f };
	//��push_back
	uiObjects.push_back(obj);
	objectUI = obj;

	obj = UIObject::CreateSP("mesh/plane.obj", "texture/game_start.png", unitTrans);
	obj->SetCamera(cameraUI);
	//�����s�ړ��A�g��k��
	/*obj->trans.pos = { 0.f, -0.1f, 1.1f };
	obj->trans.scl = { 1.f, 0.2f, 1.f };*/
	obj->trans.pos = { 0.f, -0.f, 1.0f };
	obj->trans.scl = { 0.5f, 0.2f, 1.f };
	//�_�ł�����
	obj->EnableFlashing();
	uiObjects.push_back(obj);
	//objectUI = obj1;
#endif
}

void TitleScene::DestroyUI()
{
	uiObjects.clear();
}

void TitleScene::UpdateUI()
{
	auto it = uiObjects.begin();
	while (it != uiObjects.end())
	{
		(*it)->Update();
		++it;
	}
}

void TitleScene::DrawUI()
{
	auto it = uiObjects.begin();
	while (it != uiObjects.end())
	{
		(*it)->Draw();
		++it;
	}
}

//=================================================================================================

void TitleScene::MouseDown(MouseEvent event) 
{
	if (event.isLeft())
	{
		shiftsPlayScene = true;
		console() << "Left button." << std::endl;
	}
}

void TitleScene::MouseUp(MouseEvent event) 
{
	
}

void TitleScene::MouseMove(MouseEvent event) 
{

}

void TitleScene::MouseDrag(MouseEvent event) 
{

}
