#ifndef _TARGET_H_
#define _TARGET_H_

#include "DxLib.h"
#include "Score.h"

class ObstructBase;
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

	void Update(float _deltaTime);			// �X�V.
	void Draw();			// �`��.
	void Reaction(UI* _ui, bool _hitFlag);		// ���A�N�V����.



	// ���f���n���h���̎擾.
	int GetModelHandle() { return modelHandle; }

	// �|�W�V������getter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }
	void SetSinglePosX() { m_posX = pos.x; }	//�^�[�Q�b�g�̍��W���Z�b�g
	const int GetPosX() { return m_posX; }
	static void SetTargetSpeedX(int _targetSpeed) { m_targetSpeed = _targetSpeed; }//�X�s�[�h���Z�b�g

	// �f�B���N�V������getter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// ����(accelVec)��Setter
	void SetAccel(float _accel) { m_target_accel = _accel; }

	// �����蔻�蔼�a�̎擾.
	float GetHitRadius() { return hitRadius; }


	//void SetTargetJudge(int _targetJadge) { m_targetJadge = _targetJadge; }
	void SetTargetCount(int _targetCount) { m_targetCount = _targetCount; }
	void SetSetTime(int _setTime) { m_setTime = _setTime; }
	void SetInterval(int _interval) { m_shotInterval = _interval; }

	void SetThrowSound(int _sound) { m_throwIceSoundHandle = _sound; }
	void SetHitSound(int _sound) { m_hitIceSoundHandle = _sound; }
	void SetMissSound(int _sound) { m_missIceSoundHandle = _sound; }
	void SetIceType(int _iceType) { m_iceType = _iceType; }
	int GetIceType() { return m_iceType; }

	void SetHitIce(bool _hitFlag) { m_hitFlag = _hitFlag; }
	bool GetHitIce() { return m_hitFlag; }

	static void ScoreUpdateUI(UI& _ui, bool _hitFlag);

	Target_State GetIceState() { return m_iceState; }
	void SetIceState(Target_State _iceState) { m_iceState = _iceState; }

private:
	int		modelHandle;		// ���f���n���h��.
	int		m_handImgHandle;	// ��̉摜�n���h��
	int		m_legImgHandle;		// ���̉摜�n���h��
	int		m_o2ImgHandle;		// �_�f�摜�n���h��
	int		m_FontHandle;		// �t�H���g�n���h��
	VECTOR	pos;				// �|�W�V����.
	int m_posX;

	VECTOR	velocity;		// �ړ���.
	VECTOR	accelVec;

	VECTOR	dir;			// ��]����.
	float	hitRadius;		// �����蔻��̔��a.
	int		timenow;		// �o�ߎ���.
	bool	m_hitFlag;

	int		m_throwIceSoundHandle;
	int		m_hitIceSoundHandle;
	int		m_missIceSoundHandle;


	int		m_targetCount;
	int		m_plusX;
	int		m_setTime;
	int     m_shotInterval;     // �G�l�~�[���΂��Ԋu
	int		m_iceType;

	float m_target_accel;

	//	�ÓI�萔
	//static const float MAX_SPEED;
	//static const float DEFAULT_DECEL;
	//static const float BREAK_DECEL;
	//static const float GRIP_DECEL;
	//static const float GRIP_POWER;
	//static const float COLIDE_DECEL_FAC;*/
	static const int m_target_X;
	static const int m_target_Y;
	static const int m_target_Z;
	static const int m_font_X;
	static const int m_font_Y;
	static const int m_font_size;
	static const int m_font_thick;
	static const int m_score_good;		//�@�O�b�h����̍ۂɓ���X�R�A
	static const int m_score_bad;		//�@�o�b�h����̍ۂɓ���X�R�A
	static const int m_score_perfect;	//�@�p�[�t�F�N�g����̍ۂɓ���X�R�A
	static const int m_before_good;		//	�O�b�h����i�O�j
	static const int m_perfect;	//�@�p�[�t�F�N�g����
	static const int m_after_good;		//�@�O�b�h����i��j
	static const int m_final_good;		//�@�O�b�h����i�Ō�j



	
	Target_State m_iceState;

	int  m_targetJadge;			//�^�C�~���O�̔��茋��
	int  m_targerJadgeWord;		//�^�C�~���O�̔��蕶��
	int  TimeCount;				//�o�ߎ��Ԃ��J�E���g����
	int  m_targetScore;			//�X�R�A���i�[����ϐ�
	int  m_combo;				//�R���{�����i�[����ϐ�
	static int  m_targetSpeed;			//���ꂼ��̃^�[�Q�b�g�̗���鑬�x�����߂�ϐ�
};

#endif // _TARGET_H_