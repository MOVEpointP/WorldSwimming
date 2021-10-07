#include "Result.h"
#include "Score.h"
#include "Time.h"
#include "Title.h"
const int TRANSP_MODERATION = -1;
const int LOGO_X = 0;
const int LOGO_Y = 0;
const int RESULT_NUM = 3;
const int B_P = 4;   //�����NB�̃X�R�A
const int A_P = 7;   //�����NA�̃X�R�A
const int S_P = 10;  //�����NS�̃X�R�A
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 90;
// �ő哧�ߗ�
const int MAX_TRANSP_VAL = 200;
// ���ߗʕω��p�x�N�g��
const int transModeration = -1;
//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;

Result::Result(int playerRanking)
	:m_score(0)
	, m_playerRanking(playerRanking)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_checkKeyFlag(false)
	, m_backgroundGraphHandle(0)
	, m_evaluation(0)
	, m_click_sound_handle(0)
	, m_bgmSoundHandle(0)
	, m_time(0)
	, m_oneTime(0)
	, m_tenTime(0)
	, m_decimalTime(0)
{
	// ���ߗʕϐ���122�ɐݒ�
	m_transpVal = MAX_TRANSP_VAL;
	m_transVal_Enter = MAX_TRANSP_VAL;
	// �����ߗʕϐ����P�ɐݒ�
	m_permeationAmount = 2;
	m_time = Time::GetTime();
}

Result::~Result()
{
	DeleteGraph(m_backgroundGraphHandle);
	DeleteGraph(m_evaluationGraphHandle[m_evaluation]);
	StopSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_evaluationSoundHandle[m_evaluation]);
	DeleteSoundMem(m_click_sound_handle);					//	ENTER�Ői�ނƂ��̃T�E���h�����������
}

SceneBase* Result::Update(float _deltaTime)
{


	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_volumePal++;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_volumePal--;
	}
	if (m_checkResultFlag <= RESULT_NUM)
	{
		WaitTimer(1000);
		m_checkResultFlag++;
	}
	if (!CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = FALSE;
	}
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)
	{
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_click_sound_handle);
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL);		//	�����Đ����I���܂őҋ@
		return new Title;
	}


	return this;
}

void Result::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// �t�F�[�h�C������
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backgroundGraphHandle, TRUE);
			DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}

	// ���ߗʍX�V
	UpdateTransparent();

	DrawGraph(LOGO_X, LOGO_Y, m_backgroundGraphHandle, TRUE);
	// ���߂��ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
	//	���U���g��ʂ̃��S��\��
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);
	// ���߂����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// ���߂��ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transVal_Enter);
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	PUSH ENTER
	// ���߂����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// �t�F�[�h�A�E�g����
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// �O���t�B�b�N��`��
			DrawGraph(0, 0, m_backgroundGraphHandle, FALSE);
			DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);

			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}

	//�^�C���̌��ʎ擾
	Time::calcTime(m_oneTime, m_tenTime, m_decimalTime);
	//�^�C���\��
	DrawGraph(950, SCREEN_SIZE_H / 3 , m_mapChip[m_decimalTime], TRUE);
	DrawGraph(750, SCREEN_SIZE_H / 3, m_mapChip[m_oneTime], TRUE);
	DrawGraph(650, SCREEN_SIZE_H / 3, m_mapChip[m_tenTime], TRUE);

	//DrawExtendFormatString(SCREEN_SIZE_W / 2+300 - GetFontSize(), SCREEN_SIZE_H / 3+280, 4.0, 4.0, GetColor(0, 0, 0), "%d�b", m_time);

	//�v���C���[�̏��ʕ\��
	DrawGraph(950, SCREEN_SIZE_H / 3 + 260, m_mapChip[m_playerRanking], TRUE);
	//DrawExtendFormatString(SCREEN_SIZE_W / 2+450 - GetFontSize(), SCREEN_SIZE_H / 3+430, 4.0, 4.0, GetColor(0, 0, 0), "%d", m_playerRanking);
}

void Result::Sound()
{
	switch (m_checkResultFlag)
	{
	case 1:
		/*PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 2:
		/*PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m5_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 3:
		PlaySoundMem(m_evaluationSoundHandle[m_evaluation], DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_evaluationSoundHandle[m_evaluation]);
		break;
	}
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal+100, m_bgmSoundHandle);

}

void Result::Load()
{
	m_click_sound_handle = LoadSoundMem("data/sound/swimResult/SwimEnterToSound.mp3");	//	ENTER�Ői�ލۂ̃T�E���h�����[�h
	if (Score::SetRank()==C)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_C.png");		//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (Score::SetRank() == B)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_B.png");		//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (Score::SetRank() == A)
	{
		m_evaluation = 1;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_A.png");				//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");		//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (Score::SetRank() == S)
	{
		m_evaluation = 2;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_S.png");			//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/SwimScoreSE_S.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	m_backgroundGraphHandle = LoadGraph("data/img/result_02_png/swimResult/FinalResults.png");				//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̔w�i�C���[�W���Z�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/SwimFinalResultsBGM.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ�BGM���Z�b�g
	m_guidanceGraphHandle = LoadGraph("data/img/result_02_png/swimResult/EnterToTitle.png");;			//	���S�̃O���t�B�b�N�n���h��
	LoadDivGraph("data/img/result_02_png/swimResult/mapchip.png", 10, 10, 1, 150, 150, m_mapChip);

}
 //�_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
void Result::UpdateTransparent()
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
}
