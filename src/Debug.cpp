#include "cinder/app/AppNative.h"
#include "Debug.h"
#include "KeyManager.h"

using namespace ci;

bool Debug::drawsDebugLog;
bool Debug::usesDebugMode;

CameraOrtho* Debug::cameraOrtho;
Font* Debug::font;
Vec3f* Debug::pV = nullptr;
CameraTPS* Debug::pCam = nullptr;


bool Debug::drawsColliders;
bool Debug::drawsStringHit;
int Debug::coll;

Vec2f Debug::closest;

//=======================================================================================
//初期化処理、終了処理
//=======================================================================================
void Debug::Initialize()
{
	drawsDebugLog = false;
	usesDebugMode = false;

	// 平行投影カメラの初期化
	cameraOrtho = new CameraOrtho(0, WINDOW_WIDTH,        // 画面左の座標、画面右の座標
		WINDOW_HEIGHT, 0,       // 画面下端の座標、画面上端の座標
		1, 10);                 // 画面手前の座標、画面奥の座標
								// translation
	cameraOrtho->setEyePoint(Vec3f(0.0f, 0.0f, 0.0f));
	cameraOrtho->setCenterOfInterestPoint(Vec3f(0.0f, 0.0f, -1000.0f));

	// フォントを作成
	//   フォント名: システムフォントから選ぶ
	//   フォントサイズ
	//font = Font("Hiragino Maru Gothic ProN W4", 30.0f);
	font = new Font("Hiragino Mincho ProN", 30.0f);
}

void Debug::Finalize()
{
	delete font;
	delete cameraOrtho;
}

//=======================================================================================
//基本事項(ロジック処理、描画処理)
//=======================================================================================
void Debug::Update()
{

	//デバッグモード
	if (KeyManager::GetKeyDown('9'))
	{
		//スイッチ
		if (usesDebugMode)
		{
			usesDebugMode = false;
		}
		else
		{
			usesDebugMode = true;
		}
	}

	//デバッグ描画
	if (KeyManager::GetKeyDown('0'))
	{
		//スイッチ
		if (drawsDebugLog)
		{
			drawsDebugLog = false;
		}
		else
		{
			drawsDebugLog = true;
		}
	}

	//衝突判定描画
	if (KeyManager::GetKeyDown('8'))
	{
		//スイッチ
		if (drawsColliders)
		{
			drawsColliders = false;
		}
		else
		{
			drawsColliders = true;
		}
	}

	//マイフレーム初期化
	{
		drawsStringHit = false;
		coll = 0;
	}

}

void Debug::Draw()
{

	if (drawsStringHit) DrawString(100, 100, 10.0f, "HIT");
}

//=======================================================================================
//文字列の描画
//=======================================================================================
void Debug::DrawString(float x, float y, float size, const std::string format, ...)
{
	Vec2f p = Vec2f(x, y);

	//文字列の作成-----------------------------------------
	char cstr[256] = u8"";
	va_list args;
	va_start(args, format);
	vsprintf(cstr, format.c_str(), args);
	va_end(args);
	std::string str = cstr;
	//-----------------------------------------------------
	// 平行投影カメラで描画
	gl::setMatrices(*cameraOrtho);

	gl::scale(size, size, 1.0f);
	gl::translate(0.0f, 0.0f, -5.0f);

	gl::drawString(str, p, Color(1.0f, 1.0f, 1.0f), *font);
	
}

void Debug::ChangeDebugDraw()
{
	if (KeyManager::GetKeyDown('9'))
	{
		if (drawsDebugLog) drawsDebugLog = false;
		else drawsDebugLog = true;
	}
}