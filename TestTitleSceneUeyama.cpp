#include "TestTitleSceneUeyama.h"
#include "DxLib.h"
#include "GameScene_easy.h"
#include "TestSceneUeyama.h"

// �ő哧�ߗ�
const int MAX_TRANSP_VAL = 255;
// ���ߗʕω��p�x�N�g��
const int TRANSP_MODERATION = -1;

// �S��Փx
const int ALL_DIFFICULT = 3;

const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

const int VOLUME_PAL_SUP = 130;

//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;


TestTitleSceneUeyama::TestTitleSceneUeyama()
	:m_state(TITLE_TRANS_STATE::FIRST_ENTER)
	, m_cursolNum(0)
	, m_guidanceY(0)
	, m_TitleLogox(0)
	, m_Waterx(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	,m_guidanceYFlag(false)
	, m_TitleLogoxFlag(false)
	, m_WaterxFlag(false)

{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}
	// ���ߗʕϐ���122�ɐݒ�
	m_transpVal = MAX_TRANSP_VAL;
	m_transVal_Enter = MAX_TRANSP_VAL;
	// �����ߗʕϐ����P�ɐݒ�
	m_permeationAmount = 1;

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");

}

TestTitleSceneUeyama::~TestTitleSceneUeyama()
{
	StopSoundMem(m_soundHandle);
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_click_sound_handle);		//	ENTER�Ői�ނƂ��̃T�E���h�����������
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_guidanceGraphHandle);
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns> �V�[���̃|�C���^ </returns>
SceneBase* TestTitleSceneUeyama::Update(float _deltaTime)
{

	// �X�e�[�g�����g���Ƃɏ�����ύX
		// ENTER�Ŏ��̃X�e�[�g��
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL, FALSE);//�H�t���O��邩�ǂ���
		return new GameSceneEasy();
	
	}


	return this;
}

void TestTitleSceneUeyama::Draw()
{


	if (!m_fadeInFinishFlag)
	{
		// �t�F�[�h�C������
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);

			// �O���t�B�b�N��`��
			DrawGraph(0, 0, m_backGraphHandle, TRUE);
			DrawGraph(m_TitleLogox, 0, m_logoGraphHandle, TRUE);
			DrawGraph(m_Waterx, 0, m_water, TRUE);

			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}
	// ���ߗʍX�V
	UpdateTransparent();

	// �o�b�N�O���E���h
	DrawGraph(0, 0, m_backGraphHandle, TRUE);
	

	// �I�����ɃJ�[�\�������₷���悤�ɔw�i���Â�����
	//if (m_state == TITLE_TRANS_STATE::SECOND_CHOICE)
	//{
	//	// ���߂��ĕ`��
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	//	DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
	//	// ���߂����ɖ߂�
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}

	// �X�e�[�g�����g���Ƃɏ�����ύX
	//switch (m_state)
	//{
	//case TITLE_TRANS_STATE::FIRST_ENTER:

		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
		// �^�C�g�����S
		DrawGraph(m_TitleLogox, 0, m_logoGraphHandle, TRUE);
		DrawGraph(m_Waterx, 0, m_water, TRUE);
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transVal_Enter);
		DrawGraph(0, m_guidanceY, m_guidanceGraphHandle, TRUE);		//	PUSH ENTER
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	break;

	//case TITLE_TRANS_STATE::SECOND_CHOICE:


		// �t�F�[�h�A�E�g����
		//if (m_fadeOutFlag)
		//{
		//	for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		//	{
		//		// �`��P�x���Z�b�g
		//		SetDrawBright(255 - i, 255 - i, 255 - i);

		//		// �O���t�B�b�N��`��
		//		DrawGraph(0, 0, m_backGraphHandle, FALSE);
		//		ScreenFlip();
		//	}
		//	m_fadeOutFinishFlag = true;
		//	
		//}
	//	break;

	//default:

	//	break;


}

/// <summary>
/// �T�E���h�̍Đ�����
/// </summary>
void TestTitleSceneUeyama::Sound()
{
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal+150, m_soundHandle);
}

/// <summary>
/// �ǂݍ��ݏ���
/// </summary>
void TestTitleSceneUeyama::Load()
{
	m_backGraphHandle = LoadGraph("data/img/Title/Title_Back.png");			//	�O���t�B�b�N�n���h���Ƀ^�C�g����ʂ̃C���[�W���Z�b�g
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");			//	�O���t�B�b�N�n���h���Ƀ^�C�g����ʂ̃C���[�W���Z�b�g
	m_guidanceGraphHandle = LoadGraph("data/img/Title/Title_guidance.png");	//	�O���t�B�b�N�n���h���Ƀ^�C�g����ʂ̃C���[�W���Z�b�g
	m_water = LoadGraph("data/img/Title/title_Water.png");                 //	�O���t�B�b�N�n���h���Ƀ^�C�g����ʂ̃C���[�W���Z�b�g

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");
	m_click_sound_handle = LoadSoundMem("data/sound/Title/SwimTitleSE.wav");	//	ENTER�Ői�ލۂ̃T�E���h�����[�h
}

/// <summary>
/// �_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
/// </summary>
void TestTitleSceneUeyama::UpdateTransparent()
{

	//�^�C�g���A���摜�̃t�F�[�h����
	// ���ߗʂ�255���傫���Ȃ�����
	if (m_transpVal >= MAX_TRANSP_VAL)
	{
		// ���ߗʂ�255�ɐݒ�
		m_transpVal = MAX_TRANSP_VAL - 1;
	
		// �����ߗʂ�-1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// ���ߗʂ�0��菬�������
	else if (MAX_TRANSP_VAL / 3 >= m_transpVal)
	{
		// ���ߗʂ�ݒ�
		m_transpVal = MAX_TRANSP_VAL / 3 + 1;
	
		// �����ߗʂ�1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// �����ߗʂ𓧉ߗʂɉ��Z����
	m_transpVal += m_permeationAmount;


	//Enter to Start�̃t�F�[�h����
	// ���ߗʂ�255���傫���Ȃ�����
	if (m_transVal_Enter >= MAX_TRANSP_VAL)
	{
		// ���ߗʂ�255�ɐݒ�
		m_transVal_Enter = MAX_TRANSP_VAL - 1;

		// �����ߗʂ�-1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// ���ߗʂ�0��菬�������
	else if (MAX_TRANSP_VAL / 2 >= m_transVal_Enter)
	{
		// ���ߗʂ�ݒ�
		m_transVal_Enter = MAX_TRANSP_VAL / 2 + 1;

		// �����ߗʂ�1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// �����ߗʂ𓧉ߗʂɉ��Z����
	m_transVal_Enter += m_permeationAmount;



	
	//�^�C�g�����S���㉺�ɓ�����
if (m_guidanceYFlag == false)
{
	m_guidanceY += 0.1f;
	if (m_guidanceY >= 0)
	{
		m_guidanceYFlag = true;
	}
}
else if(m_guidanceYFlag == true)
{
	m_guidanceY -= 0.1f;
	if (m_guidanceY <= -10)
	{
		m_guidanceYFlag = false;
	}
}

////�^�C�g�����S�����E�ɓ�����
//if (m_TitleLogoxFlag == false)
//{
//	m_TitleLogox += 0.2f;
//	if (m_TitleLogox >= 50)
//	{
//		m_TitleLogoxFlag = true;
//	}
//}
//else if (m_TitleLogoxFlag == true)
//{
//	m_TitleLogox -= 0.2f;
//	if (m_TitleLogox <= 0)
//	{
//		m_TitleLogoxFlag = false;
//	}

//}

//���̉摜�����E�ɓ�����
if (m_WaterxFlag == false)
{
	m_Waterx += 0.05f;
	if (m_Waterx >= 10)
	{
		m_WaterxFlag = true;
	}
}
else if (m_WaterxFlag == true)
{
	m_Waterx -= 0.05f;
	if (m_Waterx <= 0)
	{
		m_WaterxFlag = false;
	}

}
}
