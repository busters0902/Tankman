#include "Collision.h"

using namespace ci;

//=================================================================================================================

// 線と三角形の交差判定
bool Collision::IntersectLineTriangle( const Line* pL, const Triangle* pT, float& u, float& v, float& w )
{
	Vec3f pq = pL->q - pL->p;
	Vec3f pa = pT->p[0] - pL->p;
	Vec3f pb = pT->p[1] - pL->p;
	Vec3f pc = pT->p[2] - pL->p;
	
	// pqがbc,ca,abの内側にあるかどうかを判定
	// スカラー三重積
	u = Vec3ScalarTriple( &pq, &pc, &pb );
	if( u < 0.0f ) return false;
	v = Vec3ScalarTriple( &pq, &pa, &pc );
	if( v < 0.0f ) return false;
	w = Vec3ScalarTriple( &pq, &pb, &pa );
	if( w < 0.0f ) return false;
	
	// 重心座標(u,v,w)
	
	float denom = 1.0f / (u + v + w);
	u *= denom;
	v *= denom;
	w *= denom;

	return true;
}
bool Collision::IntersectLineTriangle( const Line* pL, const Triangle* pT, Vec3f* pUVW )
{
	return IntersectLineTriangle( pL, pT, pUVW->x, pUVW->y, pUVW->z );
}


bool Collision::IntersectSegmentTriangle( const Segment* pS, const Triangle* pT, Vec3f* pC)
{
	Vec3f u, v, n;
	Vec3f dir, w0, w;
	float r, a, b;

	u = pT->p[1] - pT->p[0];
	v = pT->p[2] - pT->p[0];
	n = Collision::Vec3Cross( &u , &v );
	if( n== Vec3f(0,0,0)) return false;

	dir = pS->tp - pS->ip;
	w0 = pS->ip - pT->p[0];
	a = -Collision::Vec3Dot(&n, &w0);
	b = Collision::Vec3Dot(&n, &dir);
	if (fabs(b) < 0.000001f)
	{
		return false;
	}

	r = a / b;

	//光線と面の交差位置を出力
	*pC = pS->ip + r * dir;

	if(r < 0.0f) return false;

	float uu, uv, vv, wu, wv, D;
	uu = Collision::Vec3Dot(&u, &u);
	uv = Collision::Vec3Dot(&u, &v);
	vv = Collision::Vec3Dot(&v, &v);
	w = *pC - pT->p[0];
	wu = Collision::Vec3Dot(&w, &u);
	wv = Collision::Vec3Dot(&w, &v);
	D = uv * uv - uu * vv;

	float s, t;
	s = (uv*wv - vv*wu) / D;
	if(s < 0.0f || s > 1.0f) return false;
	t = (uv * wu - uu*wv) / D;
	if(t < 0.0f || (s+t) > 1.0f) return false;

	//これがなかったらRayTriangle
	//if( D3DXVec3LengthSq(&dir) < D3DXVec3LengthSq(&(*pC - pS->ip) ) ) return false;
	if ( dir.lengthSquared() < (*pC - pS->ip).lengthSquared() ) return false;

	return true;
}

bool Collision::IntersectSegmentTriangle( const Segment* pS, const Triangle* pT, Vec3f* pC, Vec3f* pN )
{
	Vec3f u, v, n;
	Vec3f dir, w0, w;
	float r, a, b;

	u = pT->p[1] - pT->p[0];
	v = pT->p[2] - pT->p[0];
	n = Collision::Vec3Cross( &u , &v );
	if( n== Vec3f(0,0,0)) return false;

	dir = pS->tp - pS->ip;
	w0 = pS->ip - pT->p[0];
	a = -Collision::Vec3Dot(&n, &w0);
	b = Collision::Vec3Dot(&n, &dir);
	if (fabs(b) < 0.000001f)
	{
		return false;
	}

	r = a / b;

	//光線と面の交差位置を出力
	*pC = pS->ip + r * dir;

	if(r < 0.0f) return false;

	float uu, uv, vv, wu, wv, D;
	uu = Collision::Vec3Dot(&u, &u);
	uv = Collision::Vec3Dot(&u, &v);
	vv = Collision::Vec3Dot(&v, &v);
	w = *pC - pT->p[0];
	wu = Collision::Vec3Dot(&w, &u);
	wv = Collision::Vec3Dot(&w, &v);
	D = uv * uv - uu * vv;

	float s, t;
	s = (uv*wv - vv*wu) / D;
	if(s < 0.0f || s > 1.0f) return false;
	t = (uv * wu - uu*wv) / D;
	if(t < 0.0f || (s+t) > 1.0f) return false;

	//RayがpSegmentになる
	//if( D3DXVec3LengthSq(&dir) < D3DXVec3LengthSq(&(*pC - pS->ip) ) ) return false;
	if ( dir.lengthSquared() < (*pC - pS->ip).lengthSquared() ) return false;


	//D3DXVec3Normalize( pN, &n );
	*pN = Vec3Nomalize( &n );

	return true;
}

//bool Collision::InstrsectSegmentQuadrangle( const Segment* pS, const Quadrangle* pQ, Vec3f* pC)
//{
//	return false;
//}

bool Collision::IntersectSegmentPlane( const Segment* pS, const Vec3f& d, const Vec3f& e, const Vec3f& f)
{
	Vec3f pn = Vec3Cross( &(e-d), &(f-d));
	float pd = Vec3Dot( &pn, &d);

	Vec3f s = pS->tp - pS->ip;
	float t = (pd - Vec3Dot( &pn, &pS->ip) ) / Vec3Dot(&pn, &s) ;
	if(t >= 0.0f && t <= 1.0f )
	{
		Vec3f q = pS->ip + t * s;
		return true;
	}

	return false;
}
	
bool Collision::IntersectSegmentQuadrangle( const Segment* pS, const Quadrangle* pQ, Vec3f* pC)
{

	Vec3f pa = pQ->p[0] - pS->ip;
	Vec3f pb = pQ->p[1] - pS->ip;
	Vec3f pc = pQ->p[2] - pS->ip;
	Vec3f pd = pQ->p[3] - pS->ip;
	Vec3f pq = pS->tp - pS->ip;
	
	//スカラー三重積の交換法則よりQP×P1使いまわす
	Vec3f crs;
	crs = Vec3Cross(&pc, &pq);
	
	float u = Vec3Dot(&crs, &pa);
	if(u >= 0)
	{
		//ACB
		float v = -Vec3Dot(&crs, &pb);
		if(v < 0) return false;
		float w = Vec3ScalarTriple(&pq, &pb, &pa);
		if(w < 0)
		{
			return false;
		}
		//衝突位置は体積比による重心位置になる
		//*pC = (u*pb + v*pa + w*pc) / (u+v+w)+pS->ip;
	}
	else //(u>0)
	{
		//ADC
		float v = Vec3Dot(&crs, &pd);
		if(v < 0) return false;
		float w = Vec3ScalarTriple(&pq, &pa, &pd);
		if(w < 0) return false;
		u = -u;
		//衝突位置は体積比による重心位置になる		
		//*pC = (u*pd + v*pa + w*pc) / (u+v+w)+pS->ip;
	}

	if( !IntersectSegmentPlane( pS, pQ->p[0], pQ->p[1], pQ->p[2] ))
		return false;
	
	return true;
}

// 線分と平面の判定
bool Collision::SegmentVSPlane( const Vec3f& _pos1, const Vec3f& _pos2, const Vec3f& _pos3 )
{
	// 平面から対象の座標に伸びるベクトルを出す
	Vec3f v1,v2 ;
	v1 = _pos3 - _pos1 ;
	v2 = _pos3 - _pos2 ;

	// 正規化
	//D3DXVec3Normalize( &v1, &v1 ) ;
	//D3DXVec3Normalize( &v2, &v2 ) ;
	v1.normalize();
	v2.normalize();

	// 平面の法線ベクトル（真上に伸びるベクトル）を出す
	Vec3f v3 ;
	Vec3f x(1,0,0), z(0,0,1) ;

	//D3DXVec3Cross( &v3, &x, &z ) ;
	v3 = x.cross(z);

	// 内積を出す
	//float dot1 = D3DXVec3Dot( &v1, &v3 ) ;
	//float dot2 = D3DXVec3Dot( &v2, &v3 ) ;
	float dot1 = v1.dot(v3);
	float dot2 = v2.dot(v3);

	// 線分が平面と衝突していたらtrueを、していないならfalseを返す
	if( ( dot1 * dot2 ) <= 0 )
	{
		return true ;
	}

	return false ;

}

bool Collision::RectVS( const Vec3f& _pos1, const Vec3f& _whl1, const Vec3f& _pos2, const Vec3f& _whl2 )
{
	// 範囲内にそれがいればtrue,いないならfalse
	if( _pos1.x >= (_pos2.x - _whl2.x - _whl1.x) && _pos1.x <= (_pos2.x + _whl2.x + _whl1.x) )
	{
		return true ;
	}else
		return false ;
}




bool Collision::CollideSphere( const Vec3f& _pos1, const int& r1, const Vec3f& _pos2, const int& r2 )
{
	//Vec3f tmp ;
	//if( D3DXVec3LengthSq( D3DXVec3Subtract( &tmp, &_pos1, &_pos2 ) ) < ( r1 + r2 ) * ( r1 + r2 ) )
	if ( (_pos2 - _pos1).lengthSquared() < (r1 + r2) * (r1 + r2)) return true ;
	return false ;
}

bool Collision::CollideSphere(const Collision::Sphere& s1, const Collision::Sphere& s2)
{
	return CollideSphere(s1.c, s1.r, s2.c, s2.r);
}


bool Collision::CollideAABB( const Vec3f& posA, const Vec3f& sizeA,
					   const Vec3f& posB, const Vec3f& sizeB )
{
	if( abs(posA.x - posB.x) > sizeA.x + sizeB.x ) return false ;
	if( abs(posA.y - posB.y) > sizeA.y + sizeB.y ) return false ;
	if( abs(posA.z - posB.z) > sizeA.z + sizeB.z ) return false ;

	return true ;
}

bool Collision::CollideAABB(const AABB& a, const AABB& b)
{
	return CollideAABB(a.pos, a.whl, b.pos, b.whl);
}

float Collision::SqDistPointAABB(const ci::Vec3f& p, const AABB& a)
{
	float sqDist = 0.0f;
	float p_[3] = { p.x, p.y, p.z };
	float  min[3] = { a.pos.x - a.whl.x, a.pos.y - a.whl.y, a.pos.z - a.whl.z };
	float  max[3] = { a.pos.x + a.whl.x, a.pos.y + a.whl.y, a.pos.z + a.whl.z };

	for (int i = 0; i < 3; i++)
	{
		float v = p[i];
		if (v < min[i]) sqDist += (min[i] - v) * (min[i]) - v;
		if (v > max[i]) sqDist += (v - max[i]) * (v - max[i]);
	}

	return sqDist;
}


bool Collision::CorrectPointAABB(ci::Vec3f& out, const ci::Vec3f& p, const AABB& a)
{
	if (abs(p.x - a.pos.x) > abs(a.whl.x)) return false;
	if (abs(p.y - a.pos.y) > abs(a.whl.y)) return false;
	if (abs(p.z - a.pos.z) > abs(a.whl.z)) return false;

	out.x = a.whl.x - (p.x - a.pos.x);
	out.y = a.whl.y - (p.y - a.pos.y);
	out.z = a.whl.z - (p.z - a.pos.z);

	return true;
}

int Collision::IntersectSegmentQuadSegment(const ci::Vec2f& p, const ci::Vec2f& q, 
	const ci::Vec2f qd[4], Vec2f& outP, Vec2f& outQ)
{
	Vec2f cross;
	if (IntersectSegments2D(p, q, qd[0], qd[1], cross))
	{
		outP = qd[0];
		outQ = qd[1];
		return 0;
	}

	if (IntersectSegments2D(p, q, qd[3], qd[0], cross))
	{
		outP = qd[3];
		outQ = qd[0];
		return 3;
	}

	if (IntersectSegments2D(p, q, qd[1], qd[2], cross))
	{
		outP = qd[1];
		outQ = qd[2];
		return 1;
	}

	if (IntersectSegments2D(p, q, qd[2], qd[3], cross))
	{
		outP = qd[2];
		outQ = qd[3];
		return 2;
	}


	return -1;
}

bool Collision::CollideOBB( const OBB* pO1, const OBB* pO2 )
{
    const float EPSILON = 1.175494e-37f;

    float R[3][3], AbsR[3][3];
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            //R[i][j] = D3DXVec3Dot(&pO1->nv[i], &pO2->nv[j]);
			R[i][j] = Vec3Dot(&pO1->nv[i], &pO2->nv[j]);
            AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
        }
    }
        
    Vec3f t = pO2->pos - pO1->pos;
    //t = Vec3f(D3DXVec3Dot(&t, &pO1->nv[0]), D3DXVec3Dot(&t, &pO1->nv[1]), D3DXVec3Dot(&t, &pO1->nv[2]));
	t = Vec3f(Vec3Dot(&t, &pO1->nv[0]), Vec3Dot(&t, &pO1->nv[1]), Vec3Dot(&t, &pO1->nv[2]));

    //軸L=A0, L=A1, L=A2判定
    float ra, rb;

    ra = pO1->whl.x;
    rb = pO2->whl.x * AbsR[0][0] + pO2->whl.y * AbsR[0][1] + pO2->whl.z * AbsR[0][2];
    if(fabsf(t[0]) > ra + rb)return false;

    ra = pO1->whl.y;
    rb = pO2->whl.x * AbsR[1][0] + pO2->whl.y * AbsR[1][1] + pO2->whl.z * AbsR[1][2];
    if(fabsf(t[1]) > ra + rb)return false;

    ra = pO1->whl.z;
    rb = pO2->whl.x * AbsR[2][0] + pO2->whl.y * AbsR[2][1] + pO2->whl.z * AbsR[2][2];
    if(fabsf(t[2]) > ra + rb)return false;


    //軸L=B0, L=B1, L=B2判定
    ra = pO1->whl.x * AbsR[0][0] + pO1->whl.y * AbsR[1][0] + pO1->whl.z * AbsR[2][0];
    rb = pO2->whl.x;
    if(fabsf(t[0] * R[0][0] + t[1] * R[1][0] + t[2] * R[2][0]) > ra + rb)return false;

    ra = pO1->whl.x * AbsR[0][1] + pO1->whl.y * AbsR[1][1] + pO1->whl.z * AbsR[2][1];
    rb = pO2->whl.y;
    if(fabsf(t[0] * R[0][1] + t[1] * R[1][1] + t[2] * R[2][1]) > ra + rb)return false;

    ra = pO1->whl.x * AbsR[0][2] + pO1->whl.y * AbsR[1][2] + pO1->whl.z * AbsR[2][2];
    rb = pO2->whl.z;
    if(fabsf(t[0] * R[0][2] + t[1] * R[1][2] + t[2] * R[2][2]) > ra + rb)return false;


    //軸L=A0 X B0判定
    ra = pO1->whl.y * AbsR[2][0] + pO1->whl.z * AbsR[1][0];
    rb = pO2->whl.y * AbsR[0][2] + pO2->whl.z * AbsR[0][1];
    if(fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)return false;

    //軸L=A0 X B1判定
    ra = pO1->whl.y * AbsR[2][1] + pO1->whl.z * AbsR[1][1];
    rb = pO2->whl.x * AbsR[0][2] + pO2->whl.z * AbsR[0][0];
    if(fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)return false;

    //軸L=A0 X B2判定
    ra = pO1->whl.y * AbsR[2][2] + pO1->whl.z * AbsR[1][2];
    rb = pO2->whl.x * AbsR[0][1] + pO2->whl.y * AbsR[0][0];
    if(fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)return false;

    //軸L=A1 X B0判定
    ra = pO1->whl.x * AbsR[2][0] + pO1->whl.z * AbsR[0][0];
    rb = pO2->whl.y * AbsR[1][2] + pO2->whl.z * AbsR[1][1];
    if(fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)return false;

    //軸L=A1 X B1判定
    ra = pO1->whl.x * AbsR[2][1] + pO1->whl.z * AbsR[0][1];
    rb = pO2->whl.x * AbsR[1][2] + pO2->whl.z * AbsR[1][0];
    if(fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)return false;

    //軸L=A1 X B2判定
    ra = pO1->whl.x * AbsR[2][2] + pO1->whl.z * AbsR[0][2];
    rb = pO2->whl.x * AbsR[1][1] + pO2->whl.y * AbsR[1][0];
    if(fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)return false;

    //軸L=A2 X B0判定
    ra = pO1->whl.x * AbsR[1][0] + pO1->whl.y * AbsR[0][0];
    rb = pO2->whl.y * AbsR[2][2] + pO2->whl.z * AbsR[2][1];
    if(fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)return false;

    //軸L=A2 X B1判定
    ra = pO1->whl.x * AbsR[1][1] + pO1->whl.y * AbsR[0][1];
    rb = pO2->whl.x * AbsR[2][2] + pO2->whl.z * AbsR[2][0];
    if(fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)return false;

    //軸L=A2 X B2判定
    ra = pO1->whl.x * AbsR[1][2] + pO1->whl.y * AbsR[0][2];
    rb = pO2->whl.x * AbsR[2][1] + pO2->whl.y * AbsR[2][0];
    if(fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)return false;

    return true;
}


int Collision::CorrectionCheck( Vec3f& a, Vec3f& prev, Vec3f& whlA, const Vec3f& b, const Vec3f& whlB )
{
	// min,max(左上、右下の仮座標を生成)
	Vec3f _do_min, _do_max ;
	Vec3f prev_min, prev_max ;
	Vec3f _this_min, _this_max ;

	_do_min = Vec3f( a.x - whlA.x, a.y + whlA.y, a.z - whlA.z ) ;
	_do_max = Vec3f( a.x + whlA.x, a.y - whlA.y, a.z - whlA.z ) ;
	prev_min = Vec3f( prev.x - whlA.x, prev.y + whlA.y, prev.z - whlA.z ) ;
	prev_max = Vec3f( prev.x + whlA.x, prev.y - whlA.y, prev.z - whlA.z ) ;
	_this_min = Vec3f( b.x - whlB.x, b.y + whlB.y, b.z - whlA.z ) ;
	_this_max = Vec3f( b.x + whlB.x, b.y - whlB.y, b.z - whlA.z ) ;


	//Xに補整するのか？Yに補整するのか？
	// 物体の左側に当たっていた場合
	if( (_do_max.x > _this_min.x && prev_max.x <= _this_min.x) && (prev_max.y < _this_min.y || prev_min.y > _this_max.y) ) 
		{
			a.x = CorrectionX( a, whlA, b, whlB ) ;
			return 0 ;
		}

	// 物体の右側に当たっていた場合
	if( (_do_min.x < _this_max.x && prev_min.x >= _this_max.x) && (prev_max.y < _this_min.y || prev_min.y > _this_max.y) )
		{
			a.x = CorrectionX( a, a, b, whlB ) ;
			return 0 ;
		}

	// 物体の上側に当たっていた場合
	if( _do_max.y < _this_min.y && prev_max.y >= _this_min.y )
		{
			a.y = CorrectionY( a, whlA, b, whlB ) ;
			return 1 ;
		}

	// 物体の下側に当たっていた場合
	if( _do_min.y > _this_max.y && prev_min.y <= _this_max.y )
		{
			a.y = CorrectionY( a, whlA, b, whlB ) ;
			return 0 ;
		}

	return 0 ;

}

//Vec3f Collision::Correct(Vec3f& pos, Vec3f& prev, Vec3f& whl, const Vec3f& thisPos, const Vec3f& this_whl) 
//{
//	Vec3f a;
//	return a;
//}
//
//bool Collision::CorrectAABB(Vec3f& out, const Vec3f& posA, const Vec3f& sizeA,
//	const Vec3f& posB, const Vec3f& sizeB)
//{
//	if (abs(posA.x - posB.x) > sizeA.x + sizeB.x) return false;
//	if (abs(posA.y - posB.y) > sizeA.y + sizeB.y) return false;
//	if (abs(posA.z - posB.z) > sizeA.z + sizeB.z) return false;
//
//	return true;
//}


float Collision::CorrectionX( const Vec3f& _do_pos, const Vec3f& _do_whl,  const Vec3f& b, const Vec3f& whlB )
{
	// _doの座標がthisに対して左右どちらにあるか

	// 物体の左に当たっている
	if( _do_pos.x < b.x ) return b.x - whlB.x - _do_whl.x ;
	// 物体の右に当たっている
	else if ( _do_pos.x > b.x ) return b.x + whlB.x + _do_whl.x ;

	return 0 ;

}




float Collision::CorrectionY( const Vec3f& _do_pos, const Vec3f& _do_whl,  const Vec3f& b, const Vec3f& whlB )
{
	// _doの座標がthisに対して上下どちらにあるか

	// 物体の上に当たっている
	if( _do_pos.y > b.y ) return b.y + whlB.y + _do_whl.y ;
	// 物体の下に当たっている
	else if ( _do_pos.y < b.y ) return b.y - whlB.y - _do_whl.y ;

	return 0 ;
}

//=================================================================================================================
//2D
//=================================================================================================================

// 線分の衝突
bool Collision::IntersectSegments2D( const Vec3f& a1, const Vec3f& a2, const Vec3f& b1, const Vec3f& b2, Vec3f& p )
{

	Vec2f seg1_s = { a1.x, a1.y };
	Vec2f seg1_e = { a2.x, a2.y };

	Vec2f seg2_s = { b1.x, b1.y };
	Vec2f seg2_e = { b2.x, b2.y };


	Vec2f seg1_v = seg1_e - seg1_s ;
	Vec2f seg2_v = seg2_e - seg2_s ;

    Vec2f v = seg2_s - seg1_s;

    float Crs_v1_v2 = Vec2Cross( seg1_v, seg2_v );
    if ( Crs_v1_v2 == 0.0f ) {
       // 平行状態
      return false;
    }

    float Crs_v_v1 = Vec2Cross( v, seg1_v );
    float Crs_v_v2 = Vec2Cross( v, seg2_v );

    float t1 = Crs_v_v2 / Crs_v1_v2;
    float t2 = Crs_v_v1 / Crs_v1_v2;

    const float eps = 0.00001f;
    if ( t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1 ) {
       // 交差していない
      return false;
    }

	Vec2f pp = seg1_s + seg1_v * t1 ;
	p.y = pp.y ;

	return true ;

}

float Collision::Signed2DTriArea( const Vec3f& a, const Vec3f& b, const Vec3f& c )
{
	return ( a.x - c.x ) * ( b.y - c.y ) - ( a.y - c.y ) * ( b.x - c.x ) ; 
}

bool Collision::SegmentSegment( const Vec3f& a, const Vec3f& b, const Vec3f& c, const Vec3f& d, Vec3f& p )
{
	// 面積の符号は点ｃおよびｄがabのどちら側にあるかに一致
	float a1 = Signed2DTriArea( a, b, d ) ; // abdの巻いている方向を計算（＋あるいはー）
	float a2 = Signed2DTriArea( a, b, c ) ; // 交差する場合、a1と反対の符号を持っている

	// cおよびｄがabに対して互いに異なる側にある場合、面積は異なる符号を持つ
	if( a1 * a2 < 0.0f )
	{

		// aおよびbに対して線分cdに関する符号を計算する
		float a3 = Signed2DTriArea( c, d, a ) ; // cdの巻いている方向を計算（＋あるいはー）

		// 面積は整数であるので、a1-a2 = a3-a4, あるいは a4 = a3+a2-a1

		// float a4 = Signed2DTriArea( c, d, b ) ; // a3とは異なる符合をもっているはず

		float a4 = a3 + a2 - a1 ;

		// 面積が互いに異なる符合を持つ場合、点aおよびbはcdに対して反対側にある
		if( a3 * a4 < 0.0f )
		{
			// 線分は交差している。交差点を見つける
			// ここでb(三角形cdaおよびcdbの底辺、すなわちcdの長さ）は相殺される。
			float t = a3 / ( a3 / a4 ) ;

			p = a + t * ( b - a ) ;

			return true ;
		}
	}

	// 交差しない（あるいは同一直線上）
	return false ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Collision::Rect2D Collision::ToRect2DAABB(const AABB& a)
{
	Collision::Rect2D rect;
	//XZ平面
	rect.c.x = a.pos.x;
	rect.c.y = a.pos.z;
	rect.u[0] = Vec2f::xAxis();
	rect.u[1] = Vec2f::yAxis();
	rect.wh.x = a.whl.x;
	rect.wh.y = a.whl.z;
	return rect;
}

void Collision::ToQuad2D(const ci::Vec3f p3d[4], ci::Vec2f p2d[4])
{
	for (int i = 0; i < 4; i++)
	{
		p2d[i].x = p3d[i].x;
		p2d[i].y = p3d[i].z;
	}
}

bool Collision::IntersectSegments2D(const Vec2f& a1, const Vec2f& a2, const Vec2f& b1, const Vec2f& b2, Vec2f& p)
{
	const Vec2f seg1_v = a2 - a1;
	const Vec2f seg2_v = b2 - b1;

	const Vec2f v = b1 - a1;

	float Crs_v1_v2 = Vec2Cross(seg1_v, seg2_v);
	if (Crs_v1_v2 == 0.0f)
	{
		// 平行状態
		return false;
	}

	float Crs_v_v1 = Vec2Cross(v, seg1_v);
	float Crs_v_v2 = Vec2Cross(v, seg2_v);

	float t1 = Crs_v_v2 / Crs_v1_v2;
	float t2 = Crs_v_v1 / Crs_v1_v2;

	const float eps = 0.00001f;
	if (t1 + eps < 0 || t1 - eps > 1 || t2 + eps < 0 || t2 - eps > 1) {
		// 交差していない
		return false;
	}

	Vec2f pp = a1 + (seg1_v * t1);
	//p.y = pp.y ;
	p = pp;

	return true;
}

bool Collision::PointQuad(const ci::Vec2f& p, const Quad2D& q)
{
	Vec2f BA, CB, DC, AD, BP, CP, DP, AP;
	BA = q.p[0] - q.p[1];
	CB = q.p[1] - q.p[2];
	DC = q.p[2] - q.p[3];
	AD = q.p[3] - q.p[0];

	BP = p - q.p[1];
	CP = p - q.p[2];
	DP = p - q.p[3];
	AP = p - q.p[0];

	float dot1 = Vec2Dot(BA, BP);
	float dot2 = Vec2Dot(CB, CP);
	float dot3 = Vec2Dot(DC, DP);
	float dot4 = Vec2Dot(AD, AP);

	//四角形の頂点が左回り
	if (dot1 >= 0 && dot2 >= 0 && dot3 >= 0 && dot4 >= 0) return true;
	//四角形の頂点が右回り
	//if(dot1 <= 0 && dot2 <= 0 && dot3 <= 0 && dot4 <= 0) return true;

	return false;
}

//ラインにもっとも近い点
Vec2f Collision::Vec2DotLineProximate(const Vec2f& d, const Vec2f& p, const Vec2f& q)
{
	Vec2f PQ = q - p;
	float t = Vec2Dot((d - p), PQ) / Vec2Dot(PQ, PQ);
	Vec2f v = (p + t * PQ);
	return v;
}


bool Collision::OneSideLineQuad(const ci::Vec2f& p, const ci::Vec2f& q, const Quad2D& qd, ci::Vec2f& crct, bool turn)
{
	Vec2f v = q - p;

	//矩形の頂点の位置(p)ABCD
	Vec2f v1, v2, v3, v4;
	v1 = qd.p[0] - p;
	v2 = qd.p[1] - p;
	v3 = qd.p[2] - p;
	v4 = qd.p[3] - p;

	//線と矩形の格頂点 外積(cross)を調べる
	//float ca, cb, cc, cd;

	float c[4] = { Vec2Cross(v, v1), Vec2Cross(v, v2), Vec2Cross(v, v3), Vec2Cross(v, v4) };

	if (c[0] > 0 && c[1] > 0 && c[2] > 0 && c[3] > 0)	return false;
	if (c[0] < 0 && c[1] < 0 && c[2] < 0 && c[3] < 0)	return false;

	//一番距離の遠いやつ
	float fl, tmpl;
	Vec2f fv, tmpv;
	fl = 0.0f;
	fv = Vec2f::zero();
	if (!turn)
	{
		for (int i = 0; i< 4; i++)
		{
			if (c[i] >= 0.0f) continue;
			tmpv = Vec2DotLineProximate(qd.p[i], p, q);
			tmpl = Vec2LengthSq(tmpv);
			if (fl < tmpl)
			{
				fl = tmpl;
				fv = tmpv;
			}
		}
	}
	else
	{
		for (int i = 0; i< 4; i++)
		{
			if (c[i] <= 0.0f) continue;
			tmpv = Vec2DotLineProximate(qd.p[i], p, q);
			tmpl = Vec2LengthSq(tmpv);
			if (fl < tmpl)
			{
				fl = tmpl;
				fv = tmpv;
			}
		}
	}

	//
	crct = fv;


	return true;
}

void Collision::SetQuadFromRect(Quad2D& q, const Rect2D& r)
{
	Vec2f p, w, h;
	p = r.c;
	w = r.u[0] * r.wh.x;
	h = r.u[1] * r.wh.y;

	q.p[0] = p + -w + h;
	q.p[1] = p + w + h;
	q.p[2] = p + w + -h;
	q.p[3] = p + -w + -h;
}

bool Collision::IntersectSegmentRectangle2D(const ci::Vec2f& p, const ci::Vec2f& q, const Rect2D& r, ci::Vec2f& v, bool turn)
{
	//矩形の頂点を作る
	Quad2D qd;
	SetQuadFromRect(qd, r);

	//簡易判定 線と矩形の交差判定
	//Vec2f v_;
	if (!OneSideLineQuad(p, q, qd, v, turn)) return false;

	////矩形ないの点
	if (PointQuad(p, qd)) return true;
	if (PointQuad(q, qd)) return true;

	//線分同士の交差
	Vec2f cross;
	if (IntersectSegments2D(p, q, qd.p[0], qd.p[1], cross))
	{
		return true;
	}
		
	if (IntersectSegments2D(p, q, qd.p[1], qd.p[2], cross))
	{
		return true;
	}

	if (IntersectSegments2D(p, q, qd.p[2], qd.p[3], cross))
	{
		return true;
	}
	if (IntersectSegments2D(p, q, qd.p[3], qd.p[0], cross)) 
	{
		return true;
	}

	return false;
}



//=================================================================================================================????


// ベクトルの内積2D
float Collision::Vec2Dot( const Vec2f& v1, const Vec2f& v2 )
{
	return (v1.x * v2.x) + (v1.y * v2.y) ;
}

// ベクトル2Dの外積
float Collision::Vec2Cross( const Vec2f& v1, const Vec2f& v2 )
{
	return v1.x * v2.y - v1.y * v2.x ;
}

float Collision::Vec2LengthSq(const ci::Vec2f& v)
{
	return (v.x * v.x) + (v.y * v.y);
}

float Collision::Vec2Length(const ci::Vec2f& v)
{
	return hypotf(v.x, v.y);
}

// 法線
Vec2f Collision::Vec2Normal( const Vec2f* pV )
{
	Vec2f n;
	n.x = -pV->y;
	n.y =  pV->x;
	return n;
}

// 法線逆
Vec2f Collision::Vec2NormalR( const Vec2f* pV )
{
	Vec2f n;
	n.x =  pV->y;
	n.y = -pV->x;
	return n;
}

//=====================================================================================================================

// ベクトルの内積
float Collision::Vec3Dot( const Vec3f* pV1, const Vec3f* pV2 )
{
	return (pV1->x)*(pV2->x) + (pV1->y)*(pV2->y) + (pV1->z)*(pV2->z);
}

// ベクトルの外積
Vec3f Collision::Vec3Cross( const Vec3f* pV1, const Vec3f* pV2 )
{
	Vec3f v;

	v.x = (pV1->y * pV2->z) - (pV1->z * pV2->y);
	v.y = (pV1->z * pV2->x) - (pV1->x * pV2->z);
	v.z = (pV1->x * pV2->y) - (pV1->y * pV2->x);

	return v;
}

// ベクトルの長さ
float Collision::Vec3Length( const Vec3f* pV)
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

Vec3f Collision::Vec3Nomalize( const Vec3f* pV)
{
	Vec3f v;

	double len;
    double x, y, z;

    x = (double)(pV->x);
    y = (double)(pV->y);
    z = (double)(pV->z);
    len = sqrt(x * x + y * y + z * z);

	if(len < (1e-6)) return Vec3f(0,0,0);

    len = 1.0 / len;
    x *= len;
    y *= len;
    z *= len;

    v.x = (float)x;
    v.y = (float)y;
    v.z = (float)z;

	return v;
}

float Collision::Vec3ScalarTriple( const Vec3f* pV1, const Vec3f* pV2, const Vec3f* pV3 )
{
	return Vec3Dot(pV1, &Vec3Cross(pV2, pV3));
}

Vec3f Collision::Vec3RelativeCoord(const Vec3f* p0,const Vec3f* pA, const Vec3f* pB, const Vec3f* pC,
								const float u, const float v, const float w )
{
	Vec3f a, b, c;
	a = *pA - *p0;
	b = *pB - *p0;
	c = *pC - *p0;

	return	(u*a+v*b+w*c / (u+v+w) + (*p0));
}

struct AABB_
{
	Vec3f c;
	float r[3];
};

void UpdateAABB(const AABB_& a, float m[3][3], float t[3], AABB_& b)
{
	for (int i = 0; i < 3; i++)
	{
		b.c[i] = t[i];
		b.r[i] = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			b.c[i] += m[i][j] * a.c[j];
			b.r[i] += abs(m[i][j]) * a.r[j];
		}
	}

}

void Collision::ConvertMatrix(float out[3][3], const ci::Matrix44f& m)
{
	out[0][0] = m.m00;
	out[0][1] = m.m01;
	out[0][2] = m.m02;
	out[1][0] = m.m10;
	out[1][1] = m.m11;
	out[1][2] = m.m12;
	out[2][0] = m.m20;
	out[2][1] = m.m21;
	out[2][2] = m.m22;
}

void Collision::UpdateAABB(Collision::AABB& b, const Collision::AABB& a, const ci::Vec3f& t, const ci::Matrix44f& m)
{
	float m_[3][3];
	ConvertMatrix(m_, m);

	for (int i = 0; i < 3; i++)
	{
		b.pos[i] = t[i];
		b.whl[i] = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			b.pos[i] += m_[i][j] * a.pos[j];
			b.whl[i] += abs(m_[i][j]) * a.whl[j];
		}
	}

}

/*float m_[3][3];
	ConvertMatrix(m_, m);

	for (int i = 0; i < 3; i++)
	{
		b.pos[i] = t[i];
		b.whl[i] = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			b.pos[i] += m_[i][j] * pos[j];
			b.whl[i] += abs(m_[i][j]) * size[j];
		}
}*/


void Collision::UpdateAABB(Collision::AABB& b, const ci::Vec3f& pos, 
		const ci::Vec3f& size, const ci::Vec3f& t, const ci::Matrix44f& m)
{

	//i = 0
	b.pos.x = t.x;
	b.whl.x = 0.0f;

	//j = 0
	b.pos.x += m.m00 * pos.x;
	b.whl.x += abs(m.m00) * size.x;

	//j = 1
	b.pos.x += m.m01 * pos.y;
	b.whl.x += abs(m.m01) * size.y;

	//j = 2
	b.pos.x += m.m02 * pos.z;
	b.whl.x += abs(m.m02) * size.z;

	//i = 1
	b.pos.y = t.y;
	b.whl.y = 0.0f;

	//j = 0
	b.pos.y += m.m10 * pos.x;
	b.whl.y += abs(m.m10) * size.x;

	//j = 1
	b.pos.y += m.m11 * pos.y;
	b.whl.y += abs(m.m11) * size.y;

	//j = 2
	b.pos.y += m.m12 * pos.z;
	b.whl.y += abs(m.m12) * size.z;

	//i = 2
	b.pos.z = t.z;
	b.whl.z = 0.0f;

	//j = 0
	b.pos.z += m.m20 * pos.x;
	b.whl.z += abs(m.m20) * size.x;

	//j = 1
	b.pos.z += m.m21 * pos.y;
	b.whl.z += abs(m.m21) * size.y;

	//j = 2
	b.pos.z += m.m22 * pos.z;
	b.whl.z += abs(m.m22) * size.z;

	//座標計算がうまくいかないので
	b.pos = pos + t;

}