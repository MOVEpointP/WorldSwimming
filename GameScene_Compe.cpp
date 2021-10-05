#include "GameScene_Compe.h"
#include "Result.h"
#include "Target.h"
#include "Player.h"
#include "Hitchecker.h"
#include "Camera.h"
#include "NPC.h"
#include "DxLib.h"
#include "Effect.h"
#include "Time.h"
#include "Fade.h"


//static int COUNTDOWN = 7;					//	�J�E���g�_�E���̕b���i+2�j
//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;
const int m_liveX = 45;
const int m_liveY = 38;//���C�u�\�L�̍��W
const int m_countryTime = 2;//���̉摜��\�����鎞��

int GameSceneCompe::m_turnFlag= false;


GameSceneCompe::GameSceneCompe()
	: m_player(nullptr)
	, m_camera(nullptr)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_npc(nullptr)
	, m_MaxGorl(false)
	, m_playerRanking(0)
	, m_countryDrawFlag(false)
	,m_timeplayer(0)
	,m_timeFlag(false)
	, m_countryTime(0)
	, m_countryFadeFlag(false)
	, m_countryTopX(0)
	, m_countryUnderX(SCREEN_SIZE_W)
	, m_turnGraphHandle(0)
	, m_spaceDiveGraph(0)
	, m_spacePushFlag(false)
	, m_spaceDivePushGraph(false)
	, m_liveGraph(0)
{

	// ���̃V�[���ֈڍs���邩�ǂ���
	m_finishFlag = FALSE;
	// �X�e�[�g�Z�b�g(�J�E���g�_�E������)
	m_state = GAME_SCENE_STATE::COUNTDOWN;
	KeyPush = false;
	
}

GameSceneCompe::~GameSceneCompe()
{
	delete m_player;	//	�v���C���[�̃|�C���^�����o�ϐ�������
	delete m_camera;	//	�J�����̃|�C���^�����o�ϐ�������
	//	�������̉������
	DeleteSoundMem(m_soundHandle);

	delete m_npc;
	DeleteGraph(m_countryGraphHandle);
}

SceneBase* GameSceneCompe::Update(float _deltaTime)
{
	// �f�o�b�O�r���h�̂݃f�o�b�O�֐����Ăяo��
#ifdef _DEBUG
	DebugKey();
#endif


		m_player->SetScene(true);
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);
		m_npc->Update(_deltaTime);
		// �v���C���[�̃����L���O�ۑ��ϐ���NPC�����̃S�[���������̐�������
		// �v���C���[���S�[��������(�t���O��getter�Ŏ擾���Ă��܂�)
		static bool isProcess = false;
		if (m_player->GetGoalFlag() && !isProcess)
		{
			m_npc->addRankCount();
			// ���Z����RankCount���擾���ă��U���g�ɏ��ʂƂ��ĕ\��
			m_playerRanking = m_npc->GetRankCount();
			isProcess = true;
			m_timeFlag = false;
		}

		//�v���C���[���j���n�߂��獑�Ƃ̕\��
		if (m_player->GetPlayerState() == COMPE_SWIM)
		{
			if (m_timeFlag == false&& !m_player->GetGoalFlag())
			{
				// �J�n���̃^�C�����擾
				m_startTime = GetNowCount() / 100;
				m_timeFlag = true;
			}
		}
		//�v���C���[���S�[�������u�ԃ^�C�������Z�b�g����Ă���
		if (m_timeFlag == true)
		{
			m_timeplayer = GetNowCount() / 100- m_startTime;
			Time::SetTime(m_timeplayer);			//�^�C���̒l���Z�b�g����
		}

		// ���\���̔���
		if (m_player->GetPlayerState() == COMPE_SWIM)
		{
			if (m_countryTopX < SCREEN_SIZE_W/2 && m_countryUnderX > SCREEN_SIZE_W / 2)
			{

				m_countryTopX += 30;
				m_countryUnderX -= 30;

			}
		}

		// �V�[���J��
		if (m_npc->GetRankCount() >= 6 && m_player->GetGoalFlag())
		{
			m_MaxGorl = true;
		}
		if (m_MaxGorl == true)
		{
			m_fadeOutFlag = true;
		}
		if (m_fadeOutFinishFlag)
		{
			// �����������t���O�����ɖ߂�
			isProcess = false;
			return new Result(m_playerRanking);				//	���U���g�V�[���ɐ؂�ւ���
		}
		//���U���g��ʂ������������p
		if (m_checkKeyFlag)
		{
			return new Result(m_playerRanking);				//	���U���g�V�[���ɐ؂�ւ���

		}

		//�X�y�[�X�L�[���������ԉ摜�\�����ς��
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			m_spacePushFlag = true;

		}

	//}
	//	break;
	//default:
	//	break;
	//}
	return this;						//	�Q�[���V�[����\����������
}

void GameSceneCompe::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// �t�F�[�h�C������
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			// �O���t�B�b�N��`��
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}


	//�v�[���̕\���ʒu�ύX
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));
	//�v�[���̕`��
	MV1DrawModel(m_poolModelHandle);

	if (m_player->GetPlayerState() == COMPE_SWIM)
	{
		// �^�C���\��
		DrawGraph(0, 0, m_timeFlame,TRUE);
		DrawExtendFormatString(1920 -250, 1080 - 100, 4.0, 4.0, GetColor(255,255,255), "%d.%d", m_timeplayer/10, m_timeplayer%10);

		//���̉摜���k��
		DrawExtendGraph(m_countryTopX, 0, m_countryUnderX, SCREEN_SIZE_H, m_countryGraphHandle, TRUE);
	}
	else if (m_player->GetPlayerState() == COMPE_DIVE)
	{
		//���̉摜�\��
		DrawGraph(0, 0, m_countryGraphHandle, TRUE);
		if (!m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spaceDiveGraph, TRUE);
		}
		else if (m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spaceDivePushGraph, TRUE);

		}

	
	}	


	// �v���[���[
	m_player->Draw();
	//npc
	m_npc->Draw();

	//50m�n�_�̉摜�\��
	if (m_turnFlag == true)
	{
		DrawGraph(0, 0, m_turnGraphHandle, TRUE);

	}

	if (m_player->GetGoalFlag())
	{
		DrawGraph(0, 0, m_goalGraphHandle, TRUE);

	}

	// �t�F�[�h�A�E�g����
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(255 - i, 255 - i, 255 - i);
			// �O���t�B�b�N��`��
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}


	if (m_MaxGorl == false)
	{
		DrawGraph(0, 0, m_liveGraph, TRUE);

	}

}

void GameSceneCompe::Sound()
{
	//	�{��BGM
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 150, m_soundHandle);	
}

void GameSceneCompe::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_countryGraphHandle = LoadGraph("data/img/compe/country.png");
	m_poolModelHandle = MV1LoadModel("data/model/stage/stage2/poolModel2.pmx");
	m_turnGraphHandle= LoadGraph("data/img/compe/50m.png");
	m_goalGraphHandle = LoadGraph("data/img/compe/100m.png");
	m_spaceDiveGraph = LoadGraph("data/img/gameScene/Dive.png");
	m_spaceDivePushGraph = LoadGraph("data/img/gameScene/DivePush.png");
	m_liveGraph=LoadGraph("data/img/compe/sekai.png");

	//	�T�E���h�n���h���ɃZ�b�g
	m_soundHandle = LoadSoundMem("data/sound/Game/honban.mp3");
	m_timeFlame= LoadGraph("data/img/compe/TimeFlame.png");

	m_player = new Player;			//	�v���C���[�N���X�̃C���X�^���X�𐶐�
	m_camera = new Camera;			//	�J�����N���X�̃C���X�^���X�𐶐�
	m_npc	 = new NPC;				//	NPC�N���X�̃C���X�^���X�𐶐�

}

void GameSceneCompe::DebugKey()
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
		m_checkKeyFlag = TRUE;
	}
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)	//	�G���^�[�������ꂽ��
	{
		m_finishFlag = TRUE;
	}
}
