#include "Target.h"
#include "TestSceneKoga.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>

// �ÓI�萔.
const int Target::m_target_X      =		400;		// �^�[�Q�b�g�̏���x���W
const int Target::m_target_Y      =		 10;		// �^�[�Q�b�g�̏���y���W
const int Target::m_target_Z      =		  0;		// �^�[�Q�b�g�̏���z���W
								  
const int Target::m_score_good    =	      2;		// �X�R�A���ς��Ȃ�
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
	, TimeCount(0)
	, m_combo(0)
	, m_targetScore(0)
	, hitRadius(7.5f)

{
	// �摜�̓ǂݍ���
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");

	m_target_accel = 0.1f;

	// pos��Vector�^�Ȃ̂ŁAVGet�Ō��_�ɃZ�b�g
	pos = VGet(m_target_X, m_target_Y, m_target_Z);

	// �ړ�����͂��i���ׂĂ̍��W�j�[���ɂ���
	velocity = VGet(0, 0, 0);
	dir = VGet(1, 0, 0);

}

//-----------------------------------------------------------------------------
// @brief  �f�X�g���N�^.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// �e��n���h���̃A�����[�h.
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);
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

}

//-----------------------------------------------------------------------------
// @brief  �`��.
//-----------------------------------------------------------------------------
void Target::Draw()
{
	// ���̃A�C�R����`��
	DrawGraph(m_posX, 320, m_legImgHandle, TRUE);
	
	// �f�o�b�O�����蔻��.
	if (m_targetState == END_SHOT)
	{
		//����\�L
		if (m_targerJadgeWord == 2)		//good�Ȃ�
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 0), "good!");			//�@���茋�ʕ\�L
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 0, 0), "+1");			//�@���茋�ʕ\�L
		}																												  
		else if (m_targerJadgeWord == 3)//perfect�Ȃ�																	
		{																												  
																														  
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(255, 0, 0), "perfect!!");	//�@���茋�ʕ\�L
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(255, 100, 0), "+5");		//�@���茋�ʕ\�L

		}
		else if (m_targerJadgeWord == 1)//bad�Ȃ�
		{

 			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 255), "bad...");		//�@���茋�ʕ\�L
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 100, 255), "-5");		//�@���茋�ʕ\�L
		}

	}
		//�@���茋�ʂ�\������Ԃ̃J�E���g
		TimeCount++;

		if (TimeCount >= 250)
		{
			m_targerJadgeWord = 0;
			TimeCount = 0;
		}
	
		//�@�P�R���{�ȏ�̎��ɃR���{����\������
		if (Combo::GetCombo())
		{
			DrawExtendFormatString(1920 - 700 - GetFontSize(), 300, 4.0, 10.0, GetColor(100, 0, 0), "%d�R���{�I", Combo::GetCombo());	//�@���茋�ʕ\�L
		}																																  
																																		  
		DrawExtendFormatString(0, 100, 4.0, 5.0, GetColor(0,100, 0), "�X�R�A�F%d", Score::GetScore());									//�@���茋�ʕ\�L
																																		  
		DrawExtendFormatString(100, 600, 4.0, 5.0, GetColor(0, 100, 0), "Shift�Ŕ��ˁI", Score::GetScore());							//�@���茋�ʕ\�L


}

//-----------------------------------------------------------------------------
// @brief  ���A�N�V����.
//-----------------------------------------------------------------------------
void Target::Reaction(Target* _target, bool _hitFlag)
{

	switch (_hitFlag)
	{
	case true:

		m_plusX = (20 + m_targetCount * 20);	// �����W���Z�b�g
		pos = VGet(m_plusX, 100, -200);			// ���W���Z�b�g

		m_targetState = END_SHOT;

		break;

	case false:

		//�V���b�g�̂Ƃ���
		if (m_targetState == NOW_SHOT && pos.x >= 1100)//�A�C�R�����߂��ɂ��邩�ǂ����T��
		{
			//�X�R�A���o���Ă���
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{

					m_targetScore += m_score_good;					// �X�R�A�ω��Ȃ�
					m_targerJadgeWord = 2;
					pos = VGet(-2000, -1000, 200);					// ���W���ړ����ĕ\�����Ȃ�����
					m_combo++;										// �R���{�����Z
					m_targetState = END_SHOT;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)  // perfect
				{													    
																	    
					m_targetScore += m_score_perfect;                // �X�R�Aup
					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);                   // ���W���ړ����ĕ\�����Ȃ�����
					m_combo++;                                       // �R���{�����Z
					m_targetState = END_SHOT;

				}
				else												 // bad�i����ȊO�Ȃ�j
				{													    
					m_targetScore += m_score_bad;					 // �X�R�Adown
					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);					 // ���W���ړ����ĕ\�����Ȃ�����
					m_combo = -Combo::GetCombo();					 // �R���{�����Z�b�g
					m_targetState = END_SHOT;
				}
			}
			else if (pos.x > m_final_good)
			{

				m_targetScore += m_score_bad;	// �X�R�Adown
				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);	// ���W���ړ����ĕ\�����Ȃ�����
				m_combo = -Combo::GetCombo();	// �R���{�����Z�b�g
				m_targetState = END_SHOT;

			}

			Score::SetScore(m_targetScore);		//�X�R�A�̒l���Z�b�g����
			Combo::SetCombo(m_combo);			//�R���{�̒l���Z�b�g����

		}
		break;
	}
}



