#include "ResultHalf.h"
#include "GameScene_Compe.h"
#include "Score.h"

const int LOGO_X = 0;
const int LOGO_Y = 0;
const int GUIDANCE_X = 0;
const int GUIDANCE_Y = 0;
const int RESULT_NUM = 3;
const int B_P = 4;   //ランクBのスコア
const int A_P = 7;   //ランクAのスコア
const int S_P = 10;  //ランクSのスコア
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 90;
// 最大透過量
const int defaultTrans = 255;
// 透過量変化用ベクトル
const int transModeration = -1;
//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;
ResultHalf::ResultHalf()
	:m_score(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_checkKeyFlag(false)
	, m_scoreSoundHandle(0)
	, m_scoreGraphHandle(0)
	, m_logoGraphHandle(0)
	, m_guidanceGraphHandle(0)
	, m_exitdoorGraphHandle(0)
	, m_evaluation(0)
	, m_click_sound_handle(0)
	, m_bgmSoundHandle(0)
	, modelHandle(-1)
	, m_checkResultFlag(0)
{
	// 透過量変数を255に設定
	transParent = defaultTrans;
	// 毎透過量変数を１に設定
	permeationAmount = 1;
	//モデル読み込み
	modelHandle = MV1LoadModel("data/model/player/result.mv1");
	//３Ｄモデルの０番目のアニメーションをアタッチする
	AttachIndex = MV1AttachAnim(modelHandle, 0, -1, FALSE);
	//アタッチしたアニメーションの総再生時間を取得する
	TotalTime = MV1GetAttachAnimTotalTime(modelHandle, AttachIndex);
	//再生時間の初期化
	PlayTime = 0.0f;
	// posはVector型なので、VGetで原点にセット
	pos = VGet(30, 30, 0);
	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
	// 
	dir = VGet(160, 0, 1);
	m_score= Score::GetScore();//スコアの値を入れる

	if (m_score < 0)
	{
		m_score = 0;
	}
}

ResultHalf::~ResultHalf()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_evaluationGraphHandle[m_evaluation]);
	DeleteGraph(m_guidanceGraphHandle);
	StopSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_evaluationSoundHandle[m_evaluation]);
	DeleteSoundMem(m_click_sound_handle);					//	ENTERで進むときのサウンドメモリを解放

}

SceneBase* ResultHalf::Update(float _deltaTime)
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
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE && m_checkResultFlag>=3)
	{
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_click_sound_handle);
		PlaySoundMem(m_click_sound_handle, DX_PLAYTYPE_NORMAL);		//	音が再生し終わるまで待機
		return new GameSceneCompe;
	}
	PlayTime += 0.5f;
	if (PlayTime >= TotalTime)
	{
		PlayTime = 0.0f;
	}
	// 再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, AttachIndex, PlayTime);
	return this;
}

void ResultHalf::Draw()
{
	DrawGraph(5, 20, m_backgroundGraphHandle, FALSE);

	if (!m_fadeInFinishFlag)
	{
		 //フェードイン処理
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_logoGraphHandle, TRUE);
			DrawGraph(0, 0, m_exitdoorGraphHandle, TRUE);//
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}
	DrawGraph(LOGO_X, LOGO_Y, m_logoGraphHandle, TRUE);					//	リザルト画面のロゴを表示
	DrawGraph(0, 0, m_exitdoorGraphHandle, TRUE);//ドア表記
	// 透過して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, transParent);
	DrawGraph(GUIDANCE_X, GUIDANCE_Y, m_guidanceGraphHandle, TRUE);		//	リザルト画面のロゴを表示
	// 透過を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	if (m_checkResultFlag >= 1)
	{
		DrawGraph(SCREEN_SIZE_W / 2 , SCREEN_SIZE_H / 3+20, m_mapChip[m_score - (m_score / 10) * 10], TRUE);
		DrawGraph(SCREEN_SIZE_W / 2 -100, SCREEN_SIZE_H / 3 + 20, m_mapChip[m_score / 10], TRUE);
	}
	if (m_checkResultFlag >= 3)
	{
		DrawGraph(-25, 40, m_evaluationGraphHandle[m_evaluation], TRUE);				//	リザルト画面のロゴを表示
	}
	// フェードアウト処理
	if (m_fadeOutFlag)
	{
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// グラフィックを描画
			DrawGraph(0, 0, m_logoGraphHandle, FALSE);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}
	// 3Dモデルのスケールを拡大
	MV1SetScale(modelHandle, VGet(20.0f, 20.0f, 20.0f));
	// ３ＤモデルのX軸の回転値を９０度にセットする
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f));
	// ３Ｄモデルの描画
	//MV1DrawModel(modelHandle);

}

void ResultHalf::Sound()
{
	switch (m_checkResultFlag)
	{
	case 1:
		PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);
		break;
	case 2:
		/*PlaySoundMem(m_scoreSoundHandle, DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_scoreSoundHandle);*/
		break;
	case 3:
		PlaySoundMem(m_evaluationSoundHandle[m_evaluation], DX_PLAYTYPE_BACK, TRUE);
		ChangeVolumeSoundMem(m_volumePal + VOLUME_PAL_SUP, m_evaluationSoundHandle[m_evaluation]);
		break;
	}
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(100, m_bgmSoundHandle);//マジックナンバーにしたので後で直す

}

void ResultHalf::Load()
{
	m_click_sound_handle = LoadSoundMem("data/sound/Result/SwimEnterToSound.mp3");	//	ENTERで進む際のサウンドをロード
	if (Score::SetRank() == C)
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
	m_backgroundGraphHandle = LoadGraph("data/img/result_02_png/swimResult/HalfResult.png");				//	グラフィックハンドルにリザルト画面の背景イメージをセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/SwimFinalResultsBGM.mp3");			//	サウンドハンドルにリザルト画面のBGMをセット
	m_guidanceGraphHandle = LoadGraph("data/img/result_02_png/swimResult/EnterToTitle.png");;			//	ロゴのグラフィックハンドル
	LoadDivGraph("data/img/result_02_png/swimResult/mapchip.png", 10, 10, 1, 150, 150, m_mapChip); //スコアを表示する画像ハンドル

}