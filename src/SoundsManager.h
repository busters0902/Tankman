#pragma once
//�������Ǘ�����N���X
//�E���m�X�e�[�g

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/audio/Context.h"
#include "cinder/audio/GainNode.h"
#include "cinder/audio/SamplePlayerNode.h"

#include <memory>

class SoundsManager
{
public :
	enum BGM
	{
		BGM_TITLE,
		BGM_PLAY,
		BGM_NUM,
	};
	enum SE
	{
		SE_START,
		SE_SHOT,
		SE_HIT,
		SE_EXPLOSION,
		SE_GAMECLEAR,

		SE_NUM,
		SE_NODE_NUM = 5,
	};

private:
	SoundsManager() {}
	~SoundsManager() {}

public:
	static void Initialize();
	static void Finalize();
public:
	static ci::audio::Context* ctx;
	static ci::audio::GainNodeRef gain;

	//�m�[�h�̃o�b�t�@�[
	static ci::audio::BufferRef buffers[SE_NUM];

	//SE(�K�萔�Đ��ł���)
	static ci::audio::BufferPlayerNodeRef player[SE_NODE_NUM];
	static int playerCnt;

	//BGM�p�̃m�[�h
	static ci::audio::FilePlayerNodeRef bgmPlayer[BGM_NUM];
	
public:
	static void Play(int id);
	static void PlayBGM( int id );
	static void RePlayBGM(int id);
	//stop
public:
	static void Update();
	static void Draw();

};

