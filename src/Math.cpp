#include "Math.h"
#include "cinder/Quaternion.h"

using namespace ci;

//=============================================================================
//----2D用----
//=============================================================================

// ベクトルの内積2D
float Vec2Dot( const ci::Vec2f* v1, const ci::Vec2f* v2 )
{
	return (v1->x * v2->x) + (v1->y * v2->y) ;
}
// ベクトル2Dの外積
float Vec2Cross( const ci::Vec2f* v1, const ci::Vec2f* v2 )
{
	return (v1->x * v2->y) - (v1->y * v2->x) ;
}
// ベクトル2Dの法線(左回り)
ci::Vec2f Vec2Normal( const ci::Vec2f* pV )
{
	ci::Vec2f n;
	n.x = -pV->y, n.y = pV->x;
	//n.x = pV->y, n.y = -pV->x;
	return n;
}
// ベクトルの正規化
ci::Vec2f Vec2Normalize(const ci::Vec2f* pV )
{
	ci::Vec2f v;

	double len;
    double x, y;

    x = (double)(pV->x);
    y = (double)(pV->y);
    len = sqrt(x * x + y * y);

	if(len < (1e-6)) return ci::Vec2f(0,0);

    len = 1.0 / len;
    x *= len;
    y *= len;

    v.x = (float)x;
    v.y = (float)y;

	return v;
}
// ベクトルの長さ
float Vec2Length( const ci::Vec2f* pV)
{
    double x, y;
    double len;

    if(!pV) return 0.0f;

    x = (double)(pV->x);
    y = (double)(pV->y);
    len = sqrt(x * x + y * y);

    return (float)(len);
}
// ベクトルの長さの二乗
float Vec2LengthSq( const ci::Vec2f* pV)
{
	return (pV->x)*(pV->x) + (pV->y)*(pV->y);
}


void Rotate2D( float x1, float y1, float alpha, float& x2, float& y2 )
{
	x2 = x1 * cos(alpha) - y1 * sin(alpha);
	y2 = x1 * sin(alpha) + y1 * cos(alpha);
}

void Rotate2DVec( const ci::Vec3f* p1, float rot, ci::Vec3f& p2 )
{
	p2.x = p1->x * cos(rot) - p1->y * sin(rot);
	p2.y = p1->x * sin(rot) + p1->y * cos(rot);
}

ci::Vec3f Rotate90( const ci::Vec3f* pV, const bool turn)
{
	ci::Vec3f v;
	if(!turn) v.x = (-pV->y), v.y = (pV->x), v.z = (pV->z);
	else v.x = (pV->y), v.y = (-pV->x), v.z = (pV->z);
	return v;
}

void Rotate2D90( float& x, float& y, const float _x, const float _y, bool turn )
{
	if(!turn) x = -_y, y = _x;
	else x = _y, y = -_x;
}

ci::Vec3f ToUnit2DVector(float rot)
{
	ci::Vec3f dir;
	dir.x = sin(rot);
	dir.y = cos(rot);
	dir.z = 0.0f;
	return dir;
}

void UnitVector(float& x, float& y, const float rot)
{
	x = sin(rot);
	y = cos(rot);
}

float GetDir2D( const ci::Vec3f* pV )
{
	float mag = hypotf(pV->x, pV->y);
	float dir = acos( pV->y / mag);
	if( pV->x > 0) dir = -dir;
	return dir;
}

void Rotate2DNomalize(const ci::Vec3f* p1, float rot, ci::Vec3f& p2)
{
	Rotate2DVec(p1, rot, p2);
	ci::Vec3f v = p2;
	//D3DXVec3Normalize(&p2, &v);
	p2 = v.normalized();
}

ci::Vec3f To2DVector( float mag, float rot)
{
	ci::Vec3f v;
	v.x = sin( rot ) * mag;
	v.y = cos( rot ) * mag;
	v.z = 0;

	return v;
}

ci::Vec3f ToScalarAndDir( const ci::Vec3f* pV)
{
	//float mag = D3DXVec3Length( pV );
	float mag = pV->length();
	float dir = acos( pV->y / mag);
	if( pV->x > 0) dir = -dir;

	ci::Vec3f ans = ci::Vec3f(mag,dir,0);
	
	return ans;
}

ci::Vec3f Rotate2DVec( const ci::Vec3f* pVel, float add )
{
	ci::Vec3f v = ToScalarAndDir(pVel);
	v.y += add;
	v = To2DVector( v.x, v.y);

	return v;
}

//=================================================================================================
//----3D用----
//=================================================================================================

ci::Vec3f Vec3(float x, float y, float z)
{
	ci::Vec3f v(x,y,z);
	return v;
}

// ベクトルの内積
float Vec3Dot( const ci::Vec3f* pV1, const ci::Vec3f* pV2 )
{
	return (pV1->x)*(pV2->x) + (pV1->y)*(pV2->y) + (pV1->z)*(pV2->z);
}

// ベクトルの外積
ci::Vec3f Vec3Cross( const ci::Vec3f* pV1, const ci::Vec3f* pV2 )
{
	ci::Vec3f v;

	v.x = (pV1->y * pV2->z) - (pV1->z * pV2->y);
	v.y = (pV1->z * pV2->x) - (pV1->x * pV2->z);
	v.z = (pV1->x * pV2->y) - (pV1->y * pV2->x);

	return v;
}

// ベクトルの長さ
float Vec3Length( const ci::Vec3f* pV)
{
    double x, y, z;
    double len;

    if(!pV) return 0.0f;

    x = (double)(pV->x);
    y = (double)(pV->y);
    z = (double)(pV->z);
    len = sqrt(x * x + y * y + z * z);

    return (float)(len);
}

ci::Vec3f Vec3Nomalize( const ci::Vec3f* pV)
{
	ci::Vec3f v;

	double len;
    double x, y, z;

    x = (double)(pV->x);
    y = (double)(pV->y);
    z = (double)(pV->z);
    len = sqrt(x * x + y * y + z * z);

	if(len < (1e-6)) return ci::Vec3f(0,0,0);

    len = 1.0 / len;
    x *= len;
    y *= len;
    z *= len;

    v.x = (float)x;
    v.y = (float)y;
    v.z = (float)z;

	return v;
}

float Vec3ScalarTriple( const ci::Vec3f* pV1, const ci::Vec3f* pV2, const ci::Vec3f* pV3 )
{
	return Vec3Dot(pV1, &Vec3Cross(pV2, pV3));
}

ci::Vec3f RotationVectorY90( const ci::Vec3f* pV, bool turn)
{
	ci::Vec3f v;
	if(!turn) v.x = pV->z, v.y = pV->y, v.z = -(pV->x);
	else v.x = -(pV->z), v.y = pV->y, v.z = pV->x;
	return v;
}

ci::Vec3f RotationVectorX( const ci::Vec3f* pV, float rot)
{
	//yz座標系
	ci::Vec3f v = (*pV);
	v.y = pV->y * cos(rot) - pV->z * sin(rot);
	v.z = pV->y * sin(rot) + pV->z * cos(rot);
	return v;
}

ci::Vec3f RotationVectorY( const ci::Vec3f* pV, float rot)
{
	//y軸に向かってzx座標
	ci::Vec3f v = (*pV);
	v.z = pV->z * cos(rot) - pV->x * sin(rot);
	v.x = pV->z * sin(rot) + pV->x * cos(rot);
	return v;
}

ci::Vec3f RotationVectorZ( const ci::Vec3f* pV, float rot)
{
	//xy座標
	ci::Vec3f v = (*pV);
	v.x = pV->x * cos(rot) - pV->y * sin(rot);
	v.y = pV->x * sin(rot) + pV->y * cos(rot);
	return v;
}


ci::Vec3f RotationVector( const ci::Vec3f* pV, const ci::Vec3f* pR )
{
	Matrix44f m = Matrix44f::createTranslation(*pV) * Matrix44f::createRotation(*pR);
	Vec3f v = m.transformPointAffine( Vec3f(0,0,0));
	return v;
}

ci::Vec3f RotationVector(const ci::Vec3f* pV, const ci::Matrix44f* pR)
{
	//Matrix44f m = Matrix44f::createTranslation(*pV) * *pR;
	//Vec3f v = m.transformPointAffine(Vec3f(0, 0, 0));
	//return v;
	return pR->transformPointAffine(*pV);
}

ci::Vec3f ToUnitVector( const ci::Vec3f* pR )
{
	//unit vector
	Vec3f v = { 0,0,0 };

	return v;
}

float ToRotationX( const ci::Vec3f* pV )
{
	return atan2f(pV->z, pV->y);
}

float ToRotationY( const ci::Vec3f* pV )
{
	return atan2f(pV->x, pV->z);
}

float ToRotationZ( const ci::Vec3f* pV )
{
	return  atan2f(pV->y, pV->x);
}


ci::Vec3f ToRotationXY( const ci::Vec3f* pV )
{
	//rotation yaw pitch roll
	ci::Vec3f rot(0,0,0);
	float r1, r2;

	//rotation y
	r1 = hypotf(pV->x, pV->z);
	rot.y = acos( pV->z / r1);
	if( pV->x < 0)	rot.y = -rot.y;

	//rotation x
	r2 = hypotf( pV->y, r1);
	rot.x = acos( r1 / r2 );
	if( pV->y > 0) rot.x = -rot.x;

	return rot;
}

ci::Vec3f _ToRotationXY( const ci::Vec3f* pV )
{
	//r ypr
	ci::Vec3f _rt(0,0,0);

	//rotation 
	_rt.y = atan2f( pV->x, pV->z);
	_rt.x = atan2f( -pV->y, hypotf( pV->x, pV->z));
	
	return _rt;
}


ci::Vec3f ToRotationZX( const ci::Vec3f* pV )
{
	//r ypr
	ci::Vec3f rot(0,0,0);

	//rotation 
	rot.x = atan2f( pV->z, pV->y);
	rot.z = -atan2f( pV->x, hypotf( pV->z, pV->y));
	
	return rot;
}

float Square( const float s)
{
	return s*s;
}

bool CollideSphere( float a, float b, ci::Vec3f posA, ci::Vec3f posB)
{
	ci::Vec3f ab(posA - posB);
	//float lengh_sq = D3DXVec3LengthSq( &ab ) ;
	float lengh_sq = ab.lengthSquared();


	// 球１の半径 + 球２の半径　が二つの球の距離よりも短ければ衝突
	if( Square( a + b ) > lengh_sq ) return true;
	//if( ( a + b ) > lenght_sq ) return true;
	else return false;
}


//線と球の当たり判定....rayのpos,vector sphereのpos, radius output_intersection_point near and far
bool RaySphereCollisionPos(	const ci::Vec3f* pRP, const ci::Vec3f* pRV, 
	const ci::Vec3f* pSP, float sr, ci::Vec3f* pOutN,	ci::Vec3f* pOutF
)
{
	
	ci::Vec3f _v1, _v2, _v3, _v4, _p1;

	//線の始点から球までのベクトル
	_v1 = *pSP - *pRP;


	_v3 = pRV->normalized();

	//内積の性質から垂線の長さを求める
	//_v2 = *pRV * D3DXVec3Dot( D3DXVec3Normalize( &_v3 ,pRV ), &_v1 ) / D3DXVec3Length( pRV ) - _v1;
	_v2 = (*pRV * _v1.dot(_v3) / pRV->length()) - _v1;

	//垂線と線の交点 _p1
	_p1 = * pSP + _v2;

	//垂線の平方
	//float _v2lSq = D3DXVec3LengthSq( &_v2 );
	float _v2lSq = _v2.lengthSquared();

	//半径の平方
	float _rSq = sr * sr;

	//球と線が交わらない条件
	if(_rSq < _v2lSq ) return false;

	//三平方の定理より
	float _bl = sqrt( _rSq - _v2lSq);

	// _p1 から交点ファーまでのベクトル
	_v4 = _v3 * _bl;

	//交点 ニア、ファー
	*pOutN = _p1 - _v4;
	*pOutF = _p1 + _v4;
	
	return true;
}

//ci::Vec3f QuaternionTransformCoord( const ci::Vec3f& v, const ci::Quaternion& qtn )
//{
//	ci::Vec3f ret ;
//	ci::Quaternion wkQtn, wkAns, wkQtnC ;
//	wkQtn.x = v.x ;
//	wkQtn.y = v.y ;
//	wkQtn.z = v.z ;
//	wkQtn.w = 0.0f ;
//	wkAns = wkQtn * qtn ;
//	D3DXQuaternionConjugate( &wkQtnC, &qtn ) ;	
//	wkAns = wkQtnC * wkAns ;
//	ret = ci::Vec3f(wkAns.x, wkAns.y, wkAns.z) ;
//	return ret ;
//}

ci::Vec3f Vec3TransformCoord( const ci::Vec3f* pV, const ci::Matrix44f* pM )
{
	ci::Vec3f v;
	
	v.x = (pM->m11* pV->x) + (pM->m21* pV->y) + (pM->m31* pV->z);
	v.y = (pM->m12* pV->x) + (pM->m22* pV->y) + (pM->m32* pV->z);
	v.z = (pM->m13* pV->x) + (pM->m23* pV->y) + (pM->m33* pV->z);

	return v;
}

ci::Vec3f Vec3TransformCoord( const ci::Matrix44f* pM )
{
	ci::Vec3f v;
	v.x = pM->m11 + pM->m21 + pM->m13;
	v.y = pM->m12 + pM->m22 + pM->m23;
	v.z = pM->m13 + pM->m32 + pM->m33;

	return v;
}

void Vec3ToAxisAngle( ci::Vec3f* pAx, float& angle, const ci::Vec3f* pV1, const ci::Vec3f* pV2 )
{
	*pAx = Vec3Cross( pV1, pV2);
	angle = acos( Vec3Dot( pV1, pV2 ) );
}

Matrix44f ToRotation(const ci::Vec3f& v)
{
	Quatf q = Quatf(Vec3f::zAxis(), v);
	return q.toMatrix44();
	//Matrix44f m = Matrix44f::createRotation
}

Vec3f ToVector(const ci::Matrix44f& m)
{	
	 auto v = m.transformVec(Vec3f(0,0,1));
	return v;
}