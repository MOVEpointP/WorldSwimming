#ifndef _TARGET_H_
#define _TARGET_H_

#include "DxLib.h"
#include "Score.h"

class TestSceneKoga;
class PlayEffect;
class Player;
class UI;

enum Target_State
{
	NO_SHOT, NOW_SHOT, END_SHOT
};

class Target
{
public:
	Target();				// �R���X�g���N�^.
	~Target();				// �f�X�g���N�^.

	void Update(float _deltaTime);			            // �X�V.
	void Draw();			                            // �`��.
	void Reaction(Target* _target, bool _hitFlag);		// ���A�N�V����.


	// �|�W�V������getter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }
	void SetSinglePosX() { m_posX = pos.x; }	                                   //�^�[�Q�b�g�̍��W���Z�b�g
	static void SetTargetSpeedX(int _targetSpeed) { m_targetSpeed = _targetSpeed; }//�X�s�[�h���Z�b�g

	// �f�B���N�V������getter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// ����(accelVec)��Setter
	void SetAccel(float _accel) { m_target_accel = _accel; }

	void SetTargetCount(int _targetCount) { m_targetCount = _targetCount; }
	void SetInterval(int _interval) { m_shotInterval = _interval; }

	Target_State GetIceState() { return m_targetState; }
	void SetIceState(Target_State _iceState) { m_targetState = _iceState; }


	// �����蔻�蔼�a�̎擾.
	float GetHitRadius() { return hitRadius; }


private:
	int		m_handImgHandle;	// ��̉摜�n���h��
	int		m_legImgHandle;		// ���̉摜�n���h��
	int		m_o2ImgHandle;		// �_�f�摜�n���h��
	int		m_FontHandle;		// �t�H���g�n���h��

	VECTOR	pos;				// �^�[�Q�b�g�̃|�W�V����.
	int     m_posX;             //�^�[�Q�b�g�̂����W

	VECTOR	velocity;		    // �ړ���.
	VECTOR	accelVec;           // �����x

	VECTOR	dir;			    // ��]����.

	int		m_targetCount;		// �^�[�Q�b�g�̔ԍ�
	int		m_plusX;			// �^�[�Q�b�g��x���W

	float m_target_accel;		// �^�[�Q�b�g�̉����x

	//	�ÓI�萔
	static const int m_target_X;		// �^�[�Q�b�g�̏���x���W
	static const int m_target_Y;		// �^�[�Q�b�g�̏���y���W
	static const int m_target_Z;		// �^�[�Q�b�g�̏���z���W
	static const int m_score_good;		//�@�O�b�h����̍ۂɓ���X�R�A
	static const int m_score_bad;		//�@�o�b�h����̍ۂɓ���X�R�A
	static const int m_score_perfect;	//�@�p�[�t�F�N�g����̍ۂɓ���X�R�A
	static const int m_before_good;		//	�O�b�h����i�O�j
	static const int m_perfect;     	//�@�p�[�t�F�N�g����
	static const int m_after_good;		//�@�O�b�h����i��j
	static const int m_final_good;		//�@�O�b�h����i�Ō�j
	
	Target_State m_targetState;

	int  m_targerJadgeWord;		//�^�C�~���O�̔��蕶��
	int  m_timeCount;				//����\�L��\�����鎞�Ԃ��J�E���g����
	int  m_targetScore;			//�X�R�A���i�[����ϐ�
	int  m_combo;				//�R���{�����i�[����ϐ�
	static int  m_targetSpeed;	//���ꂼ��̃^�[�Q�b�g�̗���鑬�x�����߂�ϐ�
	int     m_shotInterval;     // �G�l�~�[���΂��Ԋu

	float	hitRadius;		// �����蔻��̔��a.
	int m_goodHandle;
	int m_badHandle;
	int m_perfectHandle;//����̕\��������摜
	int m_comboHandle;//�R���{�̕\�L
	int m_mapchipHandle[10];
	int m_tens_place;
};

#endif // _TARGET_H_