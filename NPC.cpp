//-----------------------------------------------------------------------------
// @brief  NPCクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "NPC.h"
#include "ObstructBase.h"
#include "Effect.h"

//プレイヤーのｃｐｐに描画、アップデートを行う
//描画、アップデートは今までで一人分しかされていないため

// 静的定数.
const float NPC::ACCEL = 15.0f;		// 通常の加速.

//
int NPC::m_sHandle;

//const float Player::ACCEL = 0.03f;		// 通常の加速.
const float NPC::MAX_SPEED = 0.8f;			// 最高速度.
const float NPC::TRANING_SPEED = 9.0f;	        // 練習時の倍速スピード.
const float NPC::DEFAULT_DECEL = -0.01f;		// なにもしない時の減速.
const float NPC::BREAK_DECEL = -0.05f;		// ブレーキ時の減速.
const float NPC::GRIP_DECEL = -0.025f;		// グリップの減速.
const float NPC::GRIP_POWER = 0.1f;			// グリップ力.
const float NPC::COLIDE_DECEL_FAC = 0.4f;			// 障害物にぶつかったときの減速率.

//	音量
const int VOLUME_PAL = 100;

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
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

	// サウンドの読み込み
	m_sHandle = LoadSoundMem("data/sound/sara_shrow.wav");


	// ３Ｄモデルの読み込み
	//m_diveModelHandle = MV1LoadModel("data/model/player/dive.mv1");			//ダイブモーション付きモデル
	//m_swimModelHandle = MV1LoadModel("data/model/player/Swimming01.mv1");	//泳ぎモーション付きモデル
	//m_resultModelHandle = MV1LoadModel("data/model/player/result.mv1");		//リザルトポーズモーション付きモデル

	for (int i = 0; i <= 2; i++)
	{
		m_modelHandle[i][0] = MV1LoadModel("data/model/npc/dive.mv1");
		m_modelHandle[i][1] = MV1LoadModel("data/model/npc/Swimming01.mv1");
		m_modelHandle[i][2] = MV1LoadModel("data/model/player/taisou.mv1");
		m_modelHandle[i][3] = MV1LoadModel("data/model/player/result.mv1");

		//３Ｄモデルの０番目のアニメーションをアタッチし、
		//アタッチしたアニメーションの総再生時間を取得する
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

	//再生時間の初期化
	PlayTime = 0.0f;

	// エフェクト読み込み
	m_playerOrbitEfk = new PlayEffect("data/effects/PlayerLine.efk");
	m_efkDir = VGet(0.0f, DX_PI_F, 0.0f);
	m_playerOrbitEfk->SetPlayingEffectRotation(m_efkDir);

	// posはVector型なので、VGetで原点にセット
	//pos[0] = VGet(23, 30, 20);
	//pos[1] = VGet(50, 30, 43);
	//pos[2] = VGet(80, 30, 40);

	pos[0] = VGet(23, 30, 20);
	pos[1] = VGet(50, 30, 20);
	pos[2] = VGet(80, 30, 20);

	//pos = VGet(0, 13, 50);
	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle[0][m_NPCState], pos[0]);
	MV1SetPosition(m_modelHandle[1][m_NPCState], pos[1]);
	MV1SetPosition(m_modelHandle[2][m_NPCState], pos[2]);

	// 移動する力を（すべての座標）ゼロにする
	velocity[0] = VGet(0, 0, 0);
	velocity[1] = VGet(0, 0, 0);
	velocity[2] = VGet(0, 0, 0);

	// 
	// キーを押されていない状態にする
	KeyPush = false;
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
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

	// モデルのアンロード.
	// サウンドのアンロード
	DeleteSoundMem(m_sHandle);
	// エフェクトのアンロード
	m_playerOrbitEfk->Delete();
	delete m_playerOrbitEfk;
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void NPC::Update(float _deltaTime)
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 加速処理.
	VECTOR accelVec[3];

	accelVec[0] = VGet(0, 0, 0);
	accelVec[1] = VGet(0, 0, 0);
	accelVec[2] = VGet(0, 0, 0);


	// キーが押されておらず、かつキーが押されたら
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
				// z座標が320を超えたら所定の位置に戻る
				if (VSize(pos[i]) > VSize(VGet(0, 0, 320.0f)))
				{
					dir[i] = VGet(0, 0, -1);
				}
			}

			//　プール右端についたらゴールをしたフラグを返す　NPC一体目
			if (VSize(pos[0]) < VSize(VGet(0, 0, 50.0f)))
			{
				if (GorlFlag[0] == false)
				{
					GorlFlag[0] = true;
					m_RankCount++;
				}
			}

			//　プール右端についたらゴールをしたフラグを返す　NPC二体目
			if (VSize(pos[1]) < VSize(VGet(0, 0, 57.0f)))
			{
				if (GorlFlag[1] == false)
				{
					GorlFlag[1] = true;
					m_RankCount++;
				}
			}

			//　プール右端についたらゴールをしたフラグを返す　NPC三体目
			if (VSize(pos[2]) < VSize(VGet(0, 0, 85.0f)))
			{
				if (GorlFlag[2] == false)
				{
					GorlFlag[2] = true;
					m_RankCount++;
				}
			}


			//本番か練習か
			//if (m_moveFlag == true)//本番だったら
			//{
			for (int i = 0; i <= 2; i++)
			{
				if (GorlFlag[i] == false)
				{
					accelVec[i] = VScale(dir[i], ACCEL + (i + 2));

				}
			}
				
			//}
			//if (m_moveFlag == false)//練習だったら
			//{
			//	accelVec = VScale(dir, TRANING_SPEED);
			//}
		}

		//練習と本番でモーションのスピードを調整する
		 m_motionSpeed = 0.4f;
		//if (m_moveFlag == true)
		//{
		//	

		//}
		//else if (m_moveFlag == false)
		//{
		//	m_motionSpeed = 0.3f;
		//}
		// 再生時間を進める
		PlayTime += m_motionSpeed;

	}

	for (int i = 0; i <= 2; i++)
	{
		// ベロシティ加速計算.
		velocity[i] = VAdd(velocity[i], accelVec[i]);

	}


	for (int i = 0; i <= 2; i++)
	{
		// 上下方向にいかないようにベロシティを整える
		velocity[i] = VGet(velocity[i].x * _deltaTime, 0, velocity[i].z * _deltaTime);

		// ポジションを更新.
		pos[i] = VAdd(pos[i], velocity[i]);

		if (KeyPush && m_NPCState == DIVE)
		{
			pos[i].y -= 0.1;
		}
		// ３Dモデルのポジション設定
		MV1SetPosition(m_modelHandle[i][m_NPCState], pos[i]);


	}





	//// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す

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
		// 再生時間をセットする
		MV1SetAttachAnimTime(m_modelHandle[i][m_NPCState], AttachIndex, PlayTime);

	}


}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void NPC::Draw()
{
	for (int i = 0; i <= 2; i++)
	{
		// 3Dモデルのスケールを拡大
		MV1SetScale(m_modelHandle[i][m_NPCState], VGet(5.0f, 5.0f, 5.0f));
		// ３ＤモデルのX軸の回転値を180度にセットする
		MV1SetRotationXYZ(m_modelHandle[i][m_NPCState], VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f));
		// ３Ｄモデルの描画
		MV1DrawModel(m_modelHandle[i][m_NPCState]);

	}


	//DrawExtendFormatString(0 - GetFontSize(), 100, 8.0, 8.0, GetColor(0, 0, 0), "%d", m_RankCount);


}

//-----------------------------------------------------------------------------
// @brief  外部からm_RankCountを加算するときに使用する
//-----------------------------------------------------------------------------
void NPC::addRankCount()
{
	m_RankCount += 1;
}