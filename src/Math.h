//�v�Z����Ƃ��Ɏg����֐��Q
//
//3D��ɍ����悤�ɂ��� 2016/5/??
//

#pragma once

#include "cinder/gl/gl.h"

//2D�p========================================================================

// ���ρA�O�ρA�@��  (2D)
float Vec2Dot( const ci::Vec2f* v1, const ci::Vec2f* v2 );
float Vec2Cross( const ci::Vec2f* v1, const ci::Vec2f* v2 );
ci::Vec2f Vec2Normal(const ci::Vec2f* pV );
ci::Vec2f Vec2Normalize(const ci::Vec2f* pV );
float Vec2Length( const ci::Vec2f* pV);
float Vec2LengthSq( const ci::Vec2f* pV);

//vector3.z�͖���



//���W�̃��[�e���V����
void Rotate2D(float x1, float y1, float alpha, float& x2, float& y2);
void Rotate2DVec(const ci::Vec3f* p1, float rot, ci::Vec3f& p2);

//90�x��] [true : 90][false : -90]
ci::Vec3f Rotate90( const ci::Vec3f* pV, const bool turn = false);
void Rotate2D90( float& x, float& y, const float _x, const float _y, bool turn = false );

//����(���W�A��)����P�ʃx�N�g���̐���
ci::Vec3f ToUnit2DVector(float rot);
void UnitVector(float& x, float& y, const float rot);

//2D�x�N�g���̕�����Ԃ�
float GetDir2D( const ci::Vec3f* pV );

//�P�ʃ��[�e�[�V�����x�N�g��
void Rotate2DNomalize(const ci::Vec3f* p1, float rot, ci::Vec3f& p2);

//�X�J���[�ƌ�������x�N�g��
//����1 : �X�J���[
//����2	: ����	(���W�A��)
//�߂�l: �ׂ��Ƃ� 
ci::Vec3f To2DVector( float mag, float rot);

//�׃N�g������X�J���[�ƌ���
//����1 : �x�N�g��
//�߂�l: x�ɃX�J���[�Ay�Ƀ��W�A��
ci::Vec3f ToScalarAndDir(const ci::Vec3f* pV);

//�x�N�g������]
//����1 :���̃x�N�g��
//����1 :���Z�ŉE��](���W�A��)
//�߂�l:�x�N�g��
ci::Vec3f Rotate2DVec(const ci::Vec3f* pVel, float add );

//3D�p================================================================================

//�x�N�g���̏�����
ci::Vec3f Vec3(float x, float y, float z);

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

//90�xYaw��]�����x�N�g��
ci::Vec3f RotateVecY90(const ci::Vec3f* pV, bool turn= false);

//�x�N�g����X����]
ci::Vec3f RotationVectorX( const ci::Vec3f* pV, float rot);

//�x�N�g����Y����]
ci::Vec3f RotationVectorY( const ci::Vec3f* pV, float rot);

//�x�N�g����Z����]
ci::Vec3f RotationVectorZ( const ci::Vec3f* pV, float rot);

//�x�N�g������]������
//arg1... ��]������x�N�g���̃|�C���^
//arg2... ��]���郍�[�e�[�V����
//ans.... ��]���ꂽ�x�N�g��
ci::Vec3f RotationVector( const ci::Vec3f* pV, const ci::Vec3f* pR );
ci::Vec3f RotationVector(const ci::Vec3f* pV, const ci::Matrix44f* pR);

//���[�e�[�V�������x�N�g���ɂ���
//arg... ���[�e�[�V����
//ans... �P�ʃx�N�g��
ci::Vec3f ToUnitVector( const ci::Vec3f* pR );

//�x�N�g�������ꂼ��̎��̃��[�e�[�V�����ŕ\��
float ToRotationX( const ci::Vec3f* pV );
float ToRotationY( const ci::Vec3f* pV );
float ToRotationZ( const ci::Vec3f* pV );

//�x�N�g����Yaw(Y��),Pitch(X��)�̃��[�e�[�V�����ŕ\�� 
//�����f�����͂Ђ�����Ԃ�Ȃ�..Y������v�Z���邽��
//arg... �x�N�g��
//ans... ���[�e�[�V����
ci::Vec3f ToRotationXY( const ci::Vec3f* pV );

//Y�������ɑ΂��Ẳ�]�ʂ��o��
//��X���Ɍ��߂�
ci::Vec3f ToRotationZX( const ci::Vec3f* pV );

//������float�̓�掮
float Square(const float s);

//���Ƌ��̏Փ˔���
//arg1... ����A�̔��a
//arg2... ����B�̔��a
//arg3... ����A�̍��W
//arg4... ����B�̍��W
//ans.... true �Փ�
bool CollideSphere(float a, float b, ci::Vec3f posA, ci::Vec3f posB);

//���Ƌ��̓����蔻��....ray��pos,vector sphere��pos, radius output_intersection_point near and far
//arg ... ���̍��W�A���̃x�N�g���A���̒��S���W�A���̔��a�A���Ƌ��̌�_:���ɋ߂����A���ɉ�����
bool RaySphereCollisionPos(	
	const ci::Vec3f* pRP,	const ci::Vec3f* pRV,
	const ci::Vec3f* pSP,	float sr,
	ci::Vec3f* pOutN, ci::Vec3f* pOutF );


// name... QuaternionTransformCoord
// work.... �N�H�[�^�j�I�������ɂ����x�N�g�����v�Z
// arg1.... �ϊ��O�̃x�N�g��
// arg2.... �N�H�[�^�j�I��
// ret..... ���Z����
//ci::Vec3f QuaternionTransformCoord(const ci::Vec3f& v, const ci::Quaternion& qtn);

//���VectorA, VectorB �� ��]��(axis)�A�p�x�����߂�(angle)
//axis .... �O��
//angle.... ����
ci::Vec3f Vec3TransformCoord( const ci::Vec3f* pV, const ci::Matrix44f* pM );

//����
ci::Vec3f Vec3TransformCoord( const ci::Matrix44f* pM );


void Vec3ToAxisAngle( ci::Vec3f* pAx, float& angle, const ci::Vec3f* pV1, const ci::Vec3f* pV2 );

ci::Matrix44f ToRotation( const ci::Vec3f& v );

ci::Vec3f ToVector(const ci::Matrix44f& m);