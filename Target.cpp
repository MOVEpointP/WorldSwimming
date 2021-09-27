#include "Target.h"
#include "TestSceneKoga.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>

// 静的定数.
const int Target::m_target_X      =		400;		// ターゲットの初期x座標
const int Target::m_target_Y      =		 10;		// ターゲットの初期y座標
const int Target::m_target_Z      =		  0;		// ターゲットの初期z座標
								  
const int Target::m_score_good    =	      1;		// スコアが変わらない
const int Target::m_score_bad     =		 -3;		// スコアが下がる
const int Target::m_score_perfect =	      3;		// スコアが上がる
								  
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
	, m_combo(0)
	, m_targetScore(0)
	, hitRadius(7.5f)
	,m_goodHandle(-1)
	, m_badHandle(-1)
	, m_perfectHandle(-1)
	,m_tens_place(0)
	, m_scoreHandle(0)
	, m_onePlaceScore(0)
	, m_tensPlaceScore(0)
{
	// 画像の読み込み
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");
	m_goodHandle = LoadGraph("data/img/gameScene/good.png");
	m_badHandle = LoadGraph("data/img/gameScene/bad.png");
	m_perfectHandle = LoadGraph("data/img/gameScene/perfect.png");
	m_comboHandle = LoadGraph("data/img/gameScene/combo.png");
	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapchipHandle);
	m_scoreHandle= LoadGraph("data/img/gameScene/score.png");
	//m_mapchipHandle[0]= LoadGraph("data/img/gameScene/suuji.png");
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
	DrawGraph(m_posX, 400, m_legImgHandle, TRUE);
	
	// デバッグあたり判定.
	if (m_targetState == END_SHOT)
	{
		//判定表記
		if (m_targerJadgeWord == 2)		//goodなら
		{
			DrawGraph(0, 0, m_goodHandle, TRUE);
		}																												  
		else if (m_targerJadgeWord == 3)//perfectなら																	
		{																												  
			DrawGraph(0, 0, m_perfectHandle, TRUE);
		}
		else if (m_targerJadgeWord == 1)//badなら
		{
			DrawGraph(0, 0, m_badHandle, TRUE);
		}

	}
		//　判定結果を表示する間のカウント
		m_timeCount++;

		if (m_timeCount >= 250)
		{
			m_targerJadgeWord = 0;
			m_timeCount = 0;
		}
	
		//　１コンボ以上の時にコンボ数を表示する
		auto combo = Combo::GetCombo();
		DrawGraph(1920 / 2 + 840, 500 + 50, m_mapchipHandle[Combo::GetCombo()], TRUE);
		DrawGraph(1920 / 2 + 800, 500 + 50, m_mapchipHandle[Combo::GetTenCombo()], TRUE);
		//DrawExtendFormatString(1920 / 2+840 - GetFontSize(), 500, 6.0, 10.0, GetColor(255, 255, 255), "%d", Combo::GetCombo());	//　判定結果表記
		DrawGraph(0, -20, m_comboHandle, TRUE);

		DrawGraph(0, 0, m_scoreHandle, TRUE);
		DrawGraph(1920 / 2 + 580, 400, m_mapchipHandle[Score::GetScore()], TRUE);
		DrawGraph(1920 / 2 + 520, 400, m_mapchipHandle[Score::GetTenScore()/10], TRUE);

		//DrawExtendFormatString(1920 / 2 + 850 - GetFontSize(), 730, 2.0, 2.0, GetColor(255, 255, 255), "%d", Score::GetScore());									//　判定結果表記
																																		  
		//DrawExtendFormatString(100, 600, 4.0, 5.0, GetColor(0, 100, 0), "Shiftで発射！", Score::GetScore());							//　判定結果表記

}

//-----------------------------------------------------------------------------
// @brief  リアクション.
//-----------------------------------------------------------------------------
void Target::Reaction(Target* _target, bool _hitFlag)
{

	if (_hitFlag)
	{
		m_plusX = (20 + m_targetCount * 20);	// ｘ座標をセット
		pos = VGet(m_plusX, 100, -200);			// 座標をセット

		m_targetState = END_SHOT;
	}
	else
	{

		//ショットのときに
		if (m_targetState == NOW_SHOT && pos.x >= 1100)//アイコンが近くにあるかどうか探索
		{
			//スコアを出していく
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{

					m_targetScore += m_score_good;					// スコア変化なし
					m_tensPlaceScore += m_score_good;
					m_targerJadgeWord = 2;
					pos = VGet(-2000, -1000, 200);					// 座標を移動して表示しなくする
					m_combo = 1;									// コンボ数加算
					m_targetState = END_SHOT;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)  // perfect
				{

					m_targetScore += m_score_perfect;                // スコアup
					m_tensPlaceScore += m_score_perfect;

					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);                   // 座標を移動して表示しなくする
					m_combo = 1;                                     // コンボ数加算
					m_targetState = END_SHOT;

				}
				else												 // bad（それ以外なら）
				{
					m_targetScore += m_score_bad;					 // スコアdown
					m_tensPlaceScore += m_score_bad;

					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);					 // 座標を移動して表示しなくする
					m_combo = -Combo::GetCombo();									 // コンボ数リセット
					m_tens_place = -Combo::GetTenCombo();
					m_targetState = END_SHOT;
				}

				if (Score::GetScore() >= 9)
				{
					m_targetScore = -Score::GetScore();			 // 一の位用にスコア数リセット
				}

			}
			else if (pos.x > m_final_good)
			{

				m_targetScore += m_score_bad;	// スコアdown
				m_tensPlaceScore += m_score_bad;

				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);	// 座標を移動して表示しなくする
				m_combo = -Combo::GetCombo();					// コンボ数リセット
				m_tens_place = -Combo::GetTenCombo();
				m_targetState = END_SHOT;

			}

			if (Combo::GetCombo() >= 9)
			{
 				m_combo = -Combo::GetCombo();			 // コンボ数リセット
				m_tens_place = 1;
			}


			Score::AddScore(m_targetScore);		//スコアの値を加算する
			Score::AddTenScore(m_tensPlaceScore);
			Combo::AddCombo(m_combo);			//コンボの値を加算する
			Combo::AddTenCombo(m_tens_place);	//コンボの十の位の値を加算する
			m_onePlaceScore = Score::GetScore();
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



