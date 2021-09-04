#include "TestTitleSceneUeyama.h"
#include "DxLib.h"
#include "GameScene_easy.h"
#include "TestSceneUeyama.h"

// 最大透過量
const int MAX_TRANSP_VAL = 255;
// 透過量変化用ベクトル
const int TRANSP_MODERATION = -1;

// 全難易度
const int ALL_DIFFICULT = 3;

const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

const int VOLUME_PAL_SUP = 130;

//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;


TestTitleSceneUeyama::TestTitleSceneUeyama()
	:m_state(TITLE_TRANS_STATE::FIRST_ENTER)
	, m_cursolNum(0)
	, m_guidanceY(0)
	, m_TitleLogox(0)
	, m_Waterx(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	,m_guidanceYFlag(false)
	, m_TitleLogoxFlag(false)
	, m_WaterxFlag(false)

{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}
	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_transVal_Enter = MAX_TRANSP_VAL;
	// 毎透過量変数を１に設定
	m_permeationAmount = 1;

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");

}

TestTitleSceneUeyama::~TestTitleSceneUeyama()
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
SceneBase* TestTitleSceneUeyama::Update(float _deltaTime)
{

	// ステートメントごとに処理を変更
		// ENTERで次のステートへ
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL, FALSE);//？フラグ作るかどうか
		return new GameSceneEasy();
	
	}


	return this;
}

void TestTitleSceneUeyama::Draw()
{


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
	

	// 選択時にカーソルが見やすいように背景を暗くする
	//if (m_state == TITLE_TRANS_STATE::SECOND_CHOICE)
	//{
	//	// 透過して描画
	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	//	DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
	//	// 透過を元に戻す
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//}

	// ステートメントごとに処理を変更
	//switch (m_state)
	//{
	//case TITLE_TRANS_STATE::FIRST_ENTER:

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

	//	break;

	//case TITLE_TRANS_STATE::SECOND_CHOICE:


		// フェードアウト処理
		//if (m_fadeOutFlag)
		//{
		//	for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		//	{
		//		// 描画輝度をセット
		//		SetDrawBright(255 - i, 255 - i, 255 - i);

		//		// グラフィックを描画
		//		DrawGraph(0, 0, m_backGraphHandle, FALSE);
		//		ScreenFlip();
		//	}
		//	m_fadeOutFinishFlag = true;
		//	
		//}
	//	break;

	//default:

	//	break;


}

/// <summary>
/// サウンドの再生処理
/// </summary>
void TestTitleSceneUeyama::Sound()
{
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal+150, m_soundHandle);
}

/// <summary>
/// 読み込み処理
/// </summary>
void TestTitleSceneUeyama::Load()
{
	m_backGraphHandle = LoadGraph("data/img/Title/Title_Back.png");			//	グラフィックハンドルにタイトル画面のイメージをセット
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");			//	グラフィックハンドルにタイトル画面のイメージをセット
	m_guidanceGraphHandle = LoadGraph("data/img/Title/Title_guidance.png");	//	グラフィックハンドルにタイトル画面のイメージをセット
	m_water = LoadGraph("data/img/Title/title_Water.png");                 //	グラフィックハンドルにタイトル画面のイメージをセット

	m_soundHandle = LoadSoundMem("data/sound/Title/SwimTitleBgm.wav");
	m_click_sound_handle = LoadSoundMem("data/sound/Title/SwimTitleSE.wav");	//	ENTERで進む際のサウンドをロード
}

/// <summary>
/// 点滅エフェクトに用いる透過量の更新処理
/// </summary>
void TestTitleSceneUeyama::UpdateTransparent()
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
else if(m_guidanceYFlag == true)
{
	m_guidanceY -= 0.1f;
	if (m_guidanceY <= -10)
	{
		m_guidanceYFlag = false;
	}
}

////タイトルロゴを左右に動かす
//if (m_TitleLogoxFlag == false)
//{
//	m_TitleLogox += 0.2f;
//	if (m_TitleLogox >= 50)
//	{
//		m_TitleLogoxFlag = true;
//	}
//}
//else if (m_TitleLogoxFlag == true)
//{
//	m_TitleLogox -= 0.2f;
//	if (m_TitleLogox <= 0)
//	{
//		m_TitleLogoxFlag = false;
//	}

//}

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
