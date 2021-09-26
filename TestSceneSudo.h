#pragma once

#include "SceneBase.h"
#include "DxLib.h"

class TestSceneSudo :public SceneBase
{
public:

	enum class GAME_SCENE_STATE
	{
		COUNTDOWN,
		GAME
	};

	TestSceneSudo();				//	�R���X�g���N�^
	~TestSceneSudo();			//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	class Player* m_player;			//	�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�
	class Target* m_target[25];		//  �^�[�Q�b�g���i�[����z��
	class Camera* m_camera;			//	�J�����N���X�ւ̃|�C���^�����o�ϐ�
	GAME_SCENE_STATE m_state;		//  �Q�[���V�[���̃X�e�[�^�X������

	int m_targetCount;				//	�A�C�X�̔�΂�����
	int m_startTime;				//	�Q�[���̊J�n����

	int m_soundHandle;				//	�Q�[����ʁE�T�E���h�n���h��
	int m_endSoundHandle;			//�@�S�[�����̃T�E���h�n���h��
	int m_timingImgHandle;			// �@����o�[�̉摜�n���h��
	int m_poolModelHandle;			//�@�v�[���̃��f���n���h��

	bool m_finishFlag;				//	�Q�[���I������t���O
	bool m_fadeOutFlag;				//	�t�F�[�h�A�E�g�J�n�p�̃t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O

	bool m_finishSoundFlag;			//	�S�[�����̃T�E���h����t���O
	int  m_finishFadeCount;			//	�S�[�����̃t�F�[�h�A�E�g�p�t���[���J�E���g
	int  m_targetSpeed;				//	�^�[�Q�b�g�̑��x
};