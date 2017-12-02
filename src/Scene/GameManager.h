#pragma once
//�Q�[���S�̂��Ǘ�����N���X
//�E�V���O���g��

#include "cinder/gl/gl.h"
#include "cinder/app/AppNative.h"
#include <memory>


class GameManager;
typedef std::shared_ptr<GameManager> GameManagerSP;
typedef std::weak_ptr<GameManager> GameManagerWP;

class GameManager
{
private:
	static GameManagerSP instance;
	GameManager();
	~GameManager();
public:
	static GameManagerSP &GetInstance()
	{
		if (instance.get() == NULL)
			instance.reset(new GameManager(), [](GameManager *p) {delete p; });
		return instance;
	}

public:
	//iPhone6S	��4.7�C���`	1, 334 x 750	��326ppi	�P�U�F�X
	//Galaxy S6 SC-05G	��5.1�C���`	2560�~1440	��575ppi	16:9
	enum WindowSize
	{
		//Height= 2560 / 4,
		//Width = 1440 / 4,
		Height	= 1440 / 2,
		Width	= 2560 / 2,
	};

private:
	void CreateScene();

public:
	void Setup();
	void Shutdown();
	void Update();
	void Draw();

	void Shift();

	void MouseDown(ci::app::MouseEvent event);
	void MouseUp(ci::app::MouseEvent event);
	void MouseMove(ci::app::MouseEvent event);
	void MouseDrag(ci::app::MouseEvent event);
};
