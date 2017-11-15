#pragma once
//タイトルシーン

#include "SceneBase.h"
#include "../Camera.h"

#include "../TitleObject.h"
#include "../UIObject.h"
#include "cinder/Json.h"

class TitleScene;
typedef std::shared_ptr<TitleScene> TitleSceneSP;
typedef std::weak_ptr<TitleScene> TitleSceneWP;

class TitleScene : public SceneBase
{
public:
	TitleScene();
	~TitleScene();
	TitleScene(const TitleScene &obj);	
	
public:
	void CreateObjects(const ci::JsonTree& json);
	void DestroyObjects();
	void UpdateObjects();
	void DrawObjects();

	void CreateUI(const ci::JsonTree& json);
	void DestroyUI();
	void UpdateUI();
	void DrawUI();

public:
	CameraTPS camera;
	ci::Vec3f tgt;

	std::list<TitleObjectSP> objects;
public://UI ロゴ
	ci::CameraOrtho* cameraOrtho;
	ci::CameraPersp* cameraUI;

	UIObjectSP objectUI;
	std::vector<UIObjectSP> uiObjects;

private:
	bool shiftsPlayScene;
	int tapCount;

public:
	void Update();
	void Draw();
	void Shift();

	void MouseDown(ci::app::MouseEvent event);
	void MouseUp(ci::app::MouseEvent event);
	void MouseMove(ci::app::MouseEvent event);
	void MouseDrag(ci::app::MouseEvent event);

};