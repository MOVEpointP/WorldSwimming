#pragma once
#include "SceneBase.h"


class Title : public SceneBase
{
public:


	Title();					//	�R���X�g���N�^
	~Title();					//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();

	//	�f���^�^�C���̃Z�b�^�[
	void SetDeltaTime(float _deltaTime) { m_deltaTime = _deltaTime; }
	//	�f���^�^�C���̃Q�b�^�[
	float GetDeltaTime() { return m_deltaTime; }
private:
	int m_backGraphHandle;				//	�^�C�g����ʁE�w�i�̃O���t�B�b�N�n���h��
	int m_logoGraphHandle;				//	�^�C�g����ʁE���S�̃O���t�B�b�N�n���h��
	int m_guidanceGraphHandle;			//	�^�C�g����ʁE�K�C�h�̃O���t�B�b�N�n���h��
	int m_soundHandle;					//	�^�C�g����ʁE�T�E���h�n���h��
	int m_water;
	int m_click_sound_handle;			//  ENTER�Ői�ނƂ��̃T�E���h�n���h��						
	int m_manualGraphHandle;			//�}�j���A���̃O���t�n���h��

	int m_permeationAmount;			// ���t���[�����ߗ�
	int m_transpVal;				// ���ߗ�
	int m_transVal_Enter;				//	�t�F�[�h�A�E�g���̓��ߗ�
	float m_deltaTime;				//	�f���^�^�C��

	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeInFinishFlag;		//	�t�F�[�h�C���̏I������t���O
	bool m_fadeOutFlag;				//	�t�F�[�h�A�E�g�J�n�p�̃t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O

	float m_guidanceY;               //	�^�C�g�����S��y���W
	float m_TitleLogox;               //	�^�C�g�����S��x���W
	bool m_TitleLogoxFlag;
	bool m_guidanceYFlag;
	float m_Waterx;               //	���摜��x���W
	bool m_WaterxFlag;
	bool m_gameSceneFlag;
};