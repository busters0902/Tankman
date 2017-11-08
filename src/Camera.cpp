#include "Camera.h"

using namespace ci;

//=======================================================================================
//生成・破棄
//=======================================================================================

void Camera_::Setup()
{

	Vec3f eye = Vec3f(0, 0, -0.7);
	Vec3f look = Vec3f(0.0f, 0.0f, 1000.0f);

	camera->setEyePoint(eye);
	camera->setCenterOfInterestPoint(look);

	//eyePos,lookVec
	camEyePos = eye;
	camLookVec = eye + look;
	
}

void Camera_::Update()
{
	
	camera->setEyePoint(camEyePos);

	Vec3f lookPos = camEyePos + camLookVec;
	camera->setCenterOfInterestPoint(lookPos);

}

/////////////////////////////////////////////////////////////////////////////////////////

void CameraTPS::Setup()
{
	tgtFromlookVec = Vec3f(0, 2.0f, 0);

	tgtLen = 7.0f;
	tgtRotY = 0.0f;
	tgtRotX = toRadians(30.0f);
	tgtRotSpd = toRadians(5.0f);

	//
	Vec3f pos = { 0,0,0 };
	if(tgt != nullptr )pos = *tgt + tgtFromlookVec;
	Vec3f vec = Vec3f(0, 0, tgtLen);

	vec.rotateX(tgtRotX);
	vec.rotateY(-tgtRotY);

	//床にめり込まない
	Vec3f eye = pos - vec;
	if (eye.y < 1.0f) eye.y = 1.0f;

	camera->setEyePoint(eye);
	camera->setCenterOfInterestPoint(pos);
}

void CameraTPS::Update()
{

	Vec3f pos = { 0,0,0 };
	if (tgt != nullptr) pos = *tgt + tgtFromlookVec;
	Vec3f vec = Vec3f(0, 0, -tgtLen);

	vec.rotateX(tgtRotX);

	Vec3f vec2 = Vec3f(vec.x, 0.0f, vec.z);
	vec2.rotateY(-tgtRotY);
	vec2.y = vec.y;

	const float min = 0.4f;
	Vec3f eye = pos + vec2;
	//if (eye.y < min) eye.y = min;

	camera->setEyePoint(eye);
	camera->setCenterOfInterestPoint(pos);
}

//Jsonから
void CameraTPS::Setup(const JsonTree& json)
{
	//JsonTree Pos = json["Pos"];
	float x = json["LookVec"].getValueAtIndex<float>(0);
	float y = json["LookVec"].getValueAtIndex<float>(1);
	float z = json["LookVec"].getValueAtIndex<float>(2);
	tgtFromlookVec = Vec3f(x, y, z);

	tgtLen = json.getValueForKey<float>("Length");
	x = json.getValueForKey<float>("RotX");
	y = json.getValueForKey<float>("RotY");
	tgtRotX = toRadians(x);
	tgtRotY = toRadians(y);
	z = json.getValueForKey<float>("RotSpd");
	tgtRotSpd = toRadians(z);

	//
	Vec3f pos = { 0,0,0 };
	if (tgt != nullptr)pos = *tgt + tgtFromlookVec;
	Vec3f vec = Vec3f(0, 0, tgtLen);

	vec.rotateX(tgtRotX);
	vec.rotateY(-tgtRotY);

	//床にめり込まない
	Vec3f eye = pos - vec;
	if (eye.y < 1.0f) eye.y = 1.0f;

	camera->setEyePoint(eye);
	camera->setCenterOfInterestPoint(pos);
}

//=======================================================================================
//基本事項(初期化処理、終了処理、ロジック処理、描画処理)
//=======================================================================================

void Camera_::TranslateCamera(Vec3f vec)
{
	Vec3f v1, v2;
	v1 = vec + camEyePos;
	camEyePos = v1;
	v2 = vec + camLookVec;
	camLookVec = v2;
}