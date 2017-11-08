#pragma once
//プレイシーン
//・シングルトン

#include <boost/signals2.hpp>
#include "SceneBase.h"
#include "../Camera.h"
#include "cinder/Ray.h"

#include "../Tank.h"
#include "../StageObject.h"
#include "../Enemy.h"
#include "../UIObject.h"

class PlayScene;
typedef std::shared_ptr<PlayScene> PlaySceneSP;
typedef std::weak_ptr<PlayScene> PlaySceneWP;

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene();

public: 
	bool shiftsTitleScene;

	CameraTPS camera;
	ci::Vec3f cameraTgt;

	ci::CameraOrtho* cameraOrtho;
	ci::CameraPersp* uiCamera;
	//StageObjectSP uiCursor;
	UIObjectSP uiCursor;
	void CreateUI();
	void DestroyUI();
	void UpdateUI();
	void DrawUI();

	//Player
	TankSP tank;

	//RayCursor
	ci::Ray mouseRay;
	ci::Vec3f rayIntersection;
	//void SetRay();	//カメラの位置からマウスポインタ
	
	//仮想平面
	ci::Vec3f planeOrigin;
	ci::Vec3f planeNormal;
	void ResetPlane();	//カメラとか変わったら

	//基準となる地面
	std::vector<StageObjectSP> grounds;
	void CreateGrounds(const ci::JsonTree& json);
	void DestroyGrounds();
	void UpdateGrounds();
	void DrawGrounds();

	//ステージブロック
	std::list<StageObjectSP> objects;
	void CreateObjects();
	void DestroyObjects();
	void UpdateObjects();
	void DrawObjects();

	//敵
	std::list<EnemySP> enemies;
	std::list<EnemySP> enemyRemoveList;
	void CreateEnemies(const ci::JsonTree& json);
	void DestroyEnemies();
	void UpdateEnemies();
	void DrawEnemies();

	std::list<EnemySP> waitingEnemies;
	void Event();


	//弾
	std::list<BulletSP> bullets;
	std::list<BulletSP> bulletRemoveList;
	void CreateBullets();
	void DestroyBullets();
	void UpdateBullets();
	void DrawBullets();
	//途中追加	Add
	//途中削除	Remove

	//shot
	int reload;

	//イベント？
	boost::signals2::signal<void(ci::Vec3f&, ci::Vec3f&, float)> bulletCreate; 
	void CreateBullet(ci::Vec3f& pos, ci::Vec3f& dir, float vel);

	//衝突処理
	void Collide();  
	void Hit();     

	//UI
	ci::CameraPersp* cameraUI;
	//フォント
	ci::Font* font;

	int cnt;
	int end_cnt;

	//スコア
	int score;
	void AddScore(int add = 1) { score += add; }
 
	//制限時間
	int limit;
	bool end;
	bool CheckLimit() { if (limit <= 0) end = true; }
	void DrawString(float x, float y, float size, const std::string format, ...);

public:
	void Update();
	void Draw();
	void Shift();

	void MouseDown(ci::app::MouseEvent event) ;
	void MouseUp(ci::app::MouseEvent event) ;
	void MouseMove(ci::app::MouseEvent event) ;
	void MouseDrag(ci::app::MouseEvent event) ;




};