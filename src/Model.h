#pragma once
//3Dモデルクラス

#include "cinder/gl/gl.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Material.h"
#include "cinder/ObjLoader.h"        // OBJ形式を読み込む
#include "cinder/TriMesh.h"          // 三角ポリゴンの集合体
#include <memory>

#include "Transform.h"


class Model;
typedef std::shared_ptr<Model> ModelSP;
typedef std::weak_ptr<Model> ModelWP;

typedef std::shared_ptr<ci::gl::Light> LightSP;
typedef std::shared_ptr<ci::gl::Material> MaterialSP;
typedef std::shared_ptr<ci::TriMesh> MeshSP;
typedef ci::gl::TextureRef TetureSP;

class Model
{

public:
	Model();
	~Model();
	//Model(&obj);

	static std::map<std::string, std::shared_ptr<ci::TriMesh>> meshes;
	static std::map<std::string, ci::gl::TextureRef> textures;

	enum Mode
	{
		NORMAL,
		MODEL_ONLY,
		LIGHT_ONLY,
		MATERIAL_ONLY,
		LIGHT_AND_MATERIAL,
		SHERE_MODEL,
		TEXTURE,
		MODE_NUM,
	};
	int mode;
	void SetMode(Mode m) { mode = m; }

	static Model* Create();
	static Model* Create(	const std::string& meshPath,
							const std::string& texturePath);
	static Model* Create(Mode m);
	static void Destroy(Model* obj);
	
	static ModelSP CreateSP(const std::string& meshPath,
							const std::string& texturePath);
	static ModelSP CreateSP(Mode m);
	//static ModelSP Create(const std::string& texturePath);

	static TetureSP LoadTexture(std::string);
	static MeshSP LoadMesh(std::string);

	Model(const std::string& meshPath,
		const std::string& texturePath);

	//コピーコンストラクタの代わり
	void Init(const std::string& meshPath,
		const std::string& texturePath);
	void InitMeshAndTexture(const std::string& meshPath,
		const std::string& texturePath);
	//
	void Init(Mode m);

	void InitTexture(const std::string& texturePath);

	void CreateTexture(const std::string& texturePath);
	void CreateMeshAndTexture(const std::string& meshPath,
		const std::string& texturePath);
	void CreateLight();
	void CreateMaterial();
public:
	LightSP light;
	MaterialSP material;

	MeshSP mesh;
	TetureSP texture;

public:
	void Setup();
	void ShutDown();
	void Update();
	void Draw();

};


