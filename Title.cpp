#include "Title.h"
#include "DxLib.h"
#include "TestSceneNakamura.h"
#include "Fade.h"


// 最大透過量
const int MAX_TRANSP_VAL = 255;
// 透過量変化用ベクトル
const int TRANSP_MODERATION = -1;
//画面サイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 130;
//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;
Title::Title()
	: m_guidanceY(0)
	, m_TitleLogox(0)
	, m_Waterx(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_guidanceYFlag(false)
	, m_TitleLogoxFlag(false)
	, m_WaterxFlag(false)
	, m_gameSceneFlag(false)

{
	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_transVal_Enter = MAX_TRANSP_VAL;
	// 毎透過量変数を１に設定
	m_permeationAmount = 1;

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");
}

Title::~Title()
{
	StopSoundMem(m_soundHandle);
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_click_sound_handle);		//	ENTERで進むときのサウンドメモリを解放
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_guidanceGraphHandle);
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns> シーンのポインタ </returns>
SceneBase* Title::Update(float _deltaTime)
{

	if (m_gameSceneFlag)
	{
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL, FALSE);
		return new TestSceneNakamura();
	}


	// ステートメントごとに処理を変更
		// ENTERで次のステートへ
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_gameSceneFlag = true;

	}
	return this;
}

void Title::Draw()
{
	//エンターが押されたら
	if (m_gameSceneFlag)
	{
		Fade::FadeOut(m_backGraphHandle);
		Fade::FadeIn(m_manualGraphHandle);
	}
	else
	{
		//タイトル画面の表示
		if (!m_fadeInFinishFlag)
		{
			// フェードイン処理
			for (int i = 0; i < 255; i += FADE_IN_SPEED)
			{
				// 描画輝度をセット
				SetDrawBright(i, i, i);

				// グラフィックを描画
				DrawGraph(0, 0, m_backGraphHandle, TRUE);
				DrawGraph(m_TitleLogox, 0, m_logoGraphHandle, TRUE);
				DrawGraph(m_Waterx, 0, m_water, TRUE);

				ScreenFlip();
			}
			m_fadeInFinishFlag = true;
		}
		// 透過量更新
		UpdateTransparent();

		// バックグラウンド
		DrawGraph(0, 0, m_backGraphHandle, TRUE);
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
		// タイトルロゴ
		DrawGraph(m_TitleLogox, 0, m_logoGraphHandle, TRUE);
		DrawGraph(m_Waterx, 0, m_water, TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transVal_Enter);
		DrawGraph(0, m_guidanceY, m_guidanceGraphHandle, TRUE);		//	PUSH ENTER
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
}

/// <summary>
/// サウンドの再生処理
/// </summary>
void Title::Sound()
{
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 150, m_soundHandle);
}

/// <summary>
/// 読み込み処理
/// </summary>
void Title::Load()
{
	m_backGraphHandle = LoadGraph("data/img/Title/Title_Back.png");			//	グラフィックハンドルにタイトル画面のイメージをセット
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");			//	グラフィックハンドルにタイトル画面のイメージをセット
	m_guidanceGraphHandle = LoadGraph("data/img/Title/Title_guidance.png");	//	グラフィックハンドルにタイトル画面のイメージをセット
	m_water = LoadGraph("data/img/Title/title_Water.png");                 //	グラフィックハンドルにタイトル画面のイメージをセット
	m_manualGraphHandle = LoadGraph("data/img/gameScene/manual.png");

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");
	m_click_sound_handle = LoadSoundMem("data/sound/Title/SwimTitleSE.wav");	//	ENTERで進む際のサウンドをロード
}

/// <summary>
/// 点滅エフェクトに用いる透過量の更新処理
/// </summary>
void Title::UpdateTransparent()
{
	//タイトル、水画像のフェード処理
	// 透過量が255より大きくなったら
	if (m_transpVal >= MAX_TRANSP_VAL)
	{
		// 透過量を255に設定
		m_transpVal = MAX_TRANSP_VAL - 1;

		// 毎透過量を-1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 透過量が0より小さければ
	else if (MAX_TRANSP_VAL / 3 >= m_transpVal)
	{
		// 透過量を設定
		m_transpVal = MAX_TRANSP_VAL / 3 + 1;

		// 毎透過量を1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 毎透過量を透過量に加算する
	m_transpVal += m_permeationAmount;


	//Enter to Startのフェード処理
	// 透過量が255より大きくなったら
	if (m_transVal_Enter >= MAX_TRANSP_VAL)
	{
		// 透過量を255に設定
		m_transVal_Enter = MAX_TRANSP_VAL - 1;

		// 毎透過量を-1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 透過量が0より小さければ
	else if (MAX_TRANSP_VAL / 2 >= m_transVal_Enter)
	{
		// 透過量を設定
		m_transVal_Enter = MAX_TRANSP_VAL / 2 + 1;

		// 毎透過量を1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 毎透過量を透過量に加算する
	m_transVal_Enter += m_permeationAmount;
	//タイトルロゴを上下に動かす
	if (m_guidanceYFlag == false)
	{
		m_guidanceY += 0.1f;
		if (m_guidanceY >= 0)
		{
			m_guidanceYFlag = true;
		}
	}
	else if (m_guidanceYFlag == true)
	{
		m_guidanceY -= 0.1f;
		if (m_guidanceY <= -10)
		{
			m_guidanceYFlag = false;
		}
	}


	//水の画像を左右に動かす
	if (m_WaterxFlag == false)
	{
		m_Waterx += 0.05f;
		if (m_Waterx >= 10)
		{
			m_WaterxFlag = true;
		}
	}
	else if (m_WaterxFlag == true)
	{
		m_Waterx -= 0.05f;
		if (m_Waterx <= 0)
		{
			m_WaterxFlag = false;
		}

	}
}
