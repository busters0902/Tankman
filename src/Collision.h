#pragma once
//Collision : �R���W����
//--�l�[���X�y�[�X���--
//�E�Փˊ֌W���܂Ƃ߂Ă���
//

#include "cinder/gl/gl.h"




namespace Collision
{
	//�\����=============================================================================
	//3D
	//===================================================================================

	//�L�����E���{�b�N�X... Oriented Bounding Box
	struct OBB
	{
		ci::Vec3f pos;		//�ʒu
		ci::Vec3f nv[3];		//���ʃx�N�g���H
		ci::Vec3f whl;		//�ʂ܂ł̋���(���̔���)
	};

	//�����s���E�{�b�N�X.. Axis-Aligned Bounding Box
	struct AABB
	{
		ci::Vec3f pos;	//�ʒu
		ci::Vec3f whl;	//�e�������֒���
	};

	//����...�n�_�����Ă��I�_�͂Ȃ�
	struct Ray
	{
		ci::Vec3f pos;	//�n�_
		ci::Vec3f dir;	//����
	};

	//����
	struct Line
	{
		ci::Vec3f p;	//point P
		ci::Vec3f q;	//point Q
	};

	//����
	struct Segment
	{
		ci::Vec3f ip;	//�n�_...initial point
		ci::Vec3f tp; //�I�_...terminal point
	};

	//��
	struct Sphere
	{
		ci::Vec3f c;	//center postion
		float r;		//radius
	};

	//�O�p�`
	struct Triangle
	{
		ci::Vec3f p[3];	//�|�C���g
	};
	
	//�l�p�`
	struct Quadrangle
	{
		ci::Vec3f p[4];	//�|�C���g
	};

	struct Plane
	{
		ci::Vec3f n;
		float d;
	};


	//�����`
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

	//������
	//��0�����オ�]�܂���
	struct Quad2D
	{
		ci::Vec2f p[4];
	};

	struct Segment2D
	{
		ci::Vec2f ip;	//�n�_...initial point
		ci::Vec2f tp; //�I�_...terminal point
		Segment2D() {}
		Segment2D(const ci::Vec2f& a, const ci::Vec2f& b) { ip = a, tp = b; }
	};

	//=============================================================================================
	//�֐�
	//=============================================================================================
	//3D

	// �����ƎO�p�`�̌�������
	// u, v, w... �d�S���W��Ԃ�
	bool IntersectLineTriangle( const Line* pL, const Triangle* pT, float& u, float& v, float& w );
	bool IntersectLineTriangle( const Line* pL, const Triangle* pT, ci::Vec3f* pUVW );

	// �����ƎO�p�`�̌�������
	// pC�Ɍ����ʒu��Ԃ�
	bool IntersectSegmentTriangle(const Segment* pS, const Triangle* pT, ci::Vec3f* pC );

	// �����ƎO�p�`�̌�������+�@��(�␳����)
	//�����ʒu(���[���h���W)
	//�@�����̎O�p�`�̖@��
	bool IntersectSegmentTriangle(const Segment* pS, const Triangle* pT, ci::Vec3f* pC, ci::Vec3f* pN );
	
	//�����Ǝl�p�`�̌�������
	//�����̎n�_����̑��Έʒu��Ԃ�
	bool IntersectSegmentQuadrangle(const Collision::Segment* pS, const Collision::Quadrangle* pQ, ci::Vec3f* pC);


	//�����ƕ��ʂ̓����蔻��
	//abc�͕��ʏ�̎O�_
	bool IntersectSegmentPlane( const Segment* pS, const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c);

	// �����ƕ��ʂ̓����蔻��
	// arg1... �Ώۂ̍��W(�����̎n�_)
	// arg2... �Ώۂ̍��W(�����̏I�_)
	// arg3... ���ʂ̍��W
	bool SegmentVSPlane( const ci::Vec3f& _pos1, const ci::Vec3f& _pos2, const ci::Vec3f& _pos3 );

	// ����v�Z�̌y�ʗp
	// �Փ˔�����s���͈̔�(X�������̋����Ŕ��f) ��������AABB�̐���
	// arg1... �Ώ�1�̍��W
	// arg2... �Ώ�1�̑傫���̔���
	// arg3... �Ώ�2�̍��W
	// arg4... �Ώ�2�̑傫���̔���
	bool RectVS( const ci::Vec3f& _pos1, const ci::Vec3f& _whl1, const ci::Vec3f& _pos2, const ci::Vec3f& _whl2 ) ;

	// �~�Ɖ~
	// arg1... �Ώ�1�̍��W
	// arg2... �Ώ�1�̔��a
	// arg3... �Ώ�2�̍��W
	// arg4... �Ώ�2�̔��a
	bool CollideSphere( const ci::Vec3f& _pos1, const int& r1, const ci::Vec3f& _pos2, const int& r2 ) ;
	bool CollideSphere( const Collision::Sphere& s1, const Collision::Sphere& s2);

	//��](m)���Ă���AABB���͂���AABB�����
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

	// AABB vs AABB,�ǂ̕����ɕ␮����ׂ�������
	// ans... int�^�����߂�l�͎󂯎��Ȃ�return�����������Ȃ��
	int CorrectionCheck( ci::Vec3f& _do_pos, ci::Vec3f& _do_prev, ci::Vec3f& _do_whl, const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;
	ci::Vec3f Correct(ci::Vec3f& pos, ci::Vec3f& prev, ci::Vec3f& whl, const ci::Vec3f& thisPos, const ci::Vec3f& this_whl);

	// AABBvsAABB,X�␮
	float CorrectionX( const ci::Vec3f& _do_pos, const ci::Vec3f& _do_whl,  const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;

	// AABBvsAABB,Y�␮
	float CorrectionY( const ci::Vec3f& _do_pos, const ci::Vec3f& _do_whl,  const ci::Vec3f& _this_pos, const ci::Vec3f& _this_whl ) ;

	// OBBvsOBB
	bool CollideOBB( const OBB* pO1, const OBB* pO2) ;

	//=================================================================================================================
	//�Q�c

	// �����̏Փˁi�Q�c�j
	// arg1... a�n�_
	// arg2... a�I�_�i�x�N�g�������j
	// arg3... b�n�_
	// arg4... b�I�_�i�x�N�g�������j
	// arg5... �����̌�_
	bool IntersectSegments2D( const ci::Vec3f& a1, const ci::Vec3f& a2, const ci::Vec3f& b1, const ci::Vec3f& b2, ci::Vec3f& p ) ;
	bool IntersectSegments2D( const ci::Vec2f& a1, const ci::Vec2f& a2, const ci::Vec2f& b1, const ci::Vec2f& b2, ci::Vec2f& p);

	// �����Ƌ�`�̏Փ˔���
	// arg3... ��_
	//bool IntersectSegmentRectangle( const Segment* pS, const Rect* pR, const ci::Vec3f* pC );

	// �O�p�`�̕����t�ʐς�2�{��Ԃ��A���ʂ�abc�������v���̏ꍇ�ɐ�
	// abc�����v���̏ꍇ�ɕ��Aabc���k�ނ��Ă���ꍇ�Ƀ[��
	float Signed2DTriArea( const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c );

	// ��cd����ab���悪�d�Ȃ��Ă��邩�ǂ����𔻒�A�d�Ȃ��Ă����
	// ab�ɉ�������������t�̒l����ь����ʒup���v�Z���ĕԂ�
	bool SegmentSegment( const ci::Vec3f& a, const ci::Vec3f& b, const ci::Vec3f& c, const ci::Vec3f& d, ci::Vec3f& p );

	////////////////////////////////////////////////////////////////////////////////////////

	//AABB(3D)����̕ϊ�
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
	//2D�ėp�v�Z

	//2D�x�N�g���̓���
	float Vec2Dot( const ci::Vec2f& v1, const ci::Vec2f& v2 );

	// 2D�x�N�g���̊O��
	float Vec2Cross( const ci::Vec2f& v1, const ci::Vec2f& v2 ) ;

	float Vec2LengthSq( const ci::Vec2f& v);
	float Vec2Length(const ci::Vec2f& v);

	// 2D�̃x�N�g���̖@���Z�o
	ci::Vec2f Vec2Normal(const ci::Vec2f* pV );
	ci::Vec2f Vec2NormalR(const ci::Vec2f* pV );

	//=================================================================================================================
	//3D�ėp�v�Z

	// �x�N�g���̓���
	float Vec3Dot( const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

	// �x�N�g���̊O��
	ci::Vec3f Vec3Cross( const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

	// �x�N�g���̒���
	float Vec3Length( const ci::Vec3f* pV);
	
	// �x�N�g���̐��K��
	ci::Vec3f Vec3Nomalize( const ci::Vec3f* pV );

	// �X�J���[�O�d��
	float Vec3ScalarTriple( const ci::Vec3f* pV1, const ci::Vec3f* pV2, const ci::Vec3f* pV3 );

	// �d�S���W�𑊑΍��W�ɂ���
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