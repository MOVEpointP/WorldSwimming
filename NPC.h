//-----------------------------------------------------------------------------
// @brief  �v���C���[�N���X.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _NPC_H_
#define _NPC_H_

#include "DxLib.h"

#define DIVE	0
#define SWIM	1
#define TURN	2
#define RESULT	3



class NPC final
{
public:

	NPC();				// �R���X�g���N�^.
	~NPC();				// �f�X�g���N�^.

	bool KeyPush;			//�L�[�����������ǂ���

	void Update(float _deltaTime);			// �X�V.
	void Draw();			// �`��.


	// ���f���n���h���̎擾.
	//int GetModelHandle() { return m_modelHandle[m_NPCState]; }

	// ���[�V������Ԃ̎擾
	const int GetPlayerState() const { return m_NPCState; }


	// �f�B���N�V������getter/setter.
	//void SetDir(const VECTOR set) { dir = set; }

	// �����蔻�蔼�a�̎擾.
	float GetHitRadius() { return hitRadius; }

	// �O������m_RankCount�����Z����Ƃ��Ɏg�p����
	void addRankCount();

	// �v���C���[�̃����L���O���X�V���邽�߂̕ϐ���Ԃ�
	int GetRankCount() { return m_RankCount; }

	// �S�[��������NPC�̐l��
	int m_RankCount;


private:
	int		m_modelHandle[3][4];			//	���f���n���h��  [NPC�̐l��] [���[�V����]
	int		m_diveModelHandle;		//	�_�C�u���f���n���h��.
	int		m_swimModelHandle;		//�@�j�����f���n���h��.
	int		m_resultModelHandle;	//	�K�C�i�������f���n���h��.
	int		m_NPCState;
	bool    m_moveFlag;
	float   m_motionSpeed;				//�@���f���̃��[�V�����X�s�[�h
	VECTOR	pos[3];			// �|�W�V����.
	VECTOR	velocity[3];		// �ړ���.
	VECTOR	dir[3];			// ��]����.
	float	hitRadius;		// �����蔻��̔��a.
	float TotalTime[3], PlayTime;//���[�V�����̍Đ�����
	int AttachIndex;//���[�V������
	int m_speedDisplay;		//�{���\���̖��𓮂���
	bool m_moveAnimFlag;//���[�V�������ꎞ��~����t���O
	int m_moveCount;

	// ���˃G�t�F�N�g
	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;
	class Player* m_player;			//	�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�

	// player�p�T�E���h�ϐ�
	static int m_sHandle;

	// �ÓI�萔.
	static const float ACCEL;
	static const float MAX_SPEED;
	static const float TRANING_SPEED;
	static const float DEFAULT_DECEL;
	static const float BREAK_DECEL;
	static const float GRIP_DECEL;
	static const float GRIP_POWER;
	static const float COLIDE_DECEL_FAC;

	bool GorlFlag[3];
};

#endif // _PLAYER_H_