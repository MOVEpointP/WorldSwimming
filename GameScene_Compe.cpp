#include "GameScene_Compe.h"	
#include "Result.h"
#include "Player.h"
#include "ObstructManager.h"
#include "Hitchecker.h"
#include "UI.h"
#include "Camera.h"
#include "NPC.h"
#include "DxLib.h"
#include "Effect.h"
static int enemyNum = 10;					//	�G�l�~�[�̐�
static int COUNTDOWN = 7;					//	�J�E���g�_�E���̕b���i+2�j
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
GameSceneCompe::GameSceneCompe()
	: m_player(nullptr)
	, m_camera(nullptr)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_npc(nullptr)
	, m_MaxGorl(false)
	, m_playerRanking(0)
	, m_liveY(0)
	, m_countryDraw(false)
{
	// �J�n���̃^�C�����擾
	//m_startTime = GetNowCount() / 1000;
	// �X�e�[�g�Z�b�g(�J�E���g�_�E������)
	//m_state = GAME_SCENE_STATE::COUNTDOWN;
	KeyPush = false;
}

GameSceneCompe::~GameSceneCompe()
{
	delete m_player;	//	�v���C���[�̃|�C���^�����o�ϐ�������
	delete m_camera;	//	�J�����̃|�C���^�����o�ϐ�������
	delete m_npc;       //  NPC�̃|�C���^�����o�ϐ�������
	//	�������̉������
	StopSoundMem(m_countryGraphHandle);
	DeleteSoundMem(m_soundHandle);
}

SceneBase* GameSceneCompe::Update(float _deltaTime)
{
	// �f�o�b�O�r���h�̂݃f�o�b�O�֐����Ăяo��
#ifdef _DEBUG
	DebugKey();
#endif
	m_player->SetScene(true);

	//switch (m_state)
	//{
	//case GAME_SCENE_STATE::COUNTDOWN:
	//	if ((COUNTDOWN + 1) - (GetNowCount() / 1000 - m_startTime) <= 1)
	//	{
	//		m_startTime = GetNowCount() / 5000;
	//		m_state = GAME_SCENE_STATE::GAME;
	//	}
	//	break;
	//case GAME_SCENE_STATE::GAME:
	//{
		//�v���C���[���j���n�߂��獑�Ƃ̕\��
		if (m_player->GetPlayerState() == SWIM)
		{
			m_countryDraw = true;
		}
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);
		m_npc->Update(_deltaTime);
		// �v���C���[�̃����L���O�ۑ��ϐ���NPC�����̃S�[���������̐�������
		// NPC�����̃S�[���������̐������闝�R
		// �v���C���[�̃����L���O�����߂�ۂ�NPC�����̒��������ɂ���ă����L���O�����߂邽��
		static bool isProcess = false;
		if (m_player->GetGoalFlag() && !isProcess)
		{
			m_npc->addRankCount();
			// ���Z����RankCount���擾���ă��U���g�ɏ��ʂƂ��ĕ\��
			m_playerRanking = m_npc->GetRankCount();
			isProcess = true;
		}
		// �V�[���J��
		//�v���C���[��NPC���S���S�[�������烊�U���g�V�[���Ɉڍs
		if (m_npc->GetRankCount() >= 4)
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
			//	���U���g�V�[���ɐ؂�ւ���
			return new Result(m_playerRanking);
		}
	//}
	//	break;

	//default:
	//	break;
	//}

	//	�Q�[���V�[����\����������
	return this;
}

void GameSceneCompe::Draw()
{
	//�v�[���̕\���ʒu�ύX
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));
	//�v�[���̕`��
	MV1DrawModel(m_poolModelHandle);

	m_player->Draw();
	m_npc->Draw();

	//LIVE�̕�����\��
	DrawExtendFormatString(50, m_liveY, 4.0, 4.0, GetColor(255, 0, 0), "LIVE");
	//���̉摜��\��
	if (m_countryDraw == true)
	{
		DrawGraph(0, 0, m_countryGraphHandle, TRUE);
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
	//	�T�E���h�n���h���ɃZ�b�g
	m_soundHandle = LoadSoundMem("data/sound/Game/honban.mp3");

	m_player = new Player;			//	�v���C���[�N���X�̃C���X�^���X�𐶐�
	m_camera = new Camera;			//	�J�����N���X�̃C���X�^���X�𐶐�
	m_npc = new NPC;				//	�m�o�b�N���X�̃C���X�^���X�𐶐�
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
}