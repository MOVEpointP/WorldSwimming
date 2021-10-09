#include "Target.h"
#include "TestSceneKoga.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>
#include "Effect.h"
#include "stdlib.h"

// 静的定数.
const int Target::m_target_X      =		30;		// ターゲットの初期x座標
const int Target::m_target_Y      =		 10;		// ターゲットの初期y座標
const int Target::m_target_Z      =		  0;		// ターゲットの初期z座標
								  
const int Target::m_score_good    =	      1;		// スコアが変わらない
const int Target::m_score_bad     =		 -5;		// スコアが下がる
const int Target::m_score_perfect =	      5;		// スコアが上がる
								  
const int Target::m_before_good   =    1200;		// グッド判定（前）
const int Target::m_perfect       =	   1325;      	// パーフェクト判定
const int Target::m_after_good    =    1375;		// グッド判定（後）
const int Target::m_final_good    =    1500;		// グッド判定（最後）

//const int Target::m_targetSpeed = 0;

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
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
	, m_shotCountTime(0)
	,m_startShotTime(0)
	, m_targetShotCount(0)
{
	// 画像の読み込み
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	//m_o2ImgHandle=LoadGraph("data/img/target/O2.png");
	m_goodHandle = LoadGraph("data/img/gameScene/good.png");
	m_badHandle = LoadGraph("data/img/gameScene/bad.png");
	m_perfectHandle = LoadGraph("data/img/gameScene/perfect.png");
	m_comboHandle = LoadGraph("data/img/gameScene/combo.png");
	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapchipHandle);
	m_goodSoundHandle = LoadSoundMem("data/sound/Game/good.mp3");		//スコアの効果音ハンドル
	m_badSoundHandle = LoadSoundMem("data/sound/Game/bad.mp3");		//スコアの効果音ハンドル@@@
	m_perfectSoundHandle = LoadSoundMem("data/sound/Game/perfect.mp3");		//スコアの効果音ハンドル
	m_targetStandby = LoadGraph("data/img/target/legsStandby.png");//半透明の足アイコンの画像@@@

	m_target_accel = 0.1f;

	// posはVector型なので、VGetで原点にセット
	pos = VGet(m_target_X, m_target_Y, m_target_Z);

	// 移動する力を（すべての座標）ゼロにする
	velocity = VGet(0, 0, 0);
	dir = VGet(1, 0, 0);

	// エフェクト読み込み
	m_goodOrbitEfk = LoadEffekseerEffect("data/effects/swim/good.efk",25.0f);
	m_perfecOrbitEfk= LoadEffekseerEffect("data/effects/swim/perfecd.efk", 25.0f);
	m_badOrbitEfk = LoadEffekseerEffect("data/effects/swim/bad.efk", 25.0f);

	srand(time(NULL));//乱数の種を初期化
	m_startShotTime = GetNowCount() / 100;//NO＿SHOTになった時間を図る	

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// 各種ハンドルのアンロード.
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);

	DeleteSoundMem(m_goodSoundHandle);
	DeleteSoundMem(m_badSoundHandle);
	DeleteSoundMem(m_perfectSoundHandle);

}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Target::Update(float _deltaTime)
{

	 accelVec = VGet(0, 0, 0);

	//	射出フラグがtrueになったら
	if (m_targetState == NOW_SHOT)
	{

		pos.x += m_targetSpeed;			
		accelVec = VScale(dir, m_target_accel);		
	}

	if (m_targetState == NO_SHOT)
	{
		m_shotCountTime = GetNowCount() / 100 - m_startShotTime;
		m_targetShotCount +=1+(Combo::GetCombo()) + (Combo::GetTenCombo() * 10 );
	}
	
 	if (m_targetShotCount>=340)//コンボの数が上がるほど発射する時間が短くなる
	{
		m_targetShotCount = 0;
		if (m_targetState == NO_SHOT)//NO_SHOTの場合
		{
			m_targetSpeed = 10+ (Combo::GetCombo()) + (Combo::GetTenCombo() * 10);//ターゲットの速度を変える

			m_targetState = NOW_SHOT;//ステータスにNOW_SHOTをセット 
		}
	}

	if (m_targetState == END_SHOT)//END_SHOTの場合
	{
		pos = VGet(m_target_X, m_target_Y, m_target_Z);

		m_targetState = NO_SHOT;//ステータスにNOW_SHOTをセット 

		m_startShotTime = GetNowCount() / 100;//NO＿SHOTになった時間を図る	

	}



	// ベロシティ加速計算.
	velocity = VAdd(velocity, accelVec);

	// 上下方向にいかないようにベロシティを整える.
	velocity = VGet(velocity.x * _deltaTime, 0, velocity.z * _deltaTime);

	// ポジションを更新.
	pos = VAdd(pos, velocity);

	// 力をかけ終わったベロシティの方向にディレクションを調整.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}

	// モデルに向いてほしい方向に回転.
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	//判定を描画するフラグがrueになったら時間を計る
	if (m_drawTargetFlag)
	{
		m_countTime = GetNowCount() / 1000 - m_startTime;
		efkFlag = false;
	}
	if (m_countTime  > 1)
	{
		m_drawTargetFlag = false;
		m_countTime = 0;
	}

}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Target::Draw()
{
	//DrawExtendFormatString(1920 - 600, 1080 - 100, 4.0, 4.0, GetColor(255, 255, 255), "m_startShotTime:%d", m_startShotTime);
	//DrawExtendFormatString(1920 - 600, 1080 - 200, 4.0, 4.0, GetColor(255, 255, 255), "m_shotCountTime:%d", m_shotCountTime);

	//待機時は半透明で描画される
	if (m_targetState == NOW_SHOT)
	{
		// 足のアイコンを描画
		DrawGraph(pos.x, 400, m_legImgHandle, TRUE);
	}
	else
	{
		DrawRectGraph(pos.x, 400, 0, 0, m_targetShotCount, 440, m_legImgHandle, TRUE, FALSE);

		DrawGraph(pos.x, 400, m_targetStandby, TRUE);
	}

	//判定を描画する時間が一秒経ったら消えるようにする
	if (m_drawTargetFlag&& m_targerJadgeWord == 2)
	{

		DrawGraph(0, 0, m_goodHandle, TRUE);

	}
	//判定を描画する時間が一秒経ったら消えるようにする
	if (m_drawTargetFlag&& m_targerJadgeWord == 3)
	{
		DrawGraph(0, 0, m_perfectHandle, TRUE);
	}

	//判定を描画する時間が一秒経ったら消えるようにする
	if (m_drawTargetFlag&& m_targerJadgeWord == 1)
	{
		DrawGraph(0, 0, m_badHandle, TRUE);
	}


	// デバッグあたり判定.
	if (m_targetState == END_SHOT)
	{
		//判定表記
		if (m_targerJadgeWord == 2)		//goodなら
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0&&efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_goodOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);
			}

		}																												  
		else if (m_targerJadgeWord == 3)//perfectなら																	
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0 && efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_perfecOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);

			}

		}
		else if (m_targerJadgeWord == 1)//badなら
		{
			if (IsEffekseer2DEffectPlaying(efkHandle) != 0 && efkFlag)
			{
				efkHandle = PlayEffekseer2DEffect(m_badOrbitEfk);
				SetPosPlayingEffekseer2DEffect(efkHandle, 1600, 600, 0);

			}
		}

	}
	//　１コンボ以上の時にコンボ数を表示する
	auto combo = Combo::GetCombo();
	DrawGraph(1920 / 2 + 840, 500 + 50, m_mapchipHandle[Combo::GetCombo()], TRUE);
	DrawGraph(1920 / 2 + 800, 500 + 50, m_mapchipHandle[Combo::GetTenCombo()], TRUE);
	DrawGraph(0, -20, m_comboHandle, TRUE);
}

//-----------------------------------------------------------------------------
// @brief  リアクション.
//-----------------------------------------------------------------------------
void Target::Reaction(Target* _target, bool _hitFlag)
{

	if (_hitFlag)
	{
		//m_plusX = (20 + m_targetCount * 20);	// ｘ座標をセット
		pos = VGet(m_plusX, 100, -200);			// 座標をセット

		//m_targetState = END_SHOT;
	}
	else
	{

		//ショットのときに
		if (m_targetState == NOW_SHOT && pos.x >= 1100)//アイコンが近くにあるかどうか探索
		{
			//スコアを出していく
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{
					PlaySoundMem(m_goodSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_good;					// スコア変化なし
					m_targerJadgeWord = 2;
					//pos = VGet(-2000, -1000, 200);					// 座標を移動して表示しなくする
					m_combo = 1;	
					m_drawTargetFlag = true;
					// コンボ数加算
					m_targetState = END_SHOT;
					//判定を描画するフラグをtrueにする
					//判定の描画をする最初の瞬間の時間を計測
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)  // perfect
				{
					PlaySoundMem(m_perfectSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_perfect;                // スコアup

					m_targerJadgeWord = 3;
					//pos = VGet(-2000, -1000, 200);                   // 座標を移動して表示しなくする
					m_combo = 1;                                     // コンボ数加算
					m_targetState = END_SHOT;
					//判定を描画するフラグをtrueにする
					//判定の描画をする最初の瞬間の時間を計測
					m_drawTargetFlag = true;
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;


				}
				else												 // bad（それ以外なら）
				{
					PlaySoundMem(m_badSoundHandle, DX_PLAYTYPE_BACK);
					m_targetScore += m_score_bad;					 // スコアdown

					m_targerJadgeWord = 1;
					//pos = VGet(-2000, -1000, 200);					 // 座標を移動して表示しなくする
					m_combo = -Combo::GetCombo();									 // コンボ数リセット
					m_tens_place = -Combo::GetTenCombo();
					m_targetState = END_SHOT;
					//判定を描画するフラグをtrueにする
					//判定の描画をする最初の瞬間の時間を計測
					m_drawTargetFlag = true;
					m_startTime = GetNowCount() / 1000;
					efkFlag = true;


				}


			}
			else if (pos.x > m_final_good)
			{
				PlaySoundMem(m_badSoundHandle, DX_PLAYTYPE_BACK);

				m_targetScore += m_score_bad;	// スコアdown

				m_targerJadgeWord = 1;
				//pos = VGet(-2000, -1000, 200);	// 座標を移動して表示しなくする
				m_combo = -Combo::GetCombo();					// コンボ数リセット
				m_tens_place = -Combo::GetTenCombo();
				m_targetState = END_SHOT;

				//判定を描画するフラグをtrueにする
				//判定の描画をする最初の瞬間の時間を計測
				m_drawTargetFlag = true;
				m_startTime = GetNowCount() / 1000;

				efkFlag = true;

			}

			if (Combo::GetCombo() >= 9)
			{
 				m_combo = -Combo::GetCombo();			 // コンボ数リセット
				m_tens_place = 1;
			}


			Score::AddScore(m_targetScore);		//スコアの値を加算する
			Combo::AddCombo(m_combo);			//コンボの値を加算する
			Combo::AddTenCombo(m_tens_place);	//コンボの十の位の値を加算する
			m_combo = 0;						//コンボの値をリセット
			m_tens_place = 0;					//コンボの十の位リセット
			m_targetScore = 0;
		}

		//スコアが0より下に行かないようにする
		if (Score::GetScore() <= 0)
		{
			m_targetScore = -Score::GetScore();
		}
	}
}



