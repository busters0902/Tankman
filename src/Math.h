//計算するときに使える関数群
//
//3D上に合うようにする 2016/5/??
//

#pragma once

#include "cinder/gl/gl.h"

//2D用========================================================================

// 内積、外積、法線  (2D)
float Vec2Dot( const ci::Vec2f* v1, const ci::Vec2f* v2 );
float Vec2Cross( const ci::Vec2f* v1, const ci::Vec2f* v2 );
ci::Vec2f Vec2Normal(const ci::Vec2f* pV );
ci::Vec2f Vec2Normalize(const ci::Vec2f* pV );
float Vec2Length( const ci::Vec2f* pV);
float Vec2LengthSq( const ci::Vec2f* pV);

//vector3.zは無視



//座標のローテンション
void Rotate2D(float x1, float y1, float alpha, float& x2, float& y2);
void Rotate2DVec(const ci::Vec3f* p1, float rot, ci::Vec3f& p2);

//90度回転 [true : 90][false : -90]
ci::Vec3f Rotate90( const ci::Vec3f* pV, const bool turn = false);
void Rotate2D90( float& x, float& y, const float _x, const float _y, bool turn = false );

//向き(ラジアん)から単位ベクトルの生成
ci::Vec3f ToUnit2DVector(float rot);
void UnitVector(float& x, float& y, const float rot);

//2Dベクトルの方向を返す
float GetDir2D( const ci::Vec3f* pV );

//単位ローテーションベクトル
void Rotate2DNomalize(const ci::Vec3f* p1, float rot, ci::Vec3f& p2);

//スカラーと向きからベクトル
//引数1 : スカラー
//引数2	: 向き	(ラジアン)
//戻り値: べくとる 
ci::Vec3f To2DVector( float mag, float rot);

//べクトルからスカラーと向き
//引数1 : ベクトル
//戻り値: xにスカラー、yにラジアン
ci::Vec3f ToScalarAndDir(const ci::Vec3f* pV);

//ベクトルを回転
//引数1 :元のベクトル
//引数1 :加算で右回転(ラジアン)
//戻り値:ベクトル
ci::Vec3f Rotate2DVec(const ci::Vec3f* pVel, float add );

//3D用================================================================================

//ベクトルの初期化
ci::Vec3f Vec3(float x, float y, float z);

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

//90度Yaw回転したベクトル
ci::Vec3f RotateVecY90(const ci::Vec3f* pV, bool turn= false);

//ベクトルのX軸回転
ci::Vec3f RotationVectorX( const ci::Vec3f* pV, float rot);

//ベクトルのY軸回転
ci::Vec3f RotationVectorY( const ci::Vec3f* pV, float rot);

//ベクトルのZ軸回転
ci::Vec3f RotationVectorZ( const ci::Vec3f* pV, float rot);

//ベクトルを回転させる
//arg1... 回転させるベクトルのポインタ
//arg2... 回転するローテーション
//ans.... 回転されたベクトル
ci::Vec3f RotationVector( const ci::Vec3f* pV, const ci::Vec3f* pR );
ci::Vec3f RotationVector(const ci::Vec3f* pV, const ci::Matrix44f* pR);

//ローテーションをベクトルにする
//arg... ローテーション
//ans... 単位ベクトル
ci::Vec3f ToUnitVector( const ci::Vec3f* pR );

//ベクトルをそれぞれの軸のローテーションで表す
float ToRotationX( const ci::Vec3f* pV );
float ToRotationY( const ci::Vec3f* pV );
float ToRotationZ( const ci::Vec3f* pV );

//ベクトルをYaw(Y軸),Pitch(X軸)のローテーションで表す 
//※モデル等はひっくり返らない..Y軸から計算するため
//arg... ベクトル
//ans... ローテーション
ci::Vec3f ToRotationXY( const ci::Vec3f* pV );

//Y軸方向に対しての回転量を出す
//※Xを先に決める
ci::Vec3f ToRotationZX( const ci::Vec3f* pV );

//ただのfloatの二乗式
float Square(const float s);

//球と球の衝突判定
//arg1... 物体Aの半径
//arg2... 物体Bの半径
//arg3... 物体Aの座標
//arg4... 物体Bの座標
//ans.... true 衝突
bool CollideSphere(float a, float b, ci::Vec3f posA, ci::Vec3f posB);

//線と球の当たり判定....rayのpos,vector sphereのpos, radius output_intersection_point near and far
//arg ... 線の座標、線のベクトル、球の中心座標、球の半径、線と球の交点:線に近い方、線に遠い方
bool RaySphereCollisionPos(	
	const ci::Vec3f* pRP,	const ci::Vec3f* pRV,
	const ci::Vec3f* pSP,	float sr,
	ci::Vec3f* pOutN, ci::Vec3f* pOutF );


// name... QuaternionTransformCoord
// work.... クォータニオンを元にしたベクトルを計算
// arg1.... 変換前のベクトル
// arg2.... クォータニオン
// ret..... 演算結果
//ci::Vec3f QuaternionTransformCoord(const ci::Vec3f& v, const ci::Quaternion& qtn);

//二つのVectorA, VectorB → 回転軸(axis)、角度を求める(angle)
//axis .... 外積
//angle.... 内積
ci::Vec3f Vec3TransformCoord( const ci::Vec3f* pV, const ci::Matrix44f* pM );

//未検
ci::Vec3f Vec3TransformCoord( const ci::Matrix44f* pM );


void Vec3ToAxisAngle( ci::Vec3f* pAx, float& angle, const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

ci::Matrix44f ToRotation( const ci::Vec3f& v );

ci::Vec3f ToVector(const ci::Matrix44f& m);