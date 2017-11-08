#pragma once
//
#include "SceneBase.h"

#include "PlayScene.h"
#include "TitleScene.h"

class Scene
{
	
public:
	template<typename type>
	static void Create(type* _scene)
	{
		//scene = std::make_shared<type>(*_scene);	//※コピーコンストラクタの悪夢その1
		scene = std::shared_ptr<type>(_scene);
	}

	static SceneBase& get() 
	{
		return *scene;
	}

	static SceneBaseSP Get()
	{
		return scene;
	}

private:
	static SceneBaseSP scene;

};