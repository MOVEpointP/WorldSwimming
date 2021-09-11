#include "Target.h"
#include "ObstructBase.h"
#include "TestSceneKoga.h"
#include "Hitchecker.h"
#include "Effect.h"
#include "UI.h"
#include "Score.h"
#include "Combo.h"
#include <cmath>

// 静的定数.
const int Target::m_target_X = 400;
//const int Target::m_target_X = 0;
const int Target::m_target_Y = 10;
const int Target::m_target_Z = 0;

const int Target::m_font_X = 1340;
const int Target::m_font_Y = 160;
const int Target::m_font_size = 50;
const int Target::m_font_thick = -1;

const int Target::m_score_good = 0;			//スコアが変わらない
const int Target::m_score_bad = -10;		//スコアが下がる
const int Target::m_score_perfect = 10;		//スコアが上がる

const int Target::m_before_good = 1200;		//	グッド判定（前）
const int Target::m_perfect = 1325;	//　パーフェクト判定
const int Target::m_after_good = 1375;		//　グッド判定（後）
const int Target::m_final_good = 1500;		//　グッド判定（最後）



static const double pi = 3.141592653589793;

// 音量
const int VOLUME_PAL = 100;

int Target::m_targetSpeed = 0;

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
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
	// 画像の読み込み
	//m_handImgHandle= LoadGraph("data/img/target/hand.png");
	m_legImgHandle=LoadGraph("data/img/target/legs.png");
	m_o2ImgHandle=LoadGraph("data/img/target/O2.png");
	//modelHandle = MV1LoadModel("data/model/target/icecream/SVH-icecream/icecream.pmx");
	m_FontHandle = CreateFontToHandle(NULL, m_font_size, m_font_thick, DX_FONTTYPE_NORMAL);

	m_target_accel = 0.1f;

	// posはVector型なので、VGetで原点にセット
	pos = VGet(m_target_X, m_target_Y, m_target_Z);
	// 移動する力を（すべての座標）ゼロにする
	velocity = VGet(0, 0, 0);
	// 
	dir = VGet(1, 0, 0);

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Target::~Target()
{
	// 各種ハンドルのアンロード.
	//MV1DeleteModel(modelHandle);
	//DeleteGraph(m_handImgHandle);
	DeleteGraph(m_legImgHandle);
	DeleteGraph(m_o2ImgHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Target::Update(float _deltaTime)
{
	
	 accelVec = VGet(0, 0, 0);
	//	アイス射出フラグがtrueになったら
	if (m_iceState == NOW_SHOT)
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

	// 力をかけ終わったベロシティの方向にディレクションを調整.
	if (VSize(velocity) != 0)
	{
		dir = VNorm(velocity);
	}
	//printfDx("%f %f %f\n", dir.x, dir.y, dir.z);

	// ３Dモデルのポジション設定
	//MV1SetPosition(modelHandle, pos);

	//// 向きに合わせて回転.
	//MV1SetRotationZYAxis(modelHandle, dir, VGet(0.0f, 1.0f, 0.0f), 0.0f);

	// モデルに向いてほしい方向に回転.
	//MATRIX tmpMat = MV1GetMatrix(modelHandle);
	MATRIX rotYMat = MGetRotY(180.0f * (float)(DX_PI / 180.0f));
	//tmpMat = MMult(tmpMat, rotYMat);
	//MV1SetRotationMatrix(modelHandle, tmpMat);
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Target::Draw()
{
	DrawGraph(m_posX, 320, m_legImgHandle, TRUE);
	
	// 元モデルでは小さすぎるので描画倍率を設定
	//MV1SetScale(modelHandle, VGet(3.0f, 3.0f, 3.0f));
	if (m_iceState != NO_SHOT)
	{
		// ３Ｄモデルの描画
		//MV1DrawModel(modelHandle);
	}
	
	if (m_iceState == NO_SHOT)
	{
		//int timebuffer = GetNowCount() / 1000;
		
		//DrawCircleAA(m_font_X, m_font_Y,((m_shotInterval + 1) - (timebuffer - m_setTime))*30
		//	,32, GetColor(255, 255, 255),FALSE);
	}

	// デバッグあたり判定.
	//DrawSphere3D(pos, hitRadius, 5, 0x00ffff, 0x00ffff, false);
	if (m_iceState == END_SHOT)
	{
		//判定表記
		if (m_targerJadgeWord == 2)//goodなら
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 0), "good!");//判定結果表記

		}
		else if (m_targerJadgeWord == 3)//perfectなら
		{

			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(255, 0, 0), "perfect!!");//判定結果表記
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(255, 100, 0), "+10");//判定結果表記

		}
		else if (m_targerJadgeWord == 1)//badなら
		{

 			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 320, 4.0, 10.0, GetColor(0, 0, 255), "bad...");	//判定結果表記
			DrawExtendFormatString(1920 / 1 - 200 - GetFontSize(), 500, 4.0, 10.0, GetColor(0, 100, 255), "-10");	//判定結果表記
		}

	}
		//時間が経つと文字が消える(getnowcount上手く使えなかったからフレーム数えることにした）
		TimeCount++;

		if (TimeCount >= 250)
		{
			m_targerJadgeWord = 0;
			TimeCount = 0;
		}
	
		//１コンボ以上の時に表示する(失敗してる）
		if (Combo::GetCombo())
		{
			DrawExtendFormatString(1920 - 700 - GetFontSize(), 300, 4.0, 10.0, GetColor(100, 0, 0), "%dコンボ！", Combo::GetCombo());	//判定結果表記
		}

		DrawExtendFormatString(0, 100, 4.0, 5.0, GetColor(0,100, 0), "スコア：%d", Score::GetScore());	//判定結果表記

		DrawExtendFormatString(100, 600, 4.0, 5.0, GetColor(0, 100, 0), "Shiftで発射！", Score::GetScore());	//判定結果表記


}

//-----------------------------------------------------------------------------
// @brief  リアクション.
//-----------------------------------------------------------------------------
void Target::Reaction(UI* _ui, bool _hitFlag)
{
	switch (_hitFlag)
	{
	case true:
		m_plusX = (20 + m_targetCount * 20);//ｘ座標をセット
		pos = VGet(m_plusX, 100, -200);
		//m_hitFlag = true;
		
		//ScoreUpdateUI(*_ui, _hitFlag);

		//PlaySoundMem(m_hitIceSoundHandle, DX_PLAYTYPE_BACK);
		//ChangeVolumeSoundMem(VOLUME_PAL, m_hitIceSoundHandle);

		// ３Dモデルのポジション設定
		//MV1SetPosition(modelHandle, pos);
		m_iceState = END_SHOT;
		break;
	case false:
		//ショットのときに
		if (m_iceState == NOW_SHOT&& pos.x>=1100)//アイコンが近くにあるかどうか探索
		{
			//スコアを出していく
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				if ((m_before_good <= pos.x && pos.x <= m_perfect) || (m_after_good <= pos.x && pos.x <= m_final_good))//good
				{
					m_targetScore += m_score_good;//スコア変化なし
					m_targerJadgeWord = 2;
 					pos = VGet(-2000, -1000, 200);// 座標を移動して表示しなくする
					m_combo++;//コンボ数加算
					m_iceState = END_SHOT;

				}
				else if (m_perfect < pos.x && pos.x < m_after_good)//perfect
				{
					m_targetScore += m_score_perfect;//スコアup
					m_targerJadgeWord = 3;
					pos = VGet(-2000, -1000, 200);// 座標を移動して表示しなくする
					m_combo++;//コンボ数加算
					m_iceState = END_SHOT;

				}
				else //bad（それ以外なら）
				{
					m_targetScore += m_score_bad;//スコアdown
					m_targerJadgeWord = 1;
					pos = VGet(-2000, -1000, 200);// 座標を移動して表示しなくする
					m_combo = -Combo::GetCombo();//コンボ数リセット
					m_iceState = END_SHOT;
				}
			}
			else if (pos.x > m_final_good)
			{
				m_targetScore += m_score_bad;//スコアdown
				m_targerJadgeWord = 1;
				pos = VGet(-2000, -1000, 200);// 座標を移動して表示しなくする
				m_combo = -Combo::GetCombo();//コンボ数リセット
				m_iceState = END_SHOT;
			}

			Score::SetScore(m_targetScore);//スコアの値をセットする
			Combo::SetCombo(m_combo);//コンボの値をセットする

			//ScoreUpdateUI(*_ui, _hitFlag);

			//PlaySoundMem(m_missIceSoundHandle, DX_PLAYTYPE_BACK);
			//ChangeVolumeSoundMem(VOLUME_PAL, m_missIceSoundHandle);

			// ３Dモデルのポジション設定
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
// @brief  衝突処理.
//-----------------------------------------------------------------------------
//void Target::OnHitObstruct(ObstructBase& obstruct)
//{
//	// 自分自身の位置を障害物のあたり判定分ずらす.
//	// Z軸とX軸の二次元座標として位置ずらしを行う.
//	VECTOR yZeroTarget = VGet(pos.x, 0, pos.z);
//	VECTOR yZeroObstruct = VGet(obstruct.GetPos().x, 0, obstruct.GetPos().z);
//
//	VECTOR obsToTarget = VSub(yZeroTarget, yZeroObstruct);
//
//	// ぶつかったときに離す距離。ちょうどだとfloat誤差で重なり続けるので少し間をあける.
//	float awayRange = (hitRadius + obstruct.GetHitRadius() + 0.01f);
//
//	VECTOR awayVec = VScale(VNorm(obsToTarget), awayRange);
//	pos = VAdd(yZeroObstruct, awayVec);
//
//	// ぶつかったら減速する.
//	velocity = VScale(velocity, COLIDE_DECEL_FAC);
//}