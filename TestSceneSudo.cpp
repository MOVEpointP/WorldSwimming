#include "TestSceneSudo.h"
#include "ResultHalf.h"
#include "Target.h"
#include "Player.h"
#include "Hitchecker.h"
#include "Camera.h"
#include "Fade.h"
#include "Score.h"
#include "DxLib.h"
#include "Effect.h"
#include "GameScene_Compe.h"

static int enemyNum = 22;					//	�G�l�~�[�̐�
static int COUNTDOWN = 7;

// �^�[�Q�b�g�����ł���Ԋu (�b�P��)
const int TARGET_SHOT_INTERVAL = 2;

// �^�[�Q�b�g�̑��x��������
static float targetSpeed = 200.0f;

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;

// ���ʒ���
const int GONG_VOLUME_PAL = 30;
const int DOOR_VOLUME_PAL = 40;


TestSceneSudo::TestSceneSudo()
	:m_player(nullptr)
	, m_camera(nullptr)
	, m_targetCount(0)
	, m_startTime(0)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_endSoundHandle(0)
	, m_finishSoundFlag(false)
	, m_finishFadeCount(0)
	, m_targetSpeed(0)
	, m_scoreHandle(0)
	, m_tensPlaceScore(0)
	, m_onePlaceScore(0)
	, m_rankBHandle(false)
	, m_rankSound(0)
	, m_enterPushFlag(false)
	, m_spacePushFlag(false)
{
	// ���̃V�[���ֈڍs���邩�ǂ���
	m_finishFlag = FALSE;
	// �G�l�~�[�E�X�R�AUI������
	for (int i = 0; i < enemyNum; i++)
	{
		m_target[i] = nullptr;
	}
	m_target[enemyNum] = nullptr;

	for (int i = 0; i <= 2; i++)
	{
		m_rankSoundFlag[i] = false;
	}

	// �J�n���̃^�C�����擾
	m_startTime = GetNowCount() / 1000;
	// �X�e�[�g�Z�b�g(�J�E���g�_�E������)
	m_state = GAME_SCENE_STATE::COUNTDOWN;

	srand(time(NULL));//�����̎��������
}


TestSceneSudo::~TestSceneSudo()
{
	delete m_player;	//	�v���C���[�̃|�C���^�����o�ϐ�������
	delete m_camera;	//	�J�����̃|�C���^�����o�ϐ�������

	//	�������̉������
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_endSoundHandle);
	DeleteSoundMem(m_rankBHandle);
	DeleteSoundMem(m_rankAHandle);
	DeleteSoundMem(m_rankHandle);

	for (int i = 0; i < enemyNum; i++)
	{
		delete m_target[i];
	}

	delete m_target[enemyNum];
}


SceneBase* TestSceneSudo::Update(float _deltaTime)
{
	// �f�o�b�O�r���h�̂݃f�o�b�O�֐����Ăяo��
#ifdef _DEBUG
	//DebugKey();
#endif

	m_targetSpeed = rand() % 50 + 1;//�^�[�Q�b�g�̑��x��ς���

	m_player->SetScene(false);

	//�f�o�b�N�p
	if (CheckHitKey(KEY_INPUT_0))
	{
		return new GameSceneCompe();
	}


	switch (m_state)
	{
	case GAME_SCENE_STATE::COUNTDOWN:
		if ((COUNTDOWN + 1) - (GetNowCount() / 1000 - COUNTDOWN) <= 1)
		{
			m_startTime = GetNowCount() / 1000;
			m_state = GAME_SCENE_STATE::GAME;
		}
		break;

	case GAME_SCENE_STATE::GAME:
		// �G�l�~�[�ˏo�Ǘ�
		//if (GetNowCount() / 1000 - m_startTime > COUNTDOWN)//TARGET_SHOT_INTERVAL��ς��Ďˏo�^�C�~���O�𒲐�����
		if (m_player->GetPlayerState() == SWIM)
		{
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				m_startTime = GetNowCount() / 1000;
				if (m_target[m_targetCount]->GetIceState() == NO_SHOT)//NO_SHOT�̏ꍇ
				{
					m_target[m_targetCount]->SetIceState(NOW_SHOT);//�X�e�[�^�X��NOW_SHOT���Z�b�g 
					Target::SetTargetSpeedX(m_targetSpeed);
				}
				if (m_target[m_targetCount]->GetIceState() == END_SHOT)//END_SHOT�̏ꍇ
				{
					m_targetCount++;			//���̃G�l�~�[�ɃJ�E���g��i�߂�
				}
			}
		}

		//�X�y�[�X�L�[���������ԉ摜�\�����ς��
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			m_spacePushFlag = true;

		}
		else
		{
			m_spacePushFlag = false;
		}

		//�G���^�[�L�[���������ԉ摜�\�����ς��
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_enterPushFlag = true;
		}
		else
		{
			m_enterPushFlag = false;
		}


		// ���݂̔ԍ��ɉ����ăG�l�~�[�̍X�V
		m_target[m_targetCount]->Update(_deltaTime);
		m_target[m_targetCount]->SetTargetCount(m_targetCount);

		// �A�C�R����x���̃|�W�V�������擾
		m_target[m_targetCount]->SetSinglePosX();//�^�[�Q�b�g�ɂ����W���Z�b�g

		Score::calcScore(m_onePlaceScore, m_tensPlaceScore);

		m_target[m_targetCount]->Reaction(m_target[m_targetCount], false);

		m_player->Update(_deltaTime);

		m_camera->Update(*m_player);


		if (CheckHitKey(KEY_INPUT_F))
		{
			return new ResultHalf();        //    ���U���g�V�[���ɐ؂�ւ���

		}

		//�@���K�ʕ�����������������
		if (m_player->ResultSceneFlag)
		{
			m_finishFlag = TRUE;
		}
		if (m_finishFlag == TRUE)
		{
			m_finishSoundFlag = true;
		}
		if (m_fadeOutFinishFlag)
		{
			// scoreUI�̃X�R�A��Result��score�ϐ��ɃZ�b�g
			return new ResultHalf();        //    ���U���g�V�[���ɐ؂�ւ���
		}
		break;

	default:
		break;

	}
	return this;							//	�Q�[���V�[����\����������
}


void TestSceneSudo::Draw()
{
	//�v�[���̕\���ʒu�ύX
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));

	//�v�[���̕`��
	MV1DrawModel(m_poolModelHandle);

	if (m_player->GetPlayerState() == SWIM)
	{
		DrawGraph(0, 0, m_timingImgHandle, TRUE);

		DrawGraph(0, 0, m_spaceHandle, TRUE);

		DrawGraph(0, 0, m_enterHandle, TRUE);

		// �^�[�Q�b�g(�A�C�R��)
		for (int i = 0; i <= m_targetCount; i++)
		{
			m_target[i]->Draw();
		}
		if (m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spacePushHandle, TRUE);
		}
		if (m_enterPushFlag)
		{
			DrawGraph(0, 0, m_enterPushHandle, TRUE);
		}

		DrawGraph(1920 / 2 + 500 + 20, 400, m_mapChipHandle[m_tensPlaceScore], TRUE);
		DrawGraph(0, 0, m_scoreHandle, TRUE);
		DrawGraph(1920 / 2 + 580, 400, m_mapChipHandle[m_onePlaceScore], TRUE);

	}

	// �v���[���[
	m_player->Draw();

	if (m_finishSoundFlag)
	{
		DrawExtendFormatString(SCREEN_SIZE_W / 2 - GetFontSize(), SCREEN_SIZE_H / 2, 4.0, 4.0, GetColor(0, 0, 0), "�I���I");

		m_finishFadeCount = GetNowCount() / 1000;

		if (m_finishFadeCount > 1000)
		{
			m_fadeOutFlag = true;
		}
	}

	// �t�F�[�h�A�E�g����
	if (m_fadeOutFlag)
	{
		Fade::FadeOut(false);
		m_fadeOutFinishFlag = true;
	}

	if (m_player->GetPlayerState() == DIVE)
	{
		DrawExtendFormatString(1920 / 2 - 170 - GetFontSize(), 1080 - 100, 4.0, 4.0, GetColor(0, 0, 0), "SPACE�Ŕ�т���");
	}


}

void TestSceneSudo::Sound()
{
	//���KBGM�̍Đ�
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_LOOP, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 50, m_soundHandle);

	if (m_finishSoundFlag)
	{
		PlaySoundMem(m_endSoundHandle, DX_PLAYTYPE_NORMAL);
	}

	switch (Score::SetRank())
	{
	case 0:
		break;
	case 1:

		if (CheckSoundMem(m_rankBHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankBHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	case 2:
		if (CheckSoundMem(m_rankAHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankAHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	case 3:
		if (CheckSoundMem(m_rankHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	}

}

void TestSceneSudo::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");		//���KBGM�n���h��
	m_timingImgHandle = LoadGraph("data/img/gameScene/timing2.png");		//����o�[�̉摜�n���h��
	m_scoreHandle = LoadGraph("data/img/gameScene/score.png");
	m_spaceHandle = LoadGraph("data/img/gameScene/space.png");
	m_spacePushHandle = LoadGraph("data/img/gameScene/spacePush.png");
	m_enterHandle = LoadGraph("data/img/gameScene/enter.png");
	m_enterPushHandle = LoadGraph("data/img/gameScene/enterPush.png");
	m_rankBHandle = LoadSoundMem("data/sound/Game/01.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_rankAHandle = LoadSoundMem("data/sound/Game/01.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_rankHandle = LoadSoundMem("data/sound/Game/01.mp3");		//�X�R�A�̌��ʉ��n���h��

	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapChipHandle);

	//	���f���n���h���ɃZ�b�g
	m_poolModelHandle = MV1LoadModel("data/model/stage/stage2/poolModel2.pmx");
	//	�T�E���h�n���h���ɃZ�b�g
	m_endSoundHandle = LoadSoundMem("data/sound/Game/finish.mp3");//���K�̃��j���[���̋�����B���������ɗ������ʉ�

	m_player = new Player;			//	�v���C���[�N���X�̃C���X�^���X�𐶐�
	m_camera = new Camera;			//	�J�����N���X�̃C���X�^���X�𐶐�
	for (int i = 0; i < (enemyNum + 1); i++)
	{
		m_target[i] = new Target;
		m_target[i]->SetInterval(TARGET_SHOT_INTERVAL);
		m_target[i]->SetAccel(targetSpeed);
	}
}

void TestSceneSudo::DebugKey()
{
	// �m�F�p
	if (CheckHitKey(KEY_INPUT_A))
	{
	}
	if (CheckHitKey(KEY_INPUT_B))
	{
	}
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
	}
}
