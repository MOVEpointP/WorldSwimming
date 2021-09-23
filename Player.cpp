//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "ObstructBase.h"
#include "Effect.h"
#include "NPC.h"
#include "Score.h"

// 静的定数.
const float Player::ACCEL				=15.0f;		// 通常の加速.

//
int Player::m_sHandle;

//const float Player::ACCEL = 0.03f;		// 通常の加速.
const float Player::MAX_SPEED			= 0.8f;			// 最高速度.
const float Player::TRANING_SPEED       = 9.0f;	        // 練習時の倍速スピード.
const float Player::DEFAULT_DECEL		= -0.01f;		// なにもしない時の減速.
const float Player::BREAK_DECEL			= -0.05f;		// ブレーキ時の減速.
const float Player::GRIP_DECEL			= -0.025f;		// グリップの減速.
const float Player::GRIP_POWER			= 0.1f;			// グリップ力.
const float Player::COLIDE_DECEL_FAC	= 0.4f;			// 障害物にぶつかったときの減速率.

//	音量
const int VOLUME_PAL = 100;

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
	:hitRadius(7.5f)
	,m_diveModelHandle(-1)
	,m_swimModelHandle(-1)
	,m_resultModelHandle(-1)
	,m_playerState(0)
	,m_modeCount(0)
	, m_moveFlag(false)
	, m_trainingMaxCount(2)
	, m_speedDisplay(0)
	, m_moveAnimFlag(false)
	, m_moveCount(0)
	, m_gorlFlag(false)
	, m_resultSceneFlag(false)

{	
	
	// サウンドの読み込み
	m_sHandle = LoadSoundMem("data/sound/sara_shrow.wav");

	m_modelHandle[0] = MV1LoadModel("data/model/player/dive.mv1");
	m_modelHandle[1] = MV1LoadModel("data/model/player/Swimming01.mv1");
	m_modelHandle[2] = MV1LoadModel("data/model/player/taisou.mv1");
	m_modelHandle[3] = MV1LoadModel("data/model/player/result.mv1");


	//３Ｄモデルの０番目のアニメーションをアタッチし、
	//アタッチしたアニメーションの総再生時間を取得する
	AttachIndex = MV1AttachAnim(m_modelHandle[0], 0, -1, FALSE);
	TotalTime[DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[0], AttachIndex);
	AttachIndex = MV1AttachAnim(m_modelHandle[1], 0, -1, FALSE);
	TotalTime[SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[1], AttachIndex);
	AttachIndex = MV1AttachAnim(m_modelHandle[2], 0, -1, FALSE);
	TotalTime[TURN] = MV1GetAttachAnimTotalTime(m_modelHandle[2], AttachIndex);
	AttachIndex = MV1AttachAnim(m_modelHandle[3], 0, -1, FALSE);
	TotalTime[RESULT] = MV1GetAttachAnimTotalTime(m_modelHandle[3], AttachIndex);

	//再生時間の初期化
	PlayTime = 0.0f;

	// posはVector型なので、VGetで原点にセット
	pos = VGet(0, 30, 20);
	//pos = VGet(0, 13, 50);
	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle[m_playerState], pos);
	// 移動する力を（すべての座標）ゼロにする
	velocity = VGet(0, 0, 0);
	// 
	dir = VGet(0, 0, 1);
	// キーを押されていない状態にする
	KeyPush = false;
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Player::~Player()
{
	// モデルのアンロード.
	MV1DeleteModel(m_modelHandle[0]);
	MV1DeleteModel(m_modelHandle[1]);
	MV1DeleteModel(m_modelHandle[2]);
	MV1DeleteModel(m_modelHandle[3]);
	// サウンドのアンロード
	DeleteSoundMem(m_sHandle);

	
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Player::Update(float _deltaTime)
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 加速処理.
	VECTOR accelVec = VGet(0, 0, 0);

	// キーが押されておらず、かつキーが押されたら
	if (CheckHitKey(KEY_INPUT_SPACE)&& !KeyPush)
	{
		PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(VOLUME_PAL, m_sHandle);
		KeyPush = true;
	}

	if (KeyPush)
	{
		if (m_playerState == SWIM)
		{
			// z座標が320を超えたら所定の位置に戻る
			if (VSize(pos) > VSize(VGet(0, 0, 320.0f)))
			{
				dir = VGet(0, 0, -1);
			}
			else if (VSize(pos) < VSize(VGet(0, 0, 50.0f)))
			{
			/*	pos = VGet(0, 13, 30);*/
				//velocity = VGet(0, 0, 0);
				m_modeCount++;//往復分カウントする
				dir.z = 1;
			}

			//本番か練習か
			if (m_moveFlag == true)//本番だったら
			{
				if (m_modeCount == 1)
				{
					if (m_gorlFlag == false)
					{
						m_gorlFlag = true;
					}
					m_resultSceneFlag = true;
				}

				if (m_resultSceneFlag == false)
				{
					accelVec = VScale(dir, (ACCEL+ (Score::GetScore()%10)));//スコアの分だけ早くなる
				}
			}
			else if (m_moveFlag == false)//練習だったら
			{
				if (m_modeCount == 2)
				{
					m_resultSceneFlag = true;
				}
				accelVec = VScale(dir, TRANING_SPEED);
			}
		}

		//練習と本番でモーションのスピードを調整する
		if (m_moveFlag == true)
		{
			m_motionSpeed = 0.4f;

		}
		else if (m_moveFlag == false)
		{
			m_motionSpeed = 0.3f;
		}
		// 再生時間を進める
		PlayTime += m_motionSpeed;

	}

	// ベロシティ加速計算.
	velocity = VAdd(velocity, accelVec);

	// 上下方向にいかないようにベロシティを整える.
	velocity = VGet(velocity.x * _deltaTime, 0, velocity.z * _deltaTime);

	// ポジションを更新.
	pos = VAdd(pos, velocity);

	if (KeyPush && m_playerState == DIVE)
	{
		pos.y -= 0.1;
	}


	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle[m_playerState], pos);


	//// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す

	if (PlayTime >= TotalTime[m_playerState])
	{
		if (m_playerState == DIVE)
		{
			m_playerState = SWIM;
			pos.z = 50;

			PlayTime = 0.0f;

		}
		if (m_playerState == SWIM)
		{
			PlayTime = 0.0f;

		}


	}




	// 再生時間をセットする
	MV1SetAttachAnimTime(m_modelHandle[m_playerState], AttachIndex, PlayTime);

}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{

	// 3Dモデルのスケールを拡大
	MV1SetScale(m_modelHandle[m_playerState], VGet(5.0f, 5.0f, 5.0f));
	// ３ＤモデルのX軸の回転値を180度にセットする
	MV1SetRotationXYZ(m_modelHandle[m_playerState], VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f));
	// ３Ｄモデルの描画
	MV1DrawModel(m_modelHandle[m_playerState]);
	
	if (!m_moveFlag)
	{
		//残り往復数の表記
		DrawExtendFormatString(20 - GetFontSize(), 10, 4.0, 4.0, GetColor(0, 0, 0), "残りの往復数：%d", m_trainingMaxCount - m_modeCount);		
	}



}

//-----------------------------------------------------------------------------
// @brief  衝突処理.
//-----------------------------------------------------------------------------
void Player::OnHitObstruct(ObstructBase& obstruct)
{
}

