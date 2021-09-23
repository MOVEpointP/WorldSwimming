
#ifndef _RESULTHALF_H_
#define _RESULTHALF_H_

#include "SceneBase.h"
#include "UI.h"

#include "DxLib.h"
class ResultHalf : public SceneBase
{
	public:
		ResultHalf();			//	�R���X�g���N�^
		~ResultHalf();			//�@�f�X�g���N�^

		SceneBase* Update(float _deltaTime)override;	//	�X�V
		void Draw()override;			//	�`��
		void Sound()override;			//	���y
		void Load()override;			//	������
		// ���f���n���h���̎擾.
		int GetModelHandle() { return modelHandle; }
	private:
		class Score* m_half_score;		//�@ �X�R�A�N���X�ւ̃|�C���^�����o�ϐ�
		int m_score;						//	�X�R�A�i�[�ϐ�
		int m_evaluation;
		int m_logoGraphHandle;				//	���S�̃O���t�B�b�N�n���h��
		int m_guidanceGraphHandle;			//	�K�C�h�̃O���t�B�b�N�n���h��
		int m_scoreGraphHandle;				//	�X�R�A�̃O���t�B�b�N�n���h��
		int m_evaluationGraphHandle[3];		//	�]���̃O���t�B�b�N�n���h��
		int m_exitdoorGraphHandle;			//�@enter to start �̃h�A�̃n���h��
		// �����ߗ�
		int permeationAmount;
		// ���ߗ�
		int transParent;
		int m_click_sound_handle;			//	ENTER�Ői�ނƂ��̃T�E���h�n���h��
		int m_bgmSoundHandle;				//	BGM�̃T�E���h�n���h��
		int m_scoreSoundHandle;				//	�X�R�A�̃T�E���h�n���h��
		int m_evaluationSoundHandle[3];		//	�]���̃T�E���h�n���h��
		int m_checkResultFlag = 0;				//	��ʊǗ��p�t���O
		bool m_checkKeyFlag;				//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
		bool m_fadeInFinishFlag;			//	�t�F�[�h�C���̏I������t���O
		bool m_fadeOutFlag;					//	�t�F�[�h�A�E�g�J�n�p�̃t���O
		bool m_fadeOutFinishFlag;			//	�t�F�[�h�A�E�g�̏I������t���O
		int	modelHandle;	// ���f���n���h��.
		VECTOR	pos;			// �|�W�V����.
		VECTOR	dir;			// ��]����.
		float TotalTime, PlayTime;//���[�V�����̍Đ�����
		int AttachIndex;//���[�V�����̍Đ�����
		int m_backgroundGraphHandle;
		enum rank
		{
			C,		//0
			B,		//1
			A,		//2
			S 		//3
		};
};

#endif _RESULTHALF_H_
