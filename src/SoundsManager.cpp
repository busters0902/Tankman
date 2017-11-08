#include "cinder/app/AppNative.h"
#include "SoundsManager.h"

using namespace ci;
using namespace ci::app;
using namespace ci::audio;

Context* SoundsManager::ctx;
GainNodeRef SoundsManager::gain;

BufferRef SoundsManager::buffers[SE_NUM];
BufferPlayerNodeRef SoundsManager::player[SE_NODE_NUM];
FilePlayerNodeRef SoundsManager::bgmPlayer[BGM_NUM];

int SoundsManager::playerCnt;

//=======================================================================================
//初期化処理、終了処理
//=======================================================================================
void SoundsManager::Initialize()
{
	// サウンド機能を有効にする
	ctx = audio::master();
	ctx->enable();
	
	//音声SE------------------------------------------------------------------------
	std::string path[] = 
	{
		"sounds/start.mp3",
		"sounds/shot.mp3",
		"sounds/hit.mp3",
		"sounds/exp.mp3",
		"sounds/gameclear.mp3",
	};

	for (int i = 0; i < SE_NUM; ++i) 
	{
		// サウンドデータ読み込み準備
		audio::SourceFileRef source = audio::load(loadAsset(path[i]));
		// 読み込んでBufferに格納
		buffers[i] = source->loadBuffer();
	}


	// 再生用のノードを生成
	for(int i = 0; i < SE_NODE_NUM; i++)
		player[i] = ctx->makeNode(new audio::BufferPlayerNode());

	// 音量調整用のノードを生成
	gain = ctx->makeNode(new audio::GainNode());

	// 用意したノードを連結
	for (int i = 0; i < SE_NODE_NUM; i++)
		player[i] >> gain >> ctx->getOutput();


	//音声BGM-----------------------------------------------------------------
	std::string bgmPath[] =
	{
		"sounds/title.mp3",
		"sounds/play.mp3",
	};

	//再生用ノードの生成
	bgmPlayer[0] = ctx->makeNode(new audio::FilePlayerNode());
	bgmPlayer[1] = ctx->makeNode(new audio::FilePlayerNode());

	//サウンドデータ読み込み
	
	audio::SourceFileRef source0 = audio::load(loadAsset(bgmPath[0]));
	bgmPlayer[0]->setSourceFile(source0);
	audio::SourceFileRef source1 = audio::load(loadAsset(bgmPath[1]));
	bgmPlayer[1]->setSourceFile(source1);

	//ノードの連結
	bgmPlayer[0] >> gain >> ctx->getOutput();
	bgmPlayer[1] >> gain >> ctx->getOutput();

	//bgmPlayer[0]->start();

	//-------------------------------------------------------------------------
	playerCnt = 0;

	//if(true)
	//{
	//	float value = gain->getValue();
	//	value = std::min(value - 0.9f, 1.0f);

	//	gain->setValue(value);
	//}
}

//ここ重要
void SoundsManager::Finalize()
{
	gain.reset();
	bgmPlayer[0].reset();
	bgmPlayer[1].reset();

}

//=======================================================================================
//基本事項(ロジック処理、描画処理)
//=======================================================================================
void SoundsManager::Update()
{

}

void SoundsManager::Draw()
{

}

//=======================================================================================
//再生
//=======================================================================================
void SoundsManager::Play(int id)
{
	player[playerCnt]->setBuffer(buffers[id]);
	player[playerCnt]->start();
	
	++playerCnt;
	if (playerCnt == (int)SE_NODE_NUM)
	{
		playerCnt = 0;
	}
}

void SoundsManager::PlayBGM(int id)
{
	for (int i = 0; i < BGM_NUM; i++)
	{
		if (bgmPlayer[i]->isEnabled()) bgmPlayer[i]->stop();
	}
	bgmPlayer[id]->start();

	/*if (bgmPlayer[id]->isEnabled())
	{
		bgmPlayer[id]->stop();
	}
	else bgmPlayer[id]->start();*/
}

void SoundsManager::RePlayBGM(int id)
{
	bgmPlayer[id]->start();
}
