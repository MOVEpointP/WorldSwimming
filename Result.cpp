#include "Result.h"					
#include "TestTitleSceneUeyama.h"
#include "Score.h"

const int LOGO_X = 0;			//リザルト画面の画像のｘ座標
const int LOGO_Y = 0;			//リザルト画面の画像のｙ座標
const int RESULT_NUM = 3;		//シーン切り替えができるようになるまでの秒数
const int B_P = 4;				//ランクBのスコア
const int A_P = 7;			    //ランクAのスコア	スコアの値によってランクの表記が変わる
const int S_P = 10;				//ランクSのスコア	値が小さすぎるので修正する
const int SCREEN_SIZE_W = 1920;	//画面サイズ
const int SCREEN_SIZE_H = 1080; //画面サイズ
const int VOLUME_PAL_SUP = 90;  // 音量調整
// 最大透過量
const int defaultTrans = 255;
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
	, m_backGraphHandle(0)
	, m_evaluation(0)
	, m_click_sound_handle(0)
	, m_bgmSoundHandle(0)
{
	// 透過量変数を255に設定
	transParent = defaultTrans;
	// 毎透過量変数を１に設定
	permeationAmount = 1;
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}
}

Result::~Result()
{
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_evaluationGraphHandle[m_evaluation]);
	StopSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_evaluationSoundHandle[m_evaluation]);
	DeleteSoundMem(m_click_sound_handle);					//	ENTERで進むときのサウンドメモリを解放
}

SceneBase* Result::Update(float _deltaTime)
{
	//三秒経ったらエンターキーを押してタイトルシーンに戻れるようになる
	if (m_checkResultFlag <= RESULT_NUM)
	{
		WaitTimer(1000);
		m_checkResultFlag++;
	}
	if (!CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = FALSE;
	}
	// 透過量が255より大きくなったら
	if (transParent >= defaultTrans)
	{
		// 透過量を121に設定
		transParent = defaultTrans - 1;
		// 毎透過量を-1にする
		permeationAmount *= transModeration;
	}
	// 透過量が0より小さければ
	else if (0 >= transParent)
	{
		// 透過量を１に設定
		transParent = 1;
		// 毎透過量を1にする
		permeationAmount *= transModeration;
	}
	// 毎透過量を透過量に加算する
	transParent += (permeationAmount * 3);
	//エンターキーを押したら効果音を流してタイトルシーンに戻る
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)
	{
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_click_sound_handle);
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL);		//	音が再生し終わるまで待機
		return new TestTitleSceneUeyama;
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
			DrawGraph(0, 0, m_backGraphHandle, TRUE);

			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}
	DrawGraph(LOGO_X, LOGO_Y, m_backGraphHandle, TRUE);		//	リザルト画面を表示

	// 透過して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, transParent);
	// 透過を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (m_checkResultFlag >= 3)
	{
		DrawExtendGraph(900-300+50-20+80,400-300+50, 1300+150+50-10-100+80, 600+150-100+50, m_evaluationGraphHandle[m_evaluation], TRUE);
		//ランク表示+画像サイズ縮小 引数の説明（x1,y1,x2,y2(グラフィックを描画する矩形の左上頂点の座標)(グラフィックを描画する矩形の右下頂点＋１の座標))
	}
	// フェードアウト処理
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// グラフィックを描画
			DrawGraph(0, 0, m_backGraphHandle, FALSE);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}
	auto score = Score::GetScore();
	//スコア表示
	DrawExtendFormatString(SCREEN_SIZE_W / 2+50 - GetFontSize(), SCREEN_SIZE_H / 4+90, 4.0, 4.0, GetColor(0, 0, 0), "%d", score);
	//タイム表示
	DrawExtendFormatString(SCREEN_SIZE_W / 2 + 280 - GetFontSize(), SCREEN_SIZE_H / 3 + 280, 4.0, 4.0, GetColor(0, 0, 0), "0");
	//ランク表示
	DrawExtendFormatString(SCREEN_SIZE_W / 2+450 - GetFontSize(), SCREEN_SIZE_H / 3+430, 4.0, 4.0, GetColor(0, 0, 0), "%d", m_playerRanking);
}

void Result::Sound()
{
	//ランクが表示される前に効果音が流れてしまっているので修正する
	//ランクが表示された時の効果音
	PlaySoundMem(m_evaluationSoundHandle[m_evaluation], DX_PLAYTYPE_BACK, TRUE);
	ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_evaluationSoundHandle[m_evaluation]);
	//BGMを再生
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal+100, m_bgmSoundHandle);
}

void Result::Load()///m_scoreの
{
	m_click_sound_handle = LoadSoundMem("data/sound/swimResult/Result/SwimEnterToSound.mp3");	//	ENTERで進む際のサウンドをロード
	if (m_score > B_P)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_C.png");		//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (m_score < B_P)
	{
		m_evaluation = 0;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_B.png");		//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (m_score >= B_P && m_score < A_P)
	{
		m_evaluation = 1;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_A.png");				//	グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/Result/SwimScoreSE_C_A.mp3");		//	サウンドハンドルにリザルト画面の効果音をセット
	}
	if (m_score == S_P)
	{
		m_evaluation = 2;
		m_evaluationGraphHandle[m_evaluation] = LoadGraph("data/img/result_02_png/swimResult/Result_S.png");			//	@@@@グラフィックハンドルにリザルト画面のイメージをセット
		m_evaluationSoundHandle[m_evaluation] = LoadSoundMem("data/sound/SwimScoreSE_S.mp3");			//	サウンドハンドルにリザルト画面の効果音をセット
	}
	m_backGraphHandle = LoadGraph("data/img/result_02_png/swimResult/FinalResults.png");				//	グラフィックハンドルにリザルト画面のイメージをセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/SwimFinalResultsBGM.mp3");			//	サウンドハンドルにリザルト画面のBGMをセット	
}