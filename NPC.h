//-----------------------------------------------------------------------------
// @brief  �v���C���[�N���X.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _NPC_H_
#define _NPC_H_

#define NPC_NUMBER 7

#include "DxLib.h"

#define DIVE		0		//�{�ԍŏ��̃J����
#define SWIM		1		//���K�{��
#define TURN		2       //�^�[��
#define RESULT 		3		//���U���g
#define COMPE_FIRST	4		//���K���̂̃_�C�u
#define COMPE_DIVE  5		//�{�Ԓ��̃_�C�u
#define COMPE_SWIM  6		//�{�Ԃ�SWIM

class NPC final
{
public:
	NPC();				// �R���X�g���N�^.
	~NPC();				// �f�X�g���N�^.
	bool KeyPush;			//�L�[�����������ǂ���
	void Update(float _deltaTime);			// �X�V.
	void Draw();			// �`��.
	// ���[�V������Ԃ̎擾
	const int GetPlayerState() const { return m_NPCState; }
	// �O������m_RankCount�����Z����Ƃ��Ɏg�p����
	void addRankCount();
	// �v���C���[�̃����L���O���X�V���邽�߂̕ϐ���Ԃ�
	int GetRankCount() { return m_rankcount; }

	static void SetDiveFlag(int _diveFlag) { m_diveFlag = _diveFlag; }
	// �S�[��������NPC�̐l��
private:
	int		m_modelHandle[NPC_NUMBER][7];			//	���f���n���h��  [NPC�̐l��] [���[�V����]
	int		m_NPCState;
	float   m_motionSpeed[NPC_NUMBER];					//�@���f���̃��[�V�����X�s�[�h
	VECTOR	pos[NPC_NUMBER];							// �|�W�V����.
	VECTOR	velocity[NPC_NUMBER];					// �ړ���.
	VECTOR	dir[NPC_NUMBER];							// ��]����.
	VECTOR  NPCDir[NPC_NUMBER];
	float TotalTime[NPC_NUMBER], PlayTime[NPC_NUMBER];			//���[�V�����̍Đ�����
	int AttachIndex;						//���[�V������
	class Player* m_player;					//�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�
	// �ÓI�萔.
	static const float ACCEL;
	bool m_goalFlag[NPC_NUMBER];
	int m_rankcount;
	float m_npcGoalAccel[NPC_NUMBER];					// NPC�̃X�s�[�h����
	int		m_npcState;
	int m_startTime;
	static bool m_diveFlag;
	bool m_measureDistanceFlag;

	//�����Ԃ��̃G�t�F�N�g
	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;
	VECTOR m_efkPos;
	bool efkFlag;
	int m_efkTime;
	int m_efkstartTime;
	float m_npcScoreSpeed[NPC_NUMBER];
	float m_npcMotionAccelTmp[NPC_NUMBER + 10];
	float m_npcMptionAccel[NPC_NUMBER];

};

#endif //_NPC_H_