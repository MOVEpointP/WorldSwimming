//-----------------------------------------------------------------------------
// @brief  NPCクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "NPC.h"
#include "Effect.h"

// 静的定数.
const float NPC::ACCEL = 15.0f;		// 通常の加速.
int NPC::m_diveFlag = 0;	//スタティックのメンバ変数の初期化


//	音量
const int VOLUME_PAL = 100;
//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
NPC::NPC()
	:m_NPCState(0)
	, m_startTime(0)
{
	for (int i = 0; i <= NPC_NUMBER-1; i++)
	{
		m_modelHandle[i][0] = MV1LoadModel("data/model/npc/dive.mv1");
		m_modelHandle[i][1] = MV1LoadModel("data/model/npc/Swimming01.mv1");
		m_modelHandle[i][2] = MV1LoadModel("data/model/npc/taisou.mv1");
		m_modelHandle[i][3] = MV1LoadModel("data/model/npc/result.mv1");
		m_modelHandle[i][4] = MV1LoadModel("data/model/npc/dive.mv1");
		m_modelHandle[i][5] = MV1LoadModel("data/model/npc/dive.mv1");
		m_modelHandle[i][6] = MV1LoadModel("data/model/npc/Swimming01.mv1");


		//３Ｄモデルの０番目のアニメーションをアタッチし、
		//アタッチしたアニメーションの総再生時間を取得する
		AttachIndex = MV1AttachAnim(m_modelHandle[i][DIVE], 0, -1, FALSE);
		TotalTime[DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[i][DIVE], AttachIndex);

		AttachIndex = MV1AttachAnim(m_modelHandle[i][SWIM], 0, -1, FALSE);
		TotalTime[SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[i][SWIM], AttachIndex);

		AttachIndex = MV1AttachAnim(m_modelHandle[i][TURN], 0, -1, FALSE);
		TotalTime[TURN] = MV1GetAttachAnimTotalTime(m_modelHandle[i][TURN], AttachIndex);

		AttachIndex = MV1AttachAnim(m_modelHandle[i][COMPE_FIRST], 0, -1, FALSE);
		TotalTime[COMPE_FIRST] = MV1GetAttachAnimTotalTime(m_modelHandle[i][COMPE_FIRST], AttachIndex);

		AttachIndex = MV1AttachAnim(m_modelHandle[i][COMPE_DIVE], 0, -1, FALSE);
		TotalTime[COMPE_DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[i][COMPE_DIVE], AttachIndex);

		AttachIndex = MV1AttachAnim(m_modelHandle[i][COMPE_SWIM], 0, -1, FALSE);
		TotalTime[COMPE_SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[i][COMPE_SWIM], AttachIndex);

		dir[i] = VGet(0, 0, 1);
		m_gorlflag[i] = false;
		NPCDir[i] = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);
	}
	m_rankcount = 0;
	//再生時間の初期化
	PlayTime = 0.0f;
	pos[0] = VGet(23/*0*/, 30, 20);
	pos[1] = VGet(50/*23*/, 30, 20);
	pos[2] = VGet(80/* -55*/, 30, 20);
	pos[3] = VGet(-30/*0*/, 30, 20);
	pos[4] = VGet(-60/*0*/, 30, 20);

	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle[0][m_NPCState], pos[0]);
	MV1SetPosition(m_modelHandle[1][m_NPCState], pos[1]);
	MV1SetPosition(m_modelHandle[2][m_NPCState], pos[2]);
	MV1SetPosition(m_modelHandle[3][m_NPCState], pos[3]);
	MV1SetPosition(m_modelHandle[4][m_NPCState], pos[4]);

	// 移動する力を（すべての座標）ゼロにする
	velocity[0] = VGet(0, 0, 0);
	velocity[1] = VGet(0, 0, 0);
	velocity[2] = VGet(0, 0, 0);
	velocity[3] = VGet(0, 0, 0);
	velocity[4] = VGet(0, 0, 0);


	// NPCの加速変化用変数初期化
	m_npcGoalAccel[0] = 0.1f ;
	m_npcGoalAccel[1] = 0.05f;
	m_npcGoalAccel[2] = 0.00f;
	m_npcGoalAccel[3] = 0.00f;
	m_npcGoalAccel[4] = 0.00f;

	// キーを押されていない状態にする
	KeyPush = false;

	// 開始時のタイムを取得
	m_startTime = GetNowCount() / 100;

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
NPC::~NPC()
{
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		MV1DeleteModel(m_modelHandle[i][0]);
		MV1DeleteModel(m_modelHandle[i][1]);
		MV1DeleteModel(m_modelHandle[i][2]);
		MV1DeleteModel(m_modelHandle[i][3]);
		MV1DeleteModel(m_modelHandle[i][4]);
		MV1DeleteModel(m_modelHandle[i][5]);


	}
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void NPC::Update(float _deltaTime)
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	// 加速処理.
	VECTOR accelVec[NPC_NUMBER];
	accelVec[0] = VGet(0, 0, 0);
	accelVec[1] = VGet(0, 0, 0);
	accelVec[2] = VGet(0, 0, 0);
	accelVec[3] = VGet(0, 0, 0);
	accelVec[4] = VGet(0, 0, 0);

	//本番シーンの処理

	for (int i = 0; i < NPC_NUMBER; i++)
	{

		// 本番の最初のシーンに切り替える
		if (m_NPCState == DIVE )
		{
			m_NPCState = COMPE_FIRST;
	
		}
		//COMPE_FIRST　且つ　○秒経ったらカメラ切り替え
		if (m_NPCState == COMPE_FIRST && m_diveFlag)
		{
			m_NPCState = COMPE_DIVE;
		}
	}


	// キーが押されておらず、かつスペースキーが押されたら
	if ((m_NPCState == DIVE || m_NPCState == COMPE_DIVE)&&CheckHitKey(KEY_INPUT_SPACE) && !KeyPush)
	{
		KeyPush = true;
	}
	if (KeyPush&& m_NPCState !=COMPE_FIRST)
	{
		if (m_NPCState == SWIM || m_NPCState == COMPE_SWIM)
		{
			//NPCのスピードに変化をつける
			m_npcGoalAccel[0] += 0.002f;
			m_npcGoalAccel[1] += 0.006f;
			m_npcGoalAccel[2] += 0.01f ;
			m_npcGoalAccel[3] += 0.01f ;
			m_npcGoalAccel[4] += 0.01f ;
			

			for (int i = 0; i < NPC_NUMBER; i++)
			{
				// z座標が320を超えたら所定の位置に戻る
				if (VSize(pos[i]) > VSize(VGet(0, 0, 320.0f)))
				{
					dir[i] = VGet(0, 0, -1);
					NPCDir[i] = VGet(0.0f, 0.0f, 0.0f);
				}
			}
			//　プール右端についたらゴールをしたフラグを返す　NPC一体目
			if (VSize(pos[0]) < VSize(VGet(0, 0, 50.0f)))
			{
				if (m_gorlflag[0] == false)
				{
					m_gorlflag[0] = true;
					m_rankcount++;
					pos[0].z = 50;

				}
			}
			//　プール右端についたらゴールをしたフラグを返す　NPC二体目
			if (VSize(pos[1]) < VSize(VGet(0, 0, 57.0f)))
			{
				if (m_gorlflag[1] == false)
				{
					m_gorlflag[1] = true;
					m_rankcount++;
					pos[1].z = 50;
				}
			}
			//　プール右端についたらゴールをしたフラグを返す　NPC三体目
			if (VSize(pos[2]) < VSize(VGet(0, 0, 86.0f)))
			{
				if (m_gorlflag[2] == false)
				{
					m_gorlflag[2] = true;
					m_rankcount++;
					pos[2].z = 50;
				}
			}
			if (VSize(pos[3]) < VSize(VGet(-30-1, 0, 50.0f)))
			{
				if (m_gorlflag[3] == false)
				{
					m_gorlflag[3] = true;
					m_rankcount++;
					pos[3].z = 50;

				}
			}
			if (VSize(pos[4]) < VSize(VGet(-60-1, 0, 50.0f)))
			{
				if (m_gorlflag[4] == false)
				{
					m_gorlflag[4] = true;
					m_rankcount++;
					pos[4].z = 50;

				}
			}


			for (int i = 0; i < NPC_NUMBER; i++)
			{
				if (m_gorlflag[i] == false)
				{
					accelVec[i] = VScale(dir[i], ACCEL + (i + 2)+ m_npcGoalAccel[i]);

				}
			}
		}
		//練習と本番でモーションのスピードを調整する
		 m_motionSpeed = 0.4f;

		// 再生時間を進める
		PlayTime += m_motionSpeed;
	}
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		// ベロシティ加速計算.
		velocity[i] = VAdd(velocity[i], accelVec[i]);
	}	
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		// 上下方向にいかないようにベロシティを整える
		velocity[i] = VGet(velocity[i].x * _deltaTime, 0, velocity[i].z * _deltaTime);
		
		// ポジションを更新.
		pos[i] = VAdd(pos[i], velocity[i]);

		if (KeyPush && (m_NPCState == COMPE_DIVE || m_NPCState == DIVE))
		{
			pos[i].y -= 0.1f;
		}
		// ３Dモデルのポジション設定
		MV1SetPosition(m_modelHandle[i][m_NPCState], pos[i]);
	}
	//// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (PlayTime > TotalTime[m_NPCState])
	{
		for (int i = 0; i <=  NPC_NUMBER; i++)
		{
			//NPCの位置がずれてる理由かもしれない
			if (m_NPCState == DIVE)
			{
				pos[i].z = 50;

				PlayTime = 0.0f;

				if (i == NPC_NUMBER-1)
				{
					m_NPCState = SWIM;

				}
			}
		}
		if (m_NPCState == SWIM)
		{
			PlayTime = 0.0f;
		}

		//本番シーンの処理
		if (m_NPCState == COMPE_DIVE)
		{
			m_NPCState = COMPE_SWIM;

			for (int i = 0; i < NPC_NUMBER; i++)
			{
				pos[i].z = 50;
			}

			PlayTime = 0.0f;
		}
		else if (m_NPCState == COMPE_SWIM)
		{
			PlayTime = 0.0f;
		}
		else if (m_NPCState == COMPE_FIRST)
		{
			PlayTime = 0.0f;
		}

	}
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		// 再生時間をセットする
		MV1SetAttachAnimTime(m_modelHandle[i][m_NPCState], AttachIndex, PlayTime);
	}
}
//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void NPC::Draw()
{
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		// 3Dモデルのスケールを拡大
		MV1SetScale(m_modelHandle[i][m_NPCState], VGet(5.0f, 5.0f, 5.0f));
		// ３ＤモデルのX軸の回転値を180度にセットする
		MV1SetRotationXYZ(m_modelHandle[i][m_NPCState], NPCDir[i]);
		// ３Ｄモデルの描画
		MV1DrawModel(m_modelHandle[i][m_NPCState]);
	}
}
//-----------------------------------------------------------------------------
// @brief  外部からm_RankCountを加算するときに使用する
//-----------------------------------------------------------------------------
void NPC::addRankCount()
{
	m_rankcount += 1;
}