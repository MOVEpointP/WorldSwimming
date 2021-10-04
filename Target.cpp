#include "Target.h"
#include "TestSceneKoga.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>
#include "Effect.h"

// �ÓI�萔.
const int Target::m_target_X      =		400;		// �^�[�Q�b�g�̏���x���W
const int Target::m_target_Y      =		 10;		// �^�[�Q�b�g�̏���y���W
const int Target::m_target_Z      =		  0;		// �^�[�Q�b�g�̏���z���W
								  
const int Target::m_score_good    =	      1;		// �X�R�A���ς��Ȃ�
const int Target::m_score_bad     =		 -5;		// �X�R�A��������
const int Target::m_score_perfect =	      5;		// �X�R�A���オ��
								  
const int Target::m_before_good   =    1200;		// �O�b�h����i�O�j
const int Target::m_perfect       =	   1325;      	// �p�[�t�F�N�g����
const int Target::m_after_good    =    1375;		// �O�b�h����i��j
const int Target::m_final_good    =    1500;		// �O�b�h����i�Ō�j

int Target::m_targetSpeed = 0;

//-----------------------------------------------------------------------------
// @brief  �R���X�g���N�^.
//-----------------------------------------------------------------------------
Target::Target()
	: m_targetCount(0)
	, m_plusX(0)
	, m_targetState(NO_SHOT)
	, m_legImgHandle(-1)
	, m_o2ImgHandle(-1)
	, m_posX(0)
	, m_targerJadgeWord(0)
	, m_combo(0)
	, m_targetScore(0)
	, hitRadius(7.5f)
	,m_goodHandle(-1)
	, m_badHandle(-1)
	, m_perfectHandle(-1)
	,m_tens_place(0)
	, m_countTime(0)
	, m_startTime(0)
	, efkHandle(-1)
	,efkFlag(false)
{
	// �摜�̓ǂݍ���
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");
	m_goodHandle = LoadGraph("data/img/gameScene/good.png");
	m_badHandle = LoadGraph("data/img/gameScene/bad.png");
	m_perfectHandle = LoadGraph("data/img/gameScene/perfect.png");
	m_comboHandle = LoadGraph("data/img/gameScene/combo.png");
	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapchipHandle);
	m_goodSoundHandle = LoadSoundMem("data/sound/Game/good.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_badSoundHandle = LoadSoundMem("data/sound/Game/bad.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_perfectSoundHandle = LoadSoundMem("data/sound/Game/perfect.mp3");		//�X�R�A�̌��ʉ��n���h��


	m_target_accel = 0.1f;

	// pos��Vector�^�Ȃ̂ŁAVGet�Ō��_�ɃZ�b�g
	pos = VGet(m_target_X, m_target_Y, m_target_Z);

	// �ړ�����͂��i���ׂĂ̍��W�j�[���ɂ���
	velocity = VGet(0, 0, 0);
	dir = VGet(1, 0, 0);

	// �G�t�F�N�g�ǂݍ���
	m_goodOrbitEfk = LoadEffekseerEffect("data/effects/swim/good.efk",25.0f);
	m_perfecOrbitEfk= LoadEffekseerEffect("data/effects/swim/perfecd.efk", 25.0f);
	m_badOrbitEfk = LoadEffekseerEffect("data/effects/swim/bad.efk", 25.0f);

}

//-----------------------------------------------------------------------------
// @brief  �f�X�g���N�^.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// �e��n���h���̃A�����[�h.
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);

	DeleteSoundMem(m_goodSoundHandle);
	DeleteSoundMem(m_badSoundHandle);
	DeleteSoundMem(m_perfectSoundHandle);

}

//-----------------------------------------------------------------------------
// @brief  �X�V.
//-----------------------------------------------------------------------------
void Target::Update(float _deltaTime)
{
	
	 accelVec = VGet(0, 0, 0);

	//	�ˏo�t���O��true�ɂȂ�����
	if (m_targetState == NOW_SHOT)
	{
		m_target_accel += m_targetSpeed;			
		accelVec = VScale(dir, m_target_accel);		
	}
	
	// �x���V�e�B�����v�Z.
	velocity = VAdd(velocity, accelVec);

	// �㉺�����ɂ����Ȃ��悤�Ƀx���V�e�B�𐮂���.
	velocity = VGet(velocity.x * _deltaTime, 0, velocity.z * _deltaTime);

	// �|�W�V�������X�V.
	pos = VAdd(pos, velocity);

	// �͂������I������x���V�e�B�̕����Ƀf�B���N�V�����𒲐�.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}

	// ���f���Ɍ����Ăق��������ɉ�].
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	//�����`�悷��t���O��rue�ɂȂ����玞�Ԃ��v��
	if (m_drawTargetFlag)
	{
		m_countTime = GetNowCount() / 1000 - m_startTime;
		efkFlag = false;
	}
	if (m_countTime  > 1)
	{
		m_drawTargetFlag = false;
	}

}

//-----------------------------------------------------------------------------
// @brief  �`��.
//-----------------------------------------------------------------------------
void Target::Draw()
{


	// ���̃A�C�R����`��
	DrawGraph(m_posX, 400, m_legImgHandle, TRUE);

	// �f�o�b�O�����蔻��.
	if (m_targetState == END_SHOT)
	{
		//����\�L
		if (m_targerJadgeWord == 2)		//good�Ȃ�
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0&&efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_goodOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);
			}

			//�����`�悷�鎞�Ԃ���b�o�����������悤�ɂ���
			if (m_drawTargetFlag)
			{

				DrawGraph(0, 0, m_goodHandle, TRUE);

			}
		}																												  
		else if (m_targerJadgeWord == 3)//perfect�Ȃ�																	
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0 && efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_perfecOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);

			}

			//�����`�悷�鎞�Ԃ���b�o�����������悤�ɂ���
			if (m_drawTargetFlag)
			{
				DrawGraph(0, 0, m_perfectHandle, TRUE);
			}
		}
		else if (m_targerJadgeWord == 1)//bad�Ȃ�
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0 && efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_badOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);

			}

			//�����`�悷�鎞�Ԃ���b�o�����������悤�ɂ���
			if (m_drawTargetFlag)
			{
				DrawGraph(0, 0, m_badHandle, TRUE);
			}
		}

	}
	//�@�P�R���{�ȏ�̎��ɃR���{����\������
	auto combo = Combo::GetCombo();
	DrawGraph(1920 / 2 + 840, 500 + 50, m_mapchipHandle[Combo::GetCombo()], TRUE);
	DrawGraph(1920 / 2 + 800, 500 + 50, m_mapchipHandle[Combo::GetTenCombo()], TRUE);
	DrawGraph(0, -20, m_comboHandle, TRUE);
}

//-----------------------------------------------------------------------------
// @brief  ���A�N�V����.
//-----------------------------------------------------------------------------
void Target::Reaction(Target* _target, bool _hitFlag)
{

	if (_hitFlag)
	{
		m_plusX = (20 + m_targetCount * 20);	// �����W���Z�b�g
		pos = VGet(m_plusX, 100, -200);			// ���W���Z�b�g

		m_targetState = END_SHOT;
	}
	else
	{

		//�V���b�g�̂Ƃ���
		if (m_targetState == NOW_SHOT && pos.x >= 1100)//�A�C�R�����߂��ɂ��邩�ǂ����T��
		{
			//�X�R�A���o���Ă���
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{
					PlaySoundMem(m_goodSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_good;					// �X�R�A�ω��Ȃ�
					m_targerJadgeWord = 2;
					pos = VGet(-2000, -1000, 200);					// ���W���ړ����ĕ\�����Ȃ�����
					m_combo = 1;									// �R���{�����Z
					m_targetState = END_SHOT;
					//�����`�悷��t���O��true�ɂ���
					//����̕`�������ŏ��̏u�Ԃ̎��Ԃ��v��
					m_drawTargetFlag = true;
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)  // perfect
				{
					PlaySoundMem(m_perfectSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_perfect;                // �X�R�Aup

					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);                   // ���W���ړ����ĕ\�����Ȃ�����
					m_combo = 1;                                     // �R���{�����Z
					m_targetState = END_SHOT;
					//�����`�悷��t���O��true�ɂ���
					//����̕`�������ŏ��̏u�Ԃ̎��Ԃ��v��
					m_drawTargetFlag = true;
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;


				}
				else												 // bad�i����ȊO�Ȃ�j
				{
					PlaySoundMem(m_badSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_bad;					 // �X�R�Adown

					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);					 // ���W���ړ����ĕ\�����Ȃ�����
					m_combo = -Combo::GetCombo();									 // �R���{�����Z�b�g
					m_tens_place = -Combo::GetTenCombo();
					m_targetState = END_SHOT;
					//�����`�悷��t���O��true�ɂ���
					//����̕`�������ŏ��̏u�Ԃ̎��Ԃ��v��
					m_drawTargetFlag = true;
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;


				}


			}
			else if (pos.x > m_final_good)
			{
				PlaySoundMem(m_badSoundHandle, DX_PLAYTYPE_BACK);

				m_targetScore += m_score_bad;	// �X�R�Adown

				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);	// ���W���ړ����ĕ\�����Ȃ�����
				m_combo = -Combo::GetCombo();					// �R���{�����Z�b�g
				m_tens_place = -Combo::GetTenCombo();
				m_targetState = END_SHOT;

				//�����`�悷��t���O��true�ɂ���
				//����̕`�������ŏ��̏u�Ԃ̎��Ԃ��v��
				m_drawTargetFlag = true;
				m_startTime = GetNowCount() / 1000;

				efkFlag = true;

			}

			if (Combo::GetCombo() >= 9)
			{
 				m_combo = -Combo::GetCombo();			 // �R���{�����Z�b�g
				m_tens_place = 1;
			}


			Score::AddScore(m_targetScore);		//�X�R�A�̒l�����Z����
			Combo::AddCombo(m_combo);			//�R���{�̒l�����Z����
			Combo::AddTenCombo(m_tens_place);	//�R���{�̏\�̈ʂ̒l�����Z����
			m_combo = 0;						//�R���{�̒l�����Z�b�g
			m_tens_place = 0;					//�R���{�̏\�̈ʃ��Z�b�g
			m_targetScore = 0;
		}

		//�X�R�A��0��艺�ɍs���Ȃ��悤�ɂ���
		if (Score::GetScore() <= 0)
		{
			m_targetScore = -Score::GetScore();
		}
	}
}



