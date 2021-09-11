//-----------------------------------------------------------------------------
// @brief  NPC�N���X.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "NPC.h"
#include "ObstructBase.h"
#include "Effect.h"

//�v���C���[�̂������ɕ`��A�A�b�v�f�[�g���s��
//�`��A�A�b�v�f�[�g�͍��܂łň�l����������Ă��Ȃ�����

// �ÓI�萔.
const float NPC::ACCEL = 15.0f;		// �ʏ�̉���.

//
int NPC::m_sHandle;

//const float Player::ACCEL = 0.03f;		// �ʏ�̉���.
const float NPC::MAX_SPEED = 0.8f;			// �ō����x.
const float NPC::TRANING_SPEED = 9.0f;	        // ���K���̔{���X�s�[�h.
const float NPC::DEFAULT_DECEL = -0.01f;		// �Ȃɂ����Ȃ����̌���.
const float NPC::BREAK_DECEL = -0.05f;		// �u���[�L���̌���.
const float NPC::GRIP_DECEL = -0.025f;		// �O���b�v�̌���.
const float NPC::GRIP_POWER = 0.1f;			// �O���b�v��.
const float NPC::COLIDE_DECEL_FAC = 0.4f;			// ��Q���ɂԂ������Ƃ��̌�����.

//	����
const int VOLUME_PAL = 100;

//-----------------------------------------------------------------------------
// @brief  �R���X�g���N�^.
//-----------------------------------------------------------------------------
NPC::NPC()
	:hitRadius(7.5f)
	, m_playerOrbitEfk(nullptr)
	, m_diveModelHandle(-1)
	, m_swimModelHandle(-1)
	, m_resultModelHandle(-1)
	, m_NPCState(0)
	, m_moveFlag(false)
	, m_speedDisplay(0)
	, m_moveAnimFlag(false)
	, m_moveCount(0)

{

	// �T�E���h�̓ǂݍ���
	m_sHandle = LoadSoundMem("data/sound/sara_shrow.wav");


	// �R�c���f���̓ǂݍ���
	//m_diveModelHandle = MV1LoadModel("data/model/player/dive.mv1");			//�_�C�u���[�V�����t�����f��
	//m_swimModelHandle = MV1LoadModel("data/model/player/Swimming01.mv1");	//�j�����[�V�����t�����f��
	//m_resultModelHandle = MV1LoadModel("data/model/player/result.mv1");		//���U���g�|�[�Y���[�V�����t�����f��

	for (int i = 0; i <= 2; i++)
	{
		m_modelHandle[i][0] = MV1LoadModel("data/model/npc/dive.mv1");
		m_modelHandle[i][1] = MV1LoadModel("data/model/npc/Swimming01.mv1");
		m_modelHandle[i][2] = MV1LoadModel("data/model/player/taisou.mv1");
		m_modelHandle[i][3] = MV1LoadModel("data/model/player/result.mv1");

		//�R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`���A
		//�A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		AttachIndex = MV1AttachAnim(m_modelHandle[i][0], 0, -1, FALSE);
		TotalTime[DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[i][0], AttachIndex);
		AttachIndex = MV1AttachAnim(m_modelHandle[i][1], 0, -1, FALSE);
		TotalTime[SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[i][1], AttachIndex);
		//AttachIndex = MV1AttachAnim(m_modelHandle[i][2], 0, -1, FALSE);
		//TotalTime[TURN] = MV1GetAttachAnimTotalTime(m_modelHandle[i][2], AttachIndex);
		//AttachIndex = MV1AttachAnim(m_modelHandle[i][3], 0, -1, FALSE);
		//TotalTime[RESULT] = MV1GetAttachAnimTotalTime(m_modelHandle[i][3], AttachIndex);


		dir[i] = VGet(0, 0, 1);
		GorlFlag[i] = false;

	}


	m_RankCount = 0;

	//�Đ����Ԃ̏�����
	PlayTime = 0.0f;

	// �G�t�F�N�g�ǂݍ���
	m_playerOrbitEfk = new PlayEffect("data/effects/PlayerLine.efk");
	m_efkDir = VGet(0.0f, DX_PI_F, 0.0f);
	m_playerOrbitEfk->SetPlayingEffectRotation(m_efkDir);

	// pos��Vector�^�Ȃ̂ŁAVGet�Ō��_�ɃZ�b�g
	//pos[0] = VGet(23, 30, 20);
	//pos[1] = VGet(50, 30, 43);
	//pos[2] = VGet(80, 30, 40);

	pos[0] = VGet(23, 30, 20);
	pos[1] = VGet(50, 30, 20);
	pos[2] = VGet(80, 30, 20);

	//pos = VGet(0, 13, 50);
	// �RD���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_modelHandle[0][m_NPCState], pos[0]);
	MV1SetPosition(m_modelHandle[1][m_NPCState], pos[1]);
	MV1SetPosition(m_modelHandle[2][m_NPCState], pos[2]);

	// �ړ�����͂��i���ׂĂ̍��W�j�[���ɂ���
	velocity[0] = VGet(0, 0, 0);
	velocity[1] = VGet(0, 0, 0);
	velocity[2] = VGet(0, 0, 0);

	// 
	// �L�[��������Ă��Ȃ���Ԃɂ���
	KeyPush = false;
}

//-----------------------------------------------------------------------------
// @brief  �f�X�g���N�^.
//-----------------------------------------------------------------------------
NPC::~NPC()
{

	for (int i = 0; i <= 2; i++)
	{
		MV1DeleteModel(m_modelHandle[i][0]);
		MV1DeleteModel(m_modelHandle[i][1]);
		MV1DeleteModel(m_modelHandle[i][2]);
		MV1DeleteModel(m_modelHandle[i][3]);
	}

	// ���f���̃A�����[�h.
	// �T�E���h�̃A�����[�h
	DeleteSoundMem(m_sHandle);
	// �G�t�F�N�g�̃A�����[�h
	m_playerOrbitEfk->Delete();
	delete m_playerOrbitEfk;
}

//-----------------------------------------------------------------------------
// @brief  �X�V.
//-----------------------------------------------------------------------------
void NPC::Update(float _deltaTime)
{
	// �L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ��������.
	VECTOR accelVec[3];

	accelVec[0] = VGet(0, 0, 0);
	accelVec[1] = VGet(0, 0, 0);
	accelVec[2] = VGet(0, 0, 0);


	// �L�[��������Ă��炸�A���L�[�������ꂽ��
	if (CheckHitKey(KEY_INPUT_SPACE) && !KeyPush)
	{
		PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(VOLUME_PAL, m_sHandle);
		KeyPush = true;
	}

	if (KeyPush)
	{
		if (m_NPCState == SWIM)
		{
			for (int i = 0; i <= 2; i++)
			{
				// z���W��320�𒴂����珊��̈ʒu�ɖ߂�
				if (VSize(pos[i]) > VSize(VGet(0, 0, 320.0f)))
				{
					dir[i] = VGet(0, 0, -1);
				}
			}

			//�@�v�[���E�[�ɂ�����S�[���������t���O��Ԃ��@NPC��̖�
			if (VSize(pos[0]) < VSize(VGet(0, 0, 50.0f)))
			{
				if (GorlFlag[0] == false)
				{
					GorlFlag[0] = true;
					m_RankCount++;
				}
			}

			//�@�v�[���E�[�ɂ�����S�[���������t���O��Ԃ��@NPC��̖�
			if (VSize(pos[1]) < VSize(VGet(0, 0, 57.0f)))
			{
				if (GorlFlag[1] == false)
				{
					GorlFlag[1] = true;
					m_RankCount++;
				}
			}

			//�@�v�[���E�[�ɂ�����S�[���������t���O��Ԃ��@NPC�O�̖�
			if (VSize(pos[2]) < VSize(VGet(0, 0, 85.0f)))
			{
				if (GorlFlag[2] == false)
				{
					GorlFlag[2] = true;
					m_RankCount++;
				}
			}


			//�{�Ԃ����K��
			//if (m_moveFlag == true)//�{�Ԃ�������
			//{
			for (int i = 0; i <= 2; i++)
			{
				if (GorlFlag[i] == false)
				{
					accelVec[i] = VScale(dir[i], ACCEL + (i + 2));

				}
			}
				
			//}
			//if (m_moveFlag == false)//���K��������
			//{
			//	accelVec = VScale(dir, TRANING_SPEED);
			//}
		}

		//���K�Ɩ{�ԂŃ��[�V�����̃X�s�[�h�𒲐�����
		 m_motionSpeed = 0.4f;
		//if (m_moveFlag == true)
		//{
		//	

		//}
		//else if (m_moveFlag == false)
		//{
		//	m_motionSpeed = 0.3f;
		//}
		// �Đ����Ԃ�i�߂�
		PlayTime += m_motionSpeed;

	}

	for (int i = 0; i <= 2; i++)
	{
		// �x���V�e�B�����v�Z.
		velocity[i] = VAdd(velocity[i], accelVec[i]);

	}


	for (int i = 0; i <= 2; i++)
	{
		// �㉺�����ɂ����Ȃ��悤�Ƀx���V�e�B�𐮂���
		velocity[i] = VGet(velocity[i].x * _deltaTime, 0, velocity[i].z * _deltaTime);

		// �|�W�V�������X�V.
		pos[i] = VAdd(pos[i], velocity[i]);

		if (KeyPush && m_NPCState == DIVE)
		{
			pos[i].y -= 0.1;
		}
		// �RD���f���̃|�W�V�����ݒ�
		MV1SetPosition(m_modelHandle[i][m_NPCState], pos[i]);


	}





	//// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�

	if (PlayTime >= TotalTime[m_NPCState])
	{
		for (int i = 0; i < 2; i++)
		{
			if (m_NPCState == DIVE)
			{
				m_NPCState = SWIM;
				pos[i].z = 50;

				PlayTime = 0.0f;

			}

		}
		if (m_NPCState == SWIM)
		{
			PlayTime = 0.0f;

		}


	}


	for (int i = 0; i <= 2; i++)
	{
		// �Đ����Ԃ��Z�b�g����
		MV1SetAttachAnimTime(m_modelHandle[i][m_NPCState], AttachIndex, PlayTime);

	}


}

//-----------------------------------------------------------------------------
// @brief  �`��.
//-----------------------------------------------------------------------------
void NPC::Draw()
{
	for (int i = 0; i <= 2; i++)
	{
		// 3D���f���̃X�P�[�����g��
		MV1SetScale(m_modelHandle[i][m_NPCState], VGet(5.0f, 5.0f, 5.0f));
		// �R�c���f����X���̉�]�l��180�x�ɃZ�b�g����
		MV1SetRotationXYZ(m_modelHandle[i][m_NPCState], VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f));
		// �R�c���f���̕`��
		MV1DrawModel(m_modelHandle[i][m_NPCState]);

	}


	//DrawExtendFormatString(0 - GetFontSize(), 100, 8.0, 8.0, GetColor(0, 0, 0), "%d", m_RankCount);


}

//-----------------------------------------------------------------------------
// @brief  �O������m_RankCount�����Z����Ƃ��Ɏg�p����
//-----------------------------------------------------------------------------
void NPC::addRankCount()
{
	m_RankCount += 1;
}