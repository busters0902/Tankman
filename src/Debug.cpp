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
//�����������A�I������
//=======================================================================================
void Debug::Initialize()
{
	drawsDebugLog = false;
	usesDebugMode = false;

	// ���s���e�J�����̏�����
	cameraOrtho = new CameraOrtho(0, WINDOW_WIDTH,        // ��ʍ��̍��W�A��ʉE�̍��W
		WINDOW_HEIGHT, 0,       // ��ʉ��[�̍��W�A��ʏ�[�̍��W
		1, 10);                 // ��ʎ�O�̍��W�A��ʉ��̍��W
								// translation
	cameraOrtho->setEyePoint(Vec3f(0.0f, 0.0f, 0.0f));
	cameraOrtho->setCenterOfInterestPoint(Vec3f(0.0f, 0.0f, -1000.0f));

	// �t�H���g���쐬
	//   �t�H���g��: �V�X�e���t�H���g����I��
	//   �t�H���g�T�C�Y
	//font = Font("Hiragino Maru Gothic ProN W4", 30.0f);
	font = new Font("Hiragino Mincho ProN", 30.0f);
}

void Debug::Finalize()
{
	delete font;
	delete cameraOrtho;
}

//=======================================================================================
//��{����(���W�b�N�����A�`�揈��)
//=======================================================================================
void Debug::Update()
{

	//�f�o�b�O���[�h
	if (KeyManager::GetKeyDown('9'))
	{
		//�X�C�b�`
		if (usesDebugMode)
		{
			usesDebugMode = false;
		}
		else
		{
			usesDebugMode = true;
		}
	}

	//�f�o�b�O�`��
	if (KeyManager::GetKeyDown('0'))
	{
		//�X�C�b�`
		if (drawsDebugLog)
		{
			drawsDebugLog = false;
		}
		else
		{
			drawsDebugLog = true;
		}
	}

	//�Փ˔���`��
	if (KeyManager::GetKeyDown('8'))
	{
		//�X�C�b�`
		if (drawsColliders)
		{
			drawsColliders = false;
		}
		else
		{
			drawsColliders = true;
		}
	}

	//�}�C�t���[��������
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
//������̕`��
//=======================================================================================
void Debug::DrawString(float x, float y, float size, const std::string format, ...)
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