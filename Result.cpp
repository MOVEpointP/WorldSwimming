#include "Result.h"
#include "Score.h"
#include "Time.h"
#include "Title.h"
const int TRANSP_MODERATION = -1;
const int LOGO_X = 0;
const int LOGO_Y = 0;
const int RESULT_NUM = 3;
const int B_P = 4;   //ランクBのスコア
const int A_P = 7;   //ランクAのスコア
const int S_P = 10;  //ランクSのスコア
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 90;
// 最大透過量
const int MAX_TRANSP_VAL = 200;
// 透過量変化用ベクトル
const int transModeration = -1;
//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;

Result::Result(int playerRanking)
	:m_score(0)
	, m_playerRanking(playerRanking)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_checkKeyFlag(false)
	, m_backgroundGraphHandle(0)
	, m_evaluation(0)
	, m_click_sound_handle(0)
	, m_bgmSoundHandle(0)
	, m_time(0)
	, m_oneTime(0)
	, m_tenTime(0)
	, m_decimalTime(0)
{
	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_transVal_Enter = MAX_TRANSP_VAL;
	// 毎透過量変数を１に設定
	m_permeationAmount = 2;
	m_time = Time::GetTime();
}

Result::~Result()
{
	DeleteGraph(m_backgroundGraphHandle);
	DeleteGraph(m_evaluationGraphHandle[m_evaluation]);
	StopSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_evaluationSoundHandle[m_evaluation]);
	DeleteSoundMem(m_click_sound_handle);					//	ENTERで進むときのサウンドメモリを解放
}

SceneBase* Result::Update(float _deltaTime)
{


	if (CheckHitKey(KEY_INPUT_UP))
	{
		m_volumePal++;
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		m_volumePal--;
	}
	if (m_checkResultFlag <= RESULT_NUM)
	{
		WaitTimer(1000);
		m_checkResultFlag++;
	}
	if (!CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = FALSE;
	}
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)
	{
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_click_sound_handle);
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL);		//	音が再生し終わるまで待機
		return new Title;
	}


	return this;
}

void Result::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// フェードイン処理
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backgroundGraphHandle, TRUE);
			DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}

	// 透過量更新
	UpdateTransparent();

	DrawGraph(LOGO_X, LOGO_Y, m_backgroundGraphHandle, TRUE);
	// 透過して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
	//	リザルト画面のロゴを表示
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);
	// 透過を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// 透過して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transVal_Enter);
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	PUSH ENTER
	// 透過を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	// フェードアウト処理
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// グラフィックを描画
			DrawGraph(0, 0, m_backgroundGraphHandle, FALSE);
			DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);

			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}

	//タイムの結果取得
	Time::calcTime(m_oneTime, m_tenTime, m_decimalTime);
	//タイム表示
	DrawGraph(950, SCREEN_SIZE_H / 3 , m_mapChip[m_decimalTime], TRUE);
	DrawGraph(750, SCREEN_SIZE_H / 3, m_mapChip[m_oneTime], TRUE);
	DrawGraph(650, SCREEN_SIZE_H / 3, m_mapChip[m_tenTime], TRUE);

	//DrawExtendFormatString(SCREEN_SIZE_W / 2+300 - GetFontSize(), SCREEN_SIZE_H / 3+280, 4.0, 4.0, GetColor(0, 0, 0), "%d秒", m_time);

	//プレイヤーの順位表示
	DrawGraph(950, SCREEN_SIZE_H / 3 + 260, m_mapChip[m_playerRanking], TRUE);
	//DrawExtendFormatString(SCREEN_SIZE_W / 2+450 - GetFontSize(), SCREEN_SIZE_H / 3+430, 4.0, 4.0, GetColor(0, 0, 0), "%d", m_playerRanking);
}

void Result::Sound()
{
	switch (m_checkResultFlag)
	{
	case 1:
		/*PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 2:
		/*PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m5_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 3:
		PlaySoundMem(m_evaluationSoundHandle[m_evaluation], DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_evaluationSoundHandle[m_evaluation]);
		break;
	}
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal+100, m_bgmSoundHandle);

}

void Result::Load()
{
	m_click_sound_handle = LoadSoundMem("data/sound/swimResult/SwimEnterToSound.mp3");	//	ENTERで進む際のサウンドをロード
	if (Score::SetRank()==C)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_C.png");		//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (Score::SetRank() == B)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_B.png");		//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (Score::SetRank() == A)
	{
		m_evaluation = 1;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_A.png");				//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");		//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (Score::SetRank() == S)
	{
		m_evaluation = 2;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_S.png");			//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/SwimScoreSE_S.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	m_backgroundGraphHandle = LoadGraph("data/img/result_02_png/swimResult/FinalResults.png");				//	グラフィックハンドルにリザルト画面の背景イメージをセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/SwimFinalResultsBGM.mp3");			//	サウンドハンドルにリザルト画面のBGMをセット
	m_guidanceGraphHandle = LoadGraph("data/img/result_02_png/swimResult/EnterToTitle.png");;			//	ロゴのグラフィックハンドル
	LoadDivGraph("data/img/result_02_png/swimResult/mapchip.png", 10, 10, 1, 150, 150, m_mapChip);

}
 //点滅エフェクトに用いる透過量の更新処理
void Result::UpdateTransparent()
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
}
