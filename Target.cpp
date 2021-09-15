#include "Target.h"
#include "TestSceneKoga.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>

// 静的定数.
const int Target::m_target_X      =		400;		// ターゲットの初期x座標
const int Target::m_target_Y      =		 10;		// ターゲットの初期y座標
const int Target::m_target_Z      =		  0;		// ターゲットの初期z座標
								  
const int Target::m_score_good    =	      2;		// スコアが変わらない
const int Target::m_score_bad     =		 -5;		// スコアが下がる
const int Target::m_score_perfect =	      5;		// スコアが上がる
								  
const int Target::m_before_good   =    1200;		// グッド判定（前）
const int Target::m_perfect       =	   1325;      	// パーフェクト判定
const int Target::m_after_good    =    1375;		// グッド判定（後）
const int Target::m_final_good    =    1500;		// グッド判定（最後）

int Target::m_targetSpeed = 0;

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
	, TimeCount(0)
	, m_combo(0)
	, m_targetScore(0)
	, hitRadius(7.5f)

{
	// 画像の読み込み
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");

	m_target_accel = 0.1f;

	// posはVector型なので、VGetで原点にセット
	pos = VGet(m_target_X, m_target_Y, m_target_Z);

	// 移動する力を（すべての座標）ゼロにする
	velocity = VGet(0, 0, 0);
	dir = VGet(1, 0, 0);

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// 各種ハンドルのアンロード.
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);
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
		m_target_accel += m_targetSpeed;			
		accelVec = VScale(dir, m_target_accel);		
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

}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Target::Draw()
{
	// 足のアイコンを描画
	DrawGraph(m_posX, 320, m_legImgHandle, TRUE);
	
	// デバッグあたり判定.
	if (m_targetState == END_SHOT)
	{
		//判定表記
		if (m_targerJadgeWord == 2)		//goodなら
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 0), "good!");			//　判定結果表記
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 0, 0), "+1");			//　判定結果表記
		}																												  
		else if (m_targerJadgeWord == 3)//perfectなら																	
		{																												  
																														  
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(255, 0, 0), "perfect!!");	//　判定結果表記
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(255, 100, 0), "+5");		//　判定結果表記

		}
		else if (m_targerJadgeWord == 1)//badなら
		{

 			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 255), "bad...");		//　判定結果表記
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 100, 255), "-5");		//　判定結果表記
		}

	}
		//　判定結果を表示する間のカウント
		TimeCount++;

		if (TimeCount >= 250)
		{
			m_targerJadgeWord = 0;
			TimeCount = 0;
		}
	
		//　１コンボ以上の時にコンボ数を表示する
		if (Combo::GetCombo())
		{
			DrawExtendFormatString(1920 - 700 - GetFontSize(), 300, 4.0, 10.0, GetColor(100, 0, 0), "%dコンボ！", Combo::GetCombo());	//　判定結果表記
		}																																  
																																		  
		DrawExtendFormatString(0, 100, 4.0, 5.0, GetColor(0,100, 0), "スコア：%d", Score::GetScore());									//　判定結果表記
																																		  
		DrawExtendFormatString(100, 600, 4.0, 5.0, GetColor(0, 100, 0), "Shiftで発射！", Score::GetScore());							//　判定結果表記


}

//-----------------------------------------------------------------------------
// @brief  リアクション.
//-----------------------------------------------------------------------------
void Target::Reaction(Target* _target, bool _hitFlag)
{

	switch (_hitFlag)
	{
	case true:

		m_plusX = (20 + m_targetCount * 20);	// ｘ座標をセット
		pos = VGet(m_plusX, 100, -200);			// 座標をセット

		m_targetState = END_SHOT;

		break;

	case false:

		//ショットのときに
		if (m_targetState == NOW_SHOT && pos.x >= 1100)//アイコンが近くにあるかどうか探索
		{
			//スコアを出していく
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{

					m_targetScore += m_score_good;					// スコア変化なし
					m_targerJadgeWord = 2;
					pos = VGet(-2000, -1000, 200);					// 座標を移動して表示しなくする
					m_combo++;										// コンボ数加算
					m_targetState = END_SHOT;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)  // perfect
				{													    
																	    
					m_targetScore += m_score_perfect;                // スコアup
					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);                   // 座標を移動して表示しなくする
					m_combo++;                                       // コンボ数加算
					m_targetState = END_SHOT;

				}
				else												 // bad（それ以外なら）
				{													    
					m_targetScore += m_score_bad;					 // スコアdown
					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);					 // 座標を移動して表示しなくする
					m_combo = -Combo::GetCombo();					 // コンボ数リセット
					m_targetState = END_SHOT;
				}
			}
			else if (pos.x > m_final_good)
			{

				m_targetScore += m_score_bad;	// スコアdown
				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);	// 座標を移動して表示しなくする
				m_combo = -Combo::GetCombo();	// コンボ数リセット
				m_targetState = END_SHOT;

			}

			Score::SetScore(m_targetScore);		//スコアの値をセットする
			Combo::SetCombo(m_combo);			//コンボの値をセットする

		}
		break;
	}
}



