#pragma once
//Collision : コリジョン
//--ネームスペース空間--
//・衝突関係をまとめておく
//

#include "cinder/gl/gl.h"




namespace Collision
{
	//構造体=============================================================================
	//3D
	//===================================================================================

	//有向境界線ボックス... Oriented Bounding Box
	struct OBB
	{
		ci::Vec3f pos;		//位置
		ci::Vec3f nv[3];		//方位ベクトル？
		ci::Vec3f whl;		//面までの距離(幅の半文)
	};

	//軸並行境界ボックス.. Axis-Aligned Bounding Box
	struct AABB
	{
		ci::Vec3f pos;	//位置
		ci::Vec3f whl;	//各軸方向へ長さ
	};

	//光線...始点あっても終点はない
	struct Ray
	{
		ci::Vec3f pos;	//始点
		ci::Vec3f dir;	//方向
	};

	//直線
	struct Line
	{
		ci::Vec3f p;	//point P
		ci::Vec3f q;	//point Q
	};

	//線分
	struct Segment
	{
		ci::Vec3f ip;	//始点...initial point
		ci::Vec3f tp; //終点...terminal point
	};

	//球
	struct Sphere
	{
		ci::Vec3f c;	//center postion
		float r;		//radius
	};

	//三角形
	struct Triangle
	{
		ci::Vec3f p[3];	//ポイント
	};
	
	//四角形
	struct Quadrangle
	{
		ci::Vec3f p[4];	//ポイント
	};

	struct Plane
	{
		ci::Vec3f n;
		float d;
	};


	//長方形
	struct Rectangle
	{
		ci::Vec3f a;
		ci::Vec3f b;
	};

	//======================================================================================
	//2D
	//======================================================================================

	struct Rect2D
	{
		ci::Vec2f c;
		ci::Vec2f u[2];
		ci::Vec2f wh;
	};

	//左回りに
	//※0が左上が望ましい
	struct Quad2D
	{
		ci::Vec2f p[4];
	};

	struct Segment2D
	{
		ci::Vec2f ip;	//始点...initial point
		ci::Vec2f tp; //終点...terminal point
		Segment2D() {}
		Segment2D(const ci::Vec2f& a, const ci::Vec2f& b) { ip = a, tp = b; }
	};

	//=============================================================================================
	//関数
	//=============================================================================================
	//3D

	// 直線と三角形の交差判定
	// u, v, w... 重心座標を返す
	bool IntersectLineTriangle( const Line* pL, const Triangle* pT, float& u, float& v, float& w );
	bool IntersectLineTriangle( const Line* pL, const Triangle* pT, ci::Vec3f* pUVW );

	// 線分と三角形の交差判定
	// pCに交差位置を返す
	bool IntersectSegmentTriangle(const Segment* pS, const Triangle* pT, ci::Vec3f* pC );

	// 線分と三角形の交差判定+法線(補正方向)
	//交差位置(ワールド座標)
	//法線その三角形の法線
	bool IntersectSegmentTriangle(const Segment* pS, const Triangle* pT, ci::Vec3f* pC, ci::Vec3f* pN );
	
	//線分と四角形の交差判定
	//線分の始点からの相対位置を返す
	bool IntersectSegmentQuadrangle(const Collision::Segment* pS, const Collision::Quadrangle* pQ, ci::Vec3f* pC);


	//線分と平面の当たり判定
	//abcは平面上の三点
	bool IntersectSegmentPlane( const Segment* pS, const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c);

	// 線分と平面の当たり判定
	// arg1... 対象の座標(線分の始点)
	// arg2... 対象の座標(線分の終点)
	// arg3... 平面の座標
	bool SegmentVSPlane( const ci::Vec3f& _pos1, const ci::Vec3f& _pos2, const ci::Vec3f& _pos3 );

	// 判定計算の軽量用
	// 衝突判定を行うの範囲(X軸方向の距離で判断) ※引数はAABBの成分
	// arg1... 対象1の座標
	// arg2... 対象1の大きさの半分
	// arg3... 対象2の座標
	// arg4... 対象2の大きさの半分
	bool RectVS( const ci::Vec3f& _pos1, const ci::Vec3f& _whl1, const ci::Vec3f& _pos2, const ci::Vec3f& _whl2 ) ;

	// 円と円
	// arg1... 対象1の座標
	// arg2... 対象1の半径
	// arg3... 対象2の座標
	// arg4... 対象2の半径
	bool CollideSphere( const ci::Vec3f& _pos1, const int& r1, const ci::Vec3f& _pos2, const int& r2 ) ;
	bool CollideSphere( const Collision::Sphere& s1, const Collision::Sphere& s2);

	//回転(m)しているAABBを囲ったAABBを作る
	void UpdateAABB(Collision::AABB& b, const Collision::AABB& a,const ci::Vec3f& t, const ci::Matrix44f& m);
	void UpdateAABB(Collision::AABB& b, const ci::Vec3f& pos, const ci::Vec3f& size, const ci::Vec3f& t, const ci::Matrix44f& m);
	void ConvertMatrix(float out[3][3], const ci::Matrix44f& m);

	// AABB vs AABB
	bool CollideAABB( const ci::Vec3f& posA, const ci::Vec3f& sizeA,  const ci::Vec3f& posB, const ci::Vec3f& sizeB ) ;
	bool CollideAABB(const AABB& a, const AABB& b);
	float SqDistPointAABB(const ci::Vec3f& p, const AABB& a );


	bool CorrectPointAABB(ci::Vec3f& out, const ci::Vec3f& p, const AABB& a);

	bool CorrectAABB(ci::Vec3f& out, const ci::Vec3f& posA, const ci::Vec3f& sizeA,
		const ci::Vec3f& posB, const ci::Vec3f& sizeB);

	// AABB vs AABB,どの方向に補整するべきか判定
	// ans... int型だが戻り値は受け取らないreturnしたいだけなんで
	int CorrectionCheck( ci::Vec3f& _do_pos, ci::Vec3f& _do_prev, ci::Vec3f& _do_whl, const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;
	ci::Vec3f Correct(ci::Vec3f& pos, ci::Vec3f& prev, ci::Vec3f& whl, const ci::Vec3f& thisPos, const ci::Vec3f& this_whl);

	// AABBvsAABB,X補整
	float CorrectionX( const ci::Vec3f& _do_pos, const ci::Vec3f& _do_whl,  const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;

	// AABBvsAABB,Y補整
	float CorrectionY( const ci::Vec3f& _do_pos, const ci::Vec3f& _do_whl,  const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;

	// OBBvsOBB
	bool CollideOBB( const OBB* pO1, const OBB* pO2) ;

	//=================================================================================================================
	//２Ｄ

	// 線分の衝突（２Ｄ）
	// arg1... a始点
	// arg2... a終点（ベクトル方向）
	// arg3... b始点
	// arg4... b終点（ベクトル方向）
	// arg5... 線分の交点
	bool IntersectSegments2D( const ci::Vec3f& a1, const ci::Vec3f& a2, const ci::Vec3f& b1, const ci::Vec3f& b2, ci::Vec3f& p ) ;
	bool IntersectSegments2D( const ci::Vec2f& a1, const ci::Vec2f& a2, const ci::Vec2f& b1, const ci::Vec2f& b2, ci::Vec2f& p);

	// 線分と矩形の衝突判定
	// arg3... 交点
	//bool IntersectSegmentRectangle( const Segment* pS, const Rect* pR, const ci::Vec3f* pC );

	// 三角形の符号付面積の2倍を返す、結果はabcが反時計回りの場合に正
	// abcが時計回りの場合に負、abcが縮退している場合にゼロ
	float Signed2DTriArea( const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c );

	// びcd線分abおよが重なっているかどうかを判定、重なっていれば
	// abに沿った交差するtの値および交差位置pを計算して返す
	bool SegmentSegment( const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c, const ci::Vec3f& d, ci::Vec3f& p );

	////////////////////////////////////////////////////////////////////////////////////////

	//AABB(3D)からの変換
	Rect2D ToRect2DAABB(const AABB& a);
	Quad2D ToQuad2D(const Quadrangle& p);
	void ToQuad2D( const ci::Vec3f p3d[4], ci::Vec2f p2d[4]);

	bool PointQuad(const ci::Vec2f& p, const Quad2D& q);
	ci::Vec2f Vec2DotLineProximate(const ci::Vec2f& d, const ci::Vec2f& p, const ci::Vec2f& q);
	bool OneSideLineQuad(const ci::Vec2f& p, const ci::Vec2f& q, const Quad2D& qd, ci::Vec2f& crct, bool turn);
	void SetQuadFromRect(Quad2D& q, const Rect2D& r);
	bool IntersectSegmentRectangle2D(const ci::Vec2f& p, const ci::Vec2f& q, const Rect2D& r, ci::Vec2f& v, bool turn);

	int IntersectSegmentQuadSegment(const ci::Vec2f& p, const ci::Vec2f& q,
		const ci::Vec2f qd[4], ci::Vec2f& outP, ci::Vec2f& outQ);


	//=================================================================================================================
	//2D汎用計算

	//2Dベクトルの内積
	float Vec2Dot( const ci::Vec2f& v1, const ci::Vec2f& v2 );

	// 2Dベクトルの外積
	float Vec2Cross( const ci::Vec2f& v1, const ci::Vec2f& v2 ) ;

	float Vec2LengthSq( const ci::Vec2f& v);
	float Vec2Length(const ci::Vec2f& v);

	// 2Dのベクトルの法線算出
	ci::Vec2f Vec2Normal(const ci::Vec2f* pV );
	ci::Vec2f Vec2NormalR(const ci::Vec2f* pV );

	//=================================================================================================================
	//3D汎用計算

	// ベクトルの内積
	float Vec3Dot( const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

	// ベクトルの外積
	ci::Vec3f Vec3Cross( const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

	// ベクトルの長さ
	float Vec3Length( const ci::Vec3f* pV);
	
	// ベクトルの正規化
	ci::Vec3f Vec3Nomalize( const ci::Vec3f* pV );

	// スカラー三重積
	float Vec3ScalarTriple( const ci::Vec3f* pV1, const ci::Vec3f* pV2, const ci::Vec3f* pV3 );

	// 重心座標を相対座標にする
	ci::Vec3f Vec3RelativeCoord(const ci::Vec3f* p0,const ci::Vec3f* pA,const ci::Vec3f* pB,const ci::Vec3f* pC,
									const float u, const float v, const float w );
}


typedef std::shared_ptr<Collision::AABB> AABBSP;
typedef std::weak_ptr<Collision::AABB> AABBWP;

typedef std::shared_ptr<Collision::Sphere> SphereSP;
typedef std::weak_ptr<Collision::Sphere> SphereWP;

typedef std::shared_ptr<Collision::Segment> SegmentSP;
typedef std::weak_ptr<Collision::Segment> SegmentWP;

typedef std::shared_ptr<Collision::Rect2D> Rect2DSP;
typedef std::weak_ptr<Collision::Rect2D> Rect2DWP;