#ifndef _RESULT_H_
#define _RESULT_H_

#include "SceneBase.h"
#include "UI.h"

#include "DxLib.h"

class Result : public SceneBase
{
public:
	Result(int playerRanking);			//	�R���X�g���N�^
	~Result();							//�@�f�X�g���N�^

	void UpdateTransparent();

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;				//	�`��
	void Sound()override;				//	���y
	void Load()override;				//	������
private:
	int m_score;						//	�X�R�A�i�[�ϐ�
	int m_playerRanking;				//	�v���C���[�̃����L���O�i�[�ϐ�
	int m_evaluation;
	int m_backgroundGraphHandle;		//	�w�i�̃O���t�B�b�N�n���h��
	int m_guidanceGraphHandle;			//	���S�̃O���t�B�b�N�n���h��
	int m_evaluationGraphHandle[3];		//	�]���̃O���t�B�b�N�n���h��

	int m_transpVal;				// ���ߗ�
	int m_transVal_Enter;				//	�t�F�[�h�A�E�g���̓��ߗ�
	int permeationAmount;// �����ߗ�
	int transParent;// ���ߗ�

	int m_permeationAmount;			// ���t���[�����ߗ�
	int m_click_sound_handle;			//	ENTER�Ői�ނƂ��̃T�E���h�n���h��
	int m_bgmSoundHandle;				//	BGM�̃T�E���h�n���h��
	int m_evaluationSoundHandle[3];		//	�]���̃T�E���h�n���h��
	int m_checkResultFlag = 0;				//	��ʊǗ��p�t���O
	bool m_checkKeyFlag;				//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeInFinishFlag;			//	�t�F�[�h�C���̏I������t���O
	bool m_fadeOutFlag;					//	�t�F�[�h�A�E�g�J�n�p�̃t���O
	bool m_fadeOutFinishFlag;			//	�t�F�[�h�A�E�g�̏I������t���O
	int m_time;

	enum rank
	{
		C,		//0
		B,		//1
		A,		//2
		S 		//3
	};
};

#endif // _RESULT_H_

