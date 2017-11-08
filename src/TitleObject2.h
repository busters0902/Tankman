#pragma once
//�^�C�g���V�[���Ŏg��3D�I�u�W�F�N�g
//�p�[�c�𕡐�����

#include "TitleObject.h"
#include "Parts.h"

class TitleObject2;
typedef std::shared_ptr<TitleObject2> TitleObject2SP;
typedef std::weak_ptr<TitleObject2> TitleObject2WP;

class TitleObject2 : public TitleObject
{

public:
	TitleObject2();
	~TitleObject2();

	static TitleObject2* Create();
	static void Destroy(TitleObject2* obj);

	static TitleObject2* Create( std::string const& modelName,
		std::string const& texName, Transform const& trans);

	//����g��
	static TitleObject2SP CreateSP(	std::string const& modelName,
									std::string const& texName, 
									Transform const& trans);

	//static TitleObject2SP CreateSP(jsonTree json);
public:
	void SetCamera(ci::CameraPersp* camera_);

public:
	std::vector<PartsSP> models;
	void InitParts();

public : //Collision

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};

//models�̒��g
//0. body
//1. gun turret
//2. gun
//3. left tire 1
//4. righttire 1
//5. left tire 2
//6. righttire 2
//7. left tire 3
//8. righttire 3
//9. left tire 4
//10.right tire 4