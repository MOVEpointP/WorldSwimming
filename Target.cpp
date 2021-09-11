#include "Target.h"
#include "ObstructBase.h"
#include "TestSceneKoga.h"
#include "Hitchecker.h"
#include "Effect.h"
#include "UI.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>

// �ÓI�萔.
const int Target::m_target_X = 400;
//const int Target::m_target_X = 0;
const int Target::m_target_Y = 10;
const int Target::m_target_Z = 0;

const int Target::m_font_X = 1340;
const int Target::m_font_Y = 160;
const int Target::m_font_size = 50;
const int Target::m_font_thick = -1;

const int Target::m_score_good = 0;			//�X�R�A���ς��Ȃ�
const int Target::m_score_bad = -10;		//�X�R�A��������
const int Target::m_score_perfect = 10;		//�X�R�A���オ��

const int Target::m_before_good = 1200;		//	�O�b�h����i�O�j
const int Target::m_perfect = 1325;	//�@�p�[�t�F�N�g����
const int Target::m_after_good = 1375;		//�@�O�b�h����i��j
const int Target::m_final_good = 1500;		//�@�O�b�h����i�Ō�j



static const double pi = 3.141592653589793;

// ����
const int VOLUME_PAL = 100;

int Target::m_targetSpeed = 0;

//-----------------------------------------------------------------------------
// @brief  �R���X�g���N�^.
//-----------------------------------------------------------------------------
Target::Target()
	: modelHandle(-1)
	, hitRadius(7.5f)
	, timenow(0)
	, m_targetCount(0)
	, m_plusX(0)
	, m_setTime(0) 
	, m_shotInterval(5)
	, m_iceState(NO_SHOT)
	//, m_hitFlag(false)
	, m_throwIceSoundHandle(-1)
	//, m_hitIceSoundHandle(-1)
	, m_iceType(0)
	//, m_handImgHandle(-1)
	, m_legImgHandle(-1)
	, m_o2ImgHandle(-1)
	, m_posX(0)
	, m_targetJadge(0)
	, m_targerJadgeWord(0)
	, TimeCount(0)
	, m_combo(0)
	, m_targetScore(0)
{
	// �摜�̓ǂݍ���
	//m_handImgHandle= LoadGraph("data/img/target/hand.png");
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");
	//modelHandle = MV1LoadModel("data/model/target/icecream/SVH-icecream/icecream.pmx");
	m_FontHandle = CreateFontToHandle(NULL, m_font_size, m_font_thick, DX_FONTTYPE_NORMAL);

	m_target_accel = 0.1f;

	// pos��Vector�^�Ȃ̂ŁAVGet�Ō��_�ɃZ�b�g
	pos = VGet(m_target_X, m_target_Y, m_target_Z);
	// �ړ�����͂��i���ׂĂ̍��W�j�[���ɂ���
	velocity = VGet(0, 0, 0);
	// 
	dir = VGet(1, 0, 0);

}

//-----------------------------------------------------------------------------
// @brief  �f�X�g���N�^.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// �e��n���h���̃A�����[�h.
	//MV1DeleteModel(modelHandle);
	//DeleteGraph(m_handImgHandle);
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);
}

//-----------------------------------------------------------------------------
// @brief  �X�V.
//-----------------------------------------------------------------------------
void Target::Update(float _deltaTime)
{
	
	 accelVec = VGet(0, 0, 0);
	//	�A�C�X�ˏo�t���O��true�ɂȂ�����
	if (m_iceState == NOW_SHOT)
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

	//switch (m_iceType)
	//{
	//case 0:
	//	break;
	//case 1:
	//	pos.z = cos(pos.x / 20.0f) * (pos.x / 5);
	//	break;
	//case 2:
	//	pos.y = sin(pos.x / 20.0f) * (pos.x / 5);
	//	pos.z = cos(pos.x / 20.0f) * (pos.x / 5);
	//	break;
	//}
	//pos.y = sin(pos.x / 20.0f) * (pos.x / 5);
	//pos.z = cos(pos.x / 20.0f) * (pos.x / 5);

	// �͂������I������x���V�e�B�̕����Ƀf�B���N�V�����𒲐�.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}
	//printfDx("%f %f %f\n", dir.x, dir.y, dir.z);

	// �RD���f���̃|�W�V�����ݒ�
	//MV1SetPosition(modelHandle, pos);

	//// �����ɍ��킹�ĉ�].
	//MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

	// ���f���Ɍ����Ăق��������ɉ�].
	//MATRIX tmpMat = MV1GetMatrix(modelHandle);
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	//tmpMat = MMult(tmpMat, rotYMat);
	//MV1SetRotationMatrix(modelHandle, tmpMat);
}

//-----------------------------------------------------------------------------
// @brief  �`��.
//-----------------------------------------------------------------------------
void Target::Draw()
{
	DrawGraph(m_posX, 320, m_legImgHandle, TRUE);
	
	// �����f���ł͏���������̂ŕ`��{����ݒ�
	//MV1SetScale(modelHandle, VGet(3.0f, 3.0f, 3.0f));
	if (m_iceState != NO_SHOT)
	{
		// �R�c���f���̕`��
		//MV1DrawModel(modelHandle);
	}
	
	if (m_iceState == NO_SHOT)
	{
		//int timebuffer = GetNowCount() / 1000;
		
		//DrawCircleAA(m_font_X, m_font_Y,((m_shotInterval + 1) - (timebuffer - m_setTime))*30
		//	,32, GetColor(255, 255, 255),FALSE);
	}

	// �f�o�b�O�����蔻��.
	//DrawSphere3D(pos, hitRadius, 5, 0x00ffff, 0x00ffff, false);
	if (m_iceState == END_SHOT)
	{
		//����\�L
		if (m_targerJadgeWord == 2)//good�Ȃ�
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 0), "good!");//���茋�ʕ\�L

		}
		else if (m_targerJadgeWord == 3)//perfect�Ȃ�
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(255, 0, 0), "perfect!!");//���茋�ʕ\�L
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(255, 100, 0), "+10");//���茋�ʕ\�L

		}
		else if (m_targerJadgeWord == 1)//bad�Ȃ�
		{

 			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 255), "bad...");	//���茋�ʕ\�L
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 100, 255), "-10");	//���茋�ʕ\�L
		}

	}
		//���Ԃ��o�ƕ�����������(getnowcount��肭�g���Ȃ���������t���[�������邱�Ƃɂ����j
		TimeCount++;

		if (TimeCount >= 250)
		{
			m_targerJadgeWord = 0;
			TimeCount = 0;
		}
	
		//�P�R���{�ȏ�̎��ɕ\������(���s���Ă�j
		if (Combo::GetCombo())
		{
			DrawExtendFormatString(1920 - 700 - GetFontSize(), 300, 4.0, 10.0, GetColor(100, 0, 0), "%d�R���{�I", Combo::GetCombo());	//���茋�ʕ\�L
		}

		DrawExtendFormatString(0, 100, 4.0, 5.0, GetColor(0,100, 0), "�X�R�A�F%d", Score::GetScore());	//���茋�ʕ\�L

		DrawExtendFormatString(100, 600, 4.0, 5.0, GetColor(0, 100, 0), "Shift�Ŕ��ˁI", Score::GetScore());	//���茋�ʕ\�L


}

//-----------------------------------------------------------------------------
// @brief  ���A�N�V����.
//-----------------------------------------------------------------------------
void Target::Reaction(UI* _ui, bool _hitFlag)
{
	switch (_hitFlag)
	{
	case true:
		m_plusX = (20 + m_targetCount * 20);//�����W���Z�b�g
		pos = VGet(m_plusX, 100, -200);
		//m_hitFlag = true;
		
		//ScoreUpdateUI(*_ui, _hitFlag);

		//PlaySoundMem(m_hitIceSoundHandle, DX_PLAYTYPE_BACK);
		//ChangeVolumeSoundMem(VOLUME_PAL, m_hitIceSoundHandle);

		// �RD���f���̃|�W�V�����ݒ�
		//MV1SetPosition(modelHandle, pos);
		m_iceState = END_SHOT;
		break;
	case false:
		//�V���b�g�̂Ƃ���
		if (m_iceState == NOW_SHOT&& pos.x>=1100)//�A�C�R�����߂��ɂ��邩�ǂ����T��
		{
			//�X�R�A���o���Ă���
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{
					m_targetScore += m_score_good;//�X�R�A�ω��Ȃ�
					m_targerJadgeWord = 2;
 					pos = VGet(-2000, -1000, 200);// ���W���ړ����ĕ\�����Ȃ�����
					m_combo++;//�R���{�����Z
					m_iceState = END_SHOT;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)//perfect
				{
					m_targetScore += m_score_perfect;//�X�R�Aup
					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);// ���W���ړ����ĕ\�����Ȃ�����
					m_combo++;//�R���{�����Z
					m_iceState = END_SHOT;

				}
				else //bad�i����ȊO�Ȃ�j
				{
					m_targetScore += m_score_bad;//�X�R�Adown
					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);// ���W���ړ����ĕ\�����Ȃ�����
					m_combo = -Combo::GetCombo();//�R���{�����Z�b�g
					m_iceState = END_SHOT;
				}
			}
			else if (pos.x > m_final_good)
			{
				m_targetScore += m_score_bad;//�X�R�Adown
				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);// ���W���ړ����ĕ\�����Ȃ�����
				m_combo = -Combo::GetCombo();//�R���{�����Z�b�g
				m_iceState = END_SHOT;
			}

			Score::SetScore(m_targetScore);//�X�R�A�̒l���Z�b�g����
			Combo::SetCombo(m_combo);//�R���{�̒l���Z�b�g����

			//ScoreUpdateUI(*_ui, _hitFlag);

			//PlaySoundMem(m_missIceSoundHandle, DX_PLAYTYPE_BACK);
			//ChangeVolumeSoundMem(VOLUME_PAL, m_missIceSoundHandle);

			// �RD���f���̃|�W�V�����ݒ�
			//MV1SetPosition(modelHandle, pos);

		}
		break;

	}

	
}

//void Target::ScoreUpdateUI(UI& _ui,bool _hitFlag)
//{
//	_ui.ScoreUpdate(_hitFlag);
//}

//-----------------------------------------------------------------------------
// @brief  �Փˏ���.
//-----------------------------------------------------------------------------
//void Target::OnHitObstruct(ObstructBase& obstruct)
//{
//	// �������g�̈ʒu����Q���̂����蔻�蕪���炷.
//	// Z����X���̓񎟌����W�Ƃ��Ĉʒu���炵���s��.
//	VECTOR yZeroTarget = VGet(pos.x, 0, pos.z);
//	VECTOR yZeroObstruct = VGet(obstruct.GetPos().x, 0, obstruct.GetPos().z);
//
//	VECTOR obsToTarget = VSub(yZeroTarget, yZeroObstruct);
//
//	// �Ԃ������Ƃ��ɗ��������B���傤�ǂ���float�덷�ŏd�Ȃ葱����̂ŏ����Ԃ�������.
//	float awayRange = (hitRadius + obstruct.GetHitRadius() + 0.01f);
//
//	VECTOR awayVec = VScale(VNorm(obsToTarget), awayRange);
//	pos = VAdd(yZeroObstruct, awayVec);
//
//	// �Ԃ������猸������.
//	velocity = VScale(velocity, COLIDE_DECEL_FAC);
//}