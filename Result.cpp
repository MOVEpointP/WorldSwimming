#include "Result.h"
#include "TestTitleSceneUeyama.h"

const int LOGO_X = 0;
const int LOGO_Y = 0;
const int GUIDANCE_X = 0;
const int GUIDANCE_Y = 0;
const int RESULT_NUM = 3;
const int B_P = 4;   //�����NB�̃X�R�A
const int A_P = 7;   //�����NA�̃X�R�A
const int S_P = 10;  //�����NS�̃X�R�A
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 90;

// �ő哧�ߗ�
const int defaultTrans = 255;
// ���ߗʕω��p�x�N�g��
const int transModeration = -1;

//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;

Result::Result()
	:m_score(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_checkKeyFlag(false)//
	, m_scoreSoundHandle(0)//
	, m_scoreGraphHandle(0)//
	, m_numSoundHandle(0)//
	, m_logoGraphHandle(0)//
	, m_guidanceGraphHandle(0)//
	, m_exitdoorGraphHandle(0)//
	, m_evaluation(0)//
	, m_click_sound_handle(0)//
	, m_bgmSoundHandle(0)//
{
	// ���ߗʕϐ���255�ɐݒ�
	transParent = defaultTrans;
	// �����ߗʕϐ����P�ɐݒ�
	permeationAmount = 1;

	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

}

Result::~Result()
{
	DeleteGraph(m_logoGraphHandle);
	/*DeleteGraph(m_scoreGraphHandle);*/
	for (int i = 0; i < 11; i++)
	{
		DeleteGraph(m_numGraphHandle[i]);
	}
	DeleteGraph(m_evaluationGraphHandle[m_evaluation]);
	DeleteGraph(m_guidanceGraphHandle);
	StopSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	/*DeleteSoundMem(m_scoreSoundHandle);*/
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
	/*m_scoreStr(std::to_string(m_score));*/
	if (m_checkResultFlag <= RESULT_NUM)
	{
		WaitTimer(1000);
		m_checkResultFlag++;
	}
	if (!CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = FALSE;
	}

	// ���ߗʂ�255���傫���Ȃ�����
	if (transParent >= defaultTrans)
	{
		// ���ߗʂ�121�ɐݒ�
		transParent = defaultTrans - 1;
		// �����ߗʂ�-1�ɂ���
		permeationAmount *= transModeration;
	}
	// ���ߗʂ�0��菬�������
	else if (0 >= transParent)
	{
		// ���ߗʂ��P�ɐݒ�
		transParent = 1;
		// �����ߗʂ�1�ɂ���
		permeationAmount *= transModeration;
	}
	// �����ߗʂ𓧉ߗʂɉ��Z����
	transParent += (permeationAmount * 3);

	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)
	{
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_click_sound_handle);
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL);		//	�����Đ����I���܂őҋ@
		return new TestTitleSceneUeyama;
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
			DrawGraph(0, 0, m_logoGraphHandle, TRUE);
			DrawGraph(0, 0, m_exitdoorGraphHandle, TRUE);//

			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}
	DrawGraph(LOGO_X, LOGO_Y, m_logoGraphHandle, TRUE);					//	���U���g��ʂ̃��S��\��
	DrawGraph(0, 0, m_exitdoorGraphHandle, TRUE);//�h�A�\�L

	// ���߂��ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, transParent);
	DrawGraph(GUIDANCE_X, GUIDANCE_Y, m_guidanceGraphHandle, TRUE);		//	���U���g��ʂ̃��S��\��
	// ���߂����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//if (m_checkResultFlag >= 1)
	//{
	//	DrawGraph(0, 0, m_scoreGraphHandle, TRUE);						//	���U���g��ʂ̃��S��\��
	//}
	if (m_checkResultFlag >= 2)
	{
		if (m_score != 0)
		{
			for (int i = m_score - 1; i >= 0; --i)
			{
				DrawGraph(0, 0, m_numGraphHandle[i], TRUE);				//	���U���g��ʂ̃��S��\��
			}
		}
	}
	if (m_checkResultFlag >= 3)
	{
		//DrawGraph(100, -150, m_evaluationGraphHandle[m_evaluation], TRUE);				//	���U���g��ʂ̃��S��\��
		DrawExtendGraph(900-300+50-20+80,400-300+50, 1300+150+50-10-100+80, 600+150-100+50, m_evaluationGraphHandle[m_evaluation], TRUE);
		//�����N�\��+�摜�T�C�Y�k�� �����̐����ix1,y1(�O���t�B�b�N��`�悷���`�̍��㒸�_�̍��W),x2,y2(�O���t�B�b�N��`�悷���`�̉E�����_�{�P�̍��W))

	}
	// �t�F�[�h�A�E�g����
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// �O���t�B�b�N��`��
			DrawGraph(0, 0, m_logoGraphHandle, FALSE);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}

	DrawExtendFormatString(SCREEN_SIZE_W / 2 - GetFontSize(), SCREEN_SIZE_H / 4+90, 4.0, 4.0, GetColor(0, 0, 0), "%d", m_score);
	DrawExtendFormatString(SCREEN_SIZE_W / 2+300 - GetFontSize(), SCREEN_SIZE_H / 3+280, 4.0, 4.0, GetColor(0, 0, 0), "0");
	DrawExtendFormatString(SCREEN_SIZE_W / 2+450 - GetFontSize(), SCREEN_SIZE_H / 3+430, 4.0, 4.0, GetColor(0, 0, 0), "0");

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
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 3:
		PlaySoundMem(m_evaluationSoundHandle[m_evaluation], DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_evaluationSoundHandle[m_evaluation]);
		break;
	}
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(50, m_bgmSoundHandle);//�}�W�b�N�i���o�[�ɂ����̂Ō�Œ���

}

void Result::Load()
{
	m_click_sound_handle = LoadSoundMem("data/sound/swimResult/SwimEnterToSound.mp3");	//	ENTER�Ői�ލۂ̃T�E���h�����[�h
	if (m_score > B_P)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_C.png");		//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_exitdoorGraphHandle = LoadGraph("data/img/result_02_png/Result_Exitdoor.png");	//Enter to start�̃h�A�\�L
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (m_score < B_P)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_B.png");		//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_exitdoorGraphHandle = LoadGraph("data/img/result_02_png/Result_Exitdoor.png");	//Enter to start�̃h�A�\�L
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (m_score >= B_P && m_score < A_P)
	{
		m_evaluation = 1;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_A.png");				//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_exitdoorGraphHandle = LoadGraph("data/img/result_02_png/Result_Exitdoor.png");	//Enter to start�̃h�A�\�L
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");		//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	if (m_score == S_P)
	{
		m_evaluation = 2;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_S.png");			//	@@@@�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
		m_exitdoorGraphHandle = LoadGraph("data/img/result_02_png/Result_Exitdoor.png");	//Enter to start�̃h�A�\�L
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/SwimScoreSE_S.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	}
	/*m_scoreStr(std::to_string(m_score));*/
	m_logoGraphHandle = LoadGraph("data/img/result_02_png/swimResult/FinalResults.png");				//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
	// m_scoreGraphHandle = LoadGraph("data/img/Result_score.png");			//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
	m_guidanceGraphHandle = LoadGraph("data/img/result_02_png/Result _Exitword.png");		//	�O���t�B�b�N�n���h���Ƀ��U���g��ʂ̃C���[�W���Z�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/SwimFinalResultsBGM.mp3");			//	�T�E���h�n���h���Ƀ��U���g��ʂ�BGM���Z�b�g
	/*m_scoreSoundHandle = LoadSoundMem("data/sound/score.mp3");	*/			//	�T�E���h�n���h���Ƀ��U���g��ʂ̌��ʉ����Z�b�g
	
}