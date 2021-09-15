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

	NPC();				                    // �R���X�g���N�^.
	~NPC();				                    // �f�X�g���N�^.

	bool KeyPush;			                //�L�[�����������ǂ���

	void Update(float _deltaTime);			// �X�V.
	void Draw();			                // �`��

	// �O������m_RankCount�����Z����Ƃ��Ɏg�p����
	void addRankCount();
	// �v���C���[�̃����L���O���X�V���邽�߂̕ϐ���Ԃ�
	int GetRankCount() { return m_RankCount; }
	// �S�[��������NPC�̐l��
	int m_RankCount;
private:
	int		m_modelHandle[3][4];			//	���f���n���h��  [NPC�̐l��] [���[�V����]
	int		m_NPCState;
	VECTOR	pos[3];							// �|�W�V����.
	VECTOR	velocity[3];					// �ړ���.
	VECTOR	dir[3];							// ��]����
	bool GorlFlag[3];						//���ꂼ���NPC���S�[�����������肷��t���O
	
	float   m_motionSpeed;					//�@���f���̃��[�V�����X�s�[�h
	float TotalTime[3], PlayTime;			//���[�V�����̍Đ�����
	int AttachIndex;						//�A�j���[�V�����̑��Đ�����

	class Player* m_player;					//	�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�

	// �ÓI�萔.
	static const float ACCEL;
};

#endif // _NPC_H_