
#include "GameScene_easy.h"
#include "ResultHalf.h"
#include "Target.h"
#include "Player.h"
#include "ObstructManager.h"
#include "Hitchecker.h"
#include "UI.h"
#include "Camera.h"

#include "DxLib.h"
#include "Effect.h"

//	�G�l�~�[�̐�
static int enemyNum = 19;					
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

GameSceneEasy::GameSceneEasy()
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
{
	// ���̃V�[���ֈڍs���邩�ǂ���
	m_finishFlag = FALSE;

	// �G�l�~�[�E�X�R�AUI������
	for (int i = 0; i < enemyNum; i++)
	{
		m_target[i] = nullptr;
	}
	m_target[enemyNum] = nullptr;


	// �J�n���̃^�C�����擾
	m_startTime = GetNowCount() / 1000;

	// �X�e�[�g�Z�b�g(�J�E���g�_�E������)
	m_state = GAME_SCENE_STATE::COUNTDOWN;

	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");

	srand(time(NULL));  //�����̎��������
}

GameSceneEasy::~GameSceneEasy()
{
	delete m_player;	//	�v���C���[�̃|�C���^�����o�ϐ�������
	delete m_camera;	//	�J�����̃|�C���^�����o�ϐ�������

	//	�������̉������
	DeleteSoundMem(m_soundHandle);


	for (int i = 0; i < enemyNum; i++)
	{
		delete m_target[i];
	}


	delete m_target[enemyNum];

}

SceneBase* GameSceneEasy::Update(float _deltaTime)
{
	// �f�o�b�O�r���h�̂݃f�o�b�O�֐����Ăяo��
#ifdef _DEBUG

	DebugKey();

#endif

	m_targetSpeed = rand() % 50 + 1;//�^�[�Q�b�g�̑��x��ς���

	m_player->SetScene(false);

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
		if (CheckHitKey(KEY_INPUT_LSHIFT))
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

		// ���݂̔ԍ��ɉ����ăG�l�~�[�̍X�V
		m_target[m_targetCount]->Update(_deltaTime);
		m_target[m_targetCount]->SetTargetCount(m_targetCount);

		// �A�C�R����x���̃|�W�V�������擾
		m_target[m_targetCount]->SetSinglePosX();//�^�[�Q�b�g�ɂ����W���Z�b�g
		m_target[m_targetCount]->Reaction(m_target[m_targetCount], false);
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);



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


void GameSceneEasy::Draw()
{

	//�v�[���̕\���ʒu�ύX
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));

	//�v�[���̕`��
	MV1DrawModel(m_poolModelHandle);

	DrawGraph(0, 0, m_timingImgHandle, TRUE);

	// �^�[�Q�b�g(�A�C�X)
	for (int i = 0; i <= m_targetCount; i++)
	{
		m_target[i]->Draw();
	}


	// �v���[���[
	m_player->Draw();


	if (m_finishSoundFlag)
	{
		PlaySoundMem(m_endSoundHandle, DX_PLAYTYPE_BACK);

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

void GameSceneEasy::Sound()
{

	//���KBGM�̍Đ�
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_LOOP, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 50, m_soundHandle);

}

void GameSceneEasy::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");		//���KBGM�n���h��
	m_timingImgHandle = LoadGraph("data/img/gameScene/timing.png");		//����o�[�̉摜�n���h��

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


