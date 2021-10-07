//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Effect.h"
#include "NPC.h"
#include "Score.h"
#include "GameScene_Compe.h"
#include "Camera.h"

// 静的定数.
const float Player::ACCEL				=15.0f;		// 通常の加速.

//
int Player::m_sHandle;

//const float Player::ACCEL = 0.03f;		// 通常の加速.
const float Player::TRANING_SPEED       = 9.0f;	        // 練習時の倍速スピード.
const float Player::DEFAULT_DECEL		= -0.01f;		// なにもしない時の減速.
const float Player::BREAK_DECEL			= -0.05f;		// ブレーキ時の減速.
const float Player::GRIP_DECEL			= -0.025f;		// グリップの減速.
const float Player::GRIP_POWER			= 0.1f;			// グリップ力.
const float Player::COLIDE_DECEL_FAC	= 0.4f;			// 障害物にぶつかったときの減速率.

//	音量
const int VOLUME_PAL = 100;

int Player::m_cameraPosX = 0;
VECTOR Player::m_rankEfkDir=VGet(0,0,0);

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
	: hitRadius(7.5f)
	, m_diveModelHandle(-1)
	, m_swimModelHandle(-1)
	, m_resultModelHandle(-1)
	, m_playerState(0)
	, m_modeCount(0)
	, m_moveFlag(false)
	, m_trainingMaxCount(2)
	, m_speedDisplay(0)
	, m_moveAnimFlag(false)
	, m_moveCount(0)
	, GoalFlag(false)
	, ResultSceneFlag(false)
	, m_RoundTrip(-1)
	, m_startTime(0)
	,efkFlag(true)
	,m_countTime(0)
{	
	m_modelHandle[0] = MV1LoadModel("data/model/player/dive.mv1");
	m_modelHandle[1] = MV1LoadModel("data/model/player/Swimming01.mv1");
	//m_modelHandle[2] = MV1LoadModel("data/model/player/taisou.mv1");
	//m_modelHandle[3] = MV1LoadModel("data/model/player/result.mv1");
	m_modelHandle[4] = MV1LoadModel("data/model/player/dive.mv1");
	m_modelHandle[5] = MV1LoadModel("data/model/player/dive.mv1");
	m_modelHandle[6] = MV1LoadModel("data/model/player/Swimming01.mv1");

	m_RoundTrip = LoadGraph("data/img/gameScene/oufuku.png");
	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapchipHandle);


	//３Ｄモデルの０番目のアニメーションをアタッチし、
	//アタッチしたアニメーションの総再生時間を取得する
	AttachIndex = MV1AttachAnim(m_modelHandle[DIVE], 0, -1, FALSE);
	TotalTime[DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[DIVE], AttachIndex);

	AttachIndex = MV1AttachAnim(m_modelHandle[SWIM], 0, -1, FALSE);
	TotalTime[SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[SWIM], AttachIndex);

	AttachIndex = MV1AttachAnim(m_modelHandle[TURN], 0, -1, FALSE);
	TotalTime[TURN] = MV1GetAttachAnimTotalTime(m_modelHandle[TURN], AttachIndex);

	AttachIndex = MV1AttachAnim(m_modelHandle[COMPE_FIRST], 0, -1, FALSE);
	TotalTime[COMPE_FIRST] = MV1GetAttachAnimTotalTime(m_modelHandle[COMPE_FIRST], AttachIndex);

	AttachIndex = MV1AttachAnim(m_modelHandle[COMPE_DIVE], 0, -1, FALSE);
	TotalTime[COMPE_DIVE] = MV1GetAttachAnimTotalTime(m_modelHandle[COMPE_DIVE], AttachIndex);

	AttachIndex = MV1AttachAnim(m_modelHandle[COMPE_SWIM], 0, -1, FALSE);
	TotalTime[COMPE_SWIM] = MV1GetAttachAnimTotalTime(m_modelHandle[COMPE_SWIM], AttachIndex);
	
	//再生時間の初期化
	PlayTime = 0.0f;

	// posはVector型なので、VGetで原点にセット
	pos = VGet(-2, 30, 20);
	//pos = VGet(0, 13, 50);
	// ３Dモデルのポジション設定
	MV1SetPosition(m_modelHandle[m_playerState], pos);
	// 移動する力を（すべての座標）ゼロにする
	velocity = VGet(0, 0, 0);
	// 
	dir = VGet(0, 0, 1);
	// キーを押されていない状態にする
	KeyPush = false;

	playerDir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);



	// 水しぶきエフェクト読み込み
	m_playerOrbitEfk[0] = new PlayEffect("data/effects/swim/hamon.efk");
	m_playerOrbitEfk[1] = new PlayEffect("data/effects/swim/hamon.efk");
	m_playerOrbitEfk[2] = new PlayEffect("data/effects/swim/hamon.efk");

	m_efkDir = VGet(0.0f, 2.0f, 0.0f);
	//m_playerOrbitEfk->SetPlayingEffectRotation(m_efkDir);
	m_efkstartTime = GetNowCount() / 1000;

	// キラキラエフェクト読み込み
	m_rankEfk[2] = new PlayEffect("data/effects/swim/Sranku.efk",15);
	m_rankEfk[1]= new PlayEffect("data/effects/swim/Aranku.efk",15);
	m_rankEfk[0] = new PlayEffect("data/effects/swim/Branku.efk", 15);
	

	m_rankEfkDir = VGet(0.0f, 1.5, 0.0f);


	//m_playerOrbitEfk->SetPlayingEffectRotation(m_efkDir);
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
	MV1DeleteModel(m_modelHandle[4]);
	MV1DeleteModel(m_modelHandle[5]);
	MV1DeleteModel(m_modelHandle[6]);


	// サウンドのアンロード
	DeleteSoundMem(m_sHandle);

	// エフェクトのアンロード
	m_playerOrbitEfk[0]->Delete();
	m_playerOrbitEfk[1]->Delete();
	m_playerOrbitEfk[2]->Delete();

	delete m_playerOrbitEfk[0];
	delete m_playerOrbitEfk[1];
	delete m_playerOrbitEfk[2];

	for (int i = 0; i <= 2; i++)
	{
		// エフェクトのアンロード
		m_rankEfk[i]->Delete();
	}


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

	//本番シーンの処理
	// 本番の最初のシーンに切り替える
	if (m_playerState == DIVE && m_moveFlag)
	{
		m_playerState = COMPE_FIRST;
		pos = VGet(0, 30, 20);

	}

	//COMPE_FIRST　且つ　○秒経ったらモード切り替え
	if (m_playerState == COMPE_FIRST && m_cameraPosX==-70)
	{
		m_playerState = COMPE_DIVE;
		NPC::SetDiveFlag(true);
	}

	// キーが押されておらず、かつスペースキーが押されたら
	if ((m_playerState == DIVE || m_playerState == COMPE_DIVE) && CheckHitKey(KEY_INPUT_SPACE) && !KeyPush)
	{
		PlaySoundMem(m_sHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(VOLUME_PAL, m_sHandle);
		KeyPush = true;
	}

	if (KeyPush)
	{
		// プレイヤーが泳いでいたら
		if (m_playerState == SWIM || m_playerState == COMPE_SWIM)
		{

			// z座標が320を超えたら所定の位置に戻る
			if (VSize(pos) > VSize(VGet(0, 0, 320.0f)))
			{
				dir = VGet(0, 0, -1);
				playerDir = VGet(0.0f, 0.0f, 0.0f);
				if (m_moveFlag == true)//本番だったら
				{
					GameSceneCompe::SetTurnFlag(true);

				}
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
					if (GoalFlag == false)
					{
						GoalFlag = true;
					}
					ResultSceneFlag = true;
					m_moveFlag = false;

				}

				if (ResultSceneFlag == false)
				{
					accelVec = VScale(dir, (ACCEL+ (Score::SetRank()*10)));//スコアの分だけ早くなる
				}
			}
			else if (m_moveFlag == false)//練習だったら
			{
				if (m_modeCount == 1)
				{
					ResultSceneFlag = true;
					m_moveFlag = true;

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



	if (KeyPush && (m_playerState == COMPE_DIVE || m_playerState == DIVE))
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
			// 開始時のタイムを取得
			m_startTime = GetNowCount() / 100;


			m_playerState = SWIM;
			pos.z = 50;

			PlayTime = 0.0f;

		}
		if (m_playerState == SWIM)
		{
			PlayTime = 0.0f;

		}

		//本番シーンの処理
		if (m_playerState == COMPE_DIVE)
		{
			m_playerState = COMPE_SWIM;
			pos.z = 50;

			PlayTime = 0.0f;
		}
		else if (m_playerState == COMPE_SWIM)
		{
			PlayTime = 0.0f;
		}
		else if (m_playerState == COMPE_FIRST)
		{
			PlayTime = 0.0f;
		}

	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(m_modelHandle[m_playerState], AttachIndex, PlayTime);

	// 向きに合わせて回転.
	//MV1SetRotationZYAxis(m_modelHandle[m_playerState], VGet(-0.5f, 0.5f, 0.0f), VGet(0.5f, 0.5f, 0.0f), 0.0f);

	// ３ＤモデルのY軸の回転値を９０度にセットする
	//MV1SetRotationXYZ(m_modelHandle[m_playerState], VGet(90.0f * DX_PI_F / 180.0f, 90.0f * DX_PI_F / 180.0f, 90.0f * DX_PI_F / 180.0f));


	// モデルに向いてほしい方向に回転.
	//MATRIX tmpMat = MV1GetMatrix(m_modelHandle[m_playerState]);
	//MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	//tmpMat = MMult(tmpMat, rotYMat);
	//MV1SetRotationMatrix(m_modelHandle[m_playerState], tmpMat);
	m_countTime = GetNowCount() / 100 - m_startTime;


}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{


		// ３Ｄモデルのスケールを拡大
		MV1SetScale(m_modelHandle[m_playerState], VGet(5.0f, 5.0f, 5.0f));
		// ３ＤモデルのX軸の回転値を180度にセットする
		MV1SetRotationXYZ(m_modelHandle[m_playerState], playerDir);
		// ３Ｄモデルの描画
		MV1DrawModel(m_modelHandle[m_playerState]);
		DrawExtendFormatString(1920 - 600, 1080 - 100, 4.0, 4.0, GetColor(255, 255, 255), "m_countTime:%d", m_countTime);
		DrawExtendFormatString(1920 - 600, 1080 - 200, 4.0, 4.0, GetColor(255, 255, 255), "startTime:%d", m_startTime);


	//キラキラエフェクト描画
	if (m_playerState == SWIM|| m_playerState == COMPE_SWIM)
	{
		m_rankEfk[2]->SetPlayingEffectRotation(m_rankEfkDir);
		m_rankEfk[1]->SetPlayingEffectRotation(m_rankEfkDir);
		m_rankEfk[0]->SetPlayingEffectRotation(m_rankEfkDir);

		if (m_playerOrbitEfk[0]->GetNowPlaying() != 0)
		{
			m_playerOrbitEfk[0]->PlayEffekseer(VAdd(pos, VGet(0, 0, -3)));

		}
		//一秒遅れて水の波紋エフェクトを表示させる	
		if (m_countTime >3)
		{
			if (m_playerOrbitEfk[1]->GetNowPlaying() != 0)
			{
				m_playerOrbitEfk[1]->PlayEffekseer(VAdd(pos, VGet(0, 0, -7)));
			}

		}
		//if (m_countTime > 10)
		//{
		//	if (m_playerOrbitEfk[2]->GetNowPlaying() != 0)
		//	{
		//		m_playerOrbitEfk[2]->PlayEffekseer(VAdd(pos, VGet(0, 0, -5)));
		//	}
		//}
		//// エフェクト再生中はプレイヤーの座標を追尾
		//m_playerOrbitEfk->SetPlayingEffectPos(pos);
		//ランクがSになったらSのエフェクトを出す
		if (Score::SetRank() == 3)
		{
			//ランクによってキラキラエフェクトを出す
			if (m_rankEfk[2]->GetNowPlaying() != 0 )
			{
				m_rankEfk[2]->PlayEffekseer(pos);
				//m_rankEfkDir.z =


			}


		}
		else
		{
			m_rankEfk[2]->StopEffect();

		}

		//ランクがAになったらAのエフェクトを出す

		if (Score::SetRank() == 2 )
		{
			if (m_rankEfk[1]->GetNowPlaying() != 0 )
			{
				m_rankEfk[1]->PlayEffekseer(pos);

			}


		}
		else
		{
			m_rankEfk[1]->StopEffect();

		}
		//ランクがBになったらBのエフェクトを出す

		if (Score::SetRank() == 1 )
		{
			//ランクによってキラキラエフェクトを出す
			if (m_rankEfk[0]->GetNowPlaying() != 0)
			{
				m_rankEfk[0]->PlayEffekseer(pos);

			}


		}
		else
		{
			m_rankEfk[0]->StopEffect();

		}



		m_rankEfk[2]->SetPlayingEffectPos(pos);
		m_rankEfk[1]->SetPlayingEffectPos(pos);
		m_rankEfk[0]->SetPlayingEffectPos(pos);
	}


}

//-----------------------------------------------------------------------------
// @brief  衝突処理.
//-----------------------------------------------------------------------------
void Player::OnHitObstruct(ObstructBase& obstruct)
{
}

