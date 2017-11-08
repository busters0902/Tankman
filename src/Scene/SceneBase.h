#pragma once
//�V�[���̐e�N���X
//�E�V���O���g��
//�E�ŏ��ɒe��ǂݍ���Œu���łƂ���Clone������đł��o��

#include "cinder/gl/gl.h"
#include "cinder/app/AppNative.h"
#include <memory>



class SceneBase;
typedef std::shared_ptr<SceneBase> SceneBaseSP;
typedef std::weak_ptr<SceneBase> SceneBaseWP;

class SceneBase abstract
{
public:
	virtual ~SceneBase() {}
 
public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Shift() = 0;

	virtual void MouseDown(ci::app::MouseEvent event) = 0;
	virtual void MouseUp(ci::app::MouseEvent event) = 0;
	virtual void MouseMove(ci::app::MouseEvent event) = 0;
	virtual void MouseDrag(ci::app::MouseEvent event) = 0;
};	
