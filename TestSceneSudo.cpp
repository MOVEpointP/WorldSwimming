#include "TestSceneSudo.h"
#include "ResultHalf.h"
#include "Target.h"
#include "Player.h"
#include "Hitchecker.h"
#include "Camera.h"
#include "Fade.h"
#include "Score.h"
#include "DxLib.h"
#include "Effect.h"
#include "GameScene_Compe.h"

static int enemyNum = 22;					//	エネミーの数
static int COUNTDOWN = 7;

// ターゲットが飛んでくる間隔 (秒単位)
const int TARGET_SHOT_INTERVAL = 2;

// ターゲットの速度を初期化
static float targetSpeed = 200.0f;

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;

// 音量調整
const int GONG_VOLUME_PAL = 30;
const int DOOR_VOLUME_PAL = 40;


TestSceneSudo::TestSceneSudo()
	:m_player(nullptr)
	, m_camera(nullptr)
	, m_targetCount(0)
	, m_startTime(0)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_endSoundHandle(0)
	, m_finishSoundFlag(false)
	, m_finishFadeCount(0)
	, m_targetSpeed(0)
	, m_scoreHandle(0)
	, m_tensPlaceScore(0)
	, m_onePlaceScore(0)
	, m_rankBHandle(false)
	, m_rankSound(0)
	, m_enterPushFlag(false)
	, m_spacePushFlag(false)
{
	// 次のシーンへ移行するかどうか
	m_finishFlag = FALSE;
	// エネミー・スコアUI初期化
	for (int i = 0; i < enemyNum; i++)
	{
		m_target[i] = nullptr;
	}
	m_target[enemyNum] = nullptr;

	for (int i = 0; i <= 2; i++)
	{
		m_rankSoundFlag[i] = false;
	}

	// 開始時のタイムを取得
	m_startTime = GetNowCount() / 1000;
	// ステートセット(カウントダウンから)
	m_state = GAME_SCENE_STATE::COUNTDOWN;

	srand(time(NULL));//乱数の種を初期化
}


TestSceneSudo::~TestSceneSudo()
{
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
	delete m_camera;	//	カメラのポインタメンバ変数を消去

	//	メモリの解放処理
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_endSoundHandle);
	DeleteSoundMem(m_rankBHandle);
	DeleteSoundMem(m_rankAHandle);
	DeleteSoundMem(m_rankHandle);

	for (int i = 0; i < enemyNum; i++)
	{
		delete m_target[i];
	}

	delete m_target[enemyNum];
}


SceneBase* TestSceneSudo::Update(float _deltaTime)
{
	// デバッグビルドのみデバッグ関数を呼び出す
#ifdef _DEBUG
	//DebugKey();
#endif

	m_targetSpeed = rand() % 50 + 1;//ターゲットの速度を変える

	m_player->SetScene(false);

	//デバック用
	if (CheckHitKey(KEY_INPUT_0))
	{
		return new GameSceneCompe();
	}


	switch (m_state)
	{
	case GAME_SCENE_STATE::COUNTDOWN:
		if ((COUNTDOWN + 1) - (GetNowCount() / 1000 - COUNTDOWN) <= 1)
		{
			m_startTime = GetNowCount() / 1000;
			m_state = GAME_SCENE_STATE::GAME;
		}
		break;

	case GAME_SCENE_STATE::GAME:
		// エネミー射出管理
		//if (GetNowCount() / 1000 - m_startTime > COUNTDOWN)//TARGET_SHOT_INTERVALを変えて射出タイミングを調整する
		if (m_player->GetPlayerState() == SWIM)
		{
			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				m_startTime = GetNowCount() / 1000;
				if (m_target[m_targetCount]->GetIceState() == NO_SHOT)//NO_SHOTの場合
				{
					m_target[m_targetCount]->SetIceState(NOW_SHOT);//ステータスにNOW_SHOTをセット 
					Target::SetTargetSpeedX(m_targetSpeed);
				}
				if (m_target[m_targetCount]->GetIceState() == END_SHOT)//END_SHOTの場合
				{
					m_targetCount++;			//次のエネミーにカウントを進める
				}
			}
		}

		//スペースキーを押した間画像表示が変わる
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			m_spacePushFlag = true;

		}
		else
		{
			m_spacePushFlag = false;
		}

		//エンターキーを押した間画像表示が変わる
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_enterPushFlag = true;
		}
		else
		{
			m_enterPushFlag = false;
		}


		// 現在の番号に応じてエネミーの更新
		m_target[m_targetCount]->Update(_deltaTime);
		m_target[m_targetCount]->SetTargetCount(m_targetCount);

		// アイコンのx軸のポジションを取得
		m_target[m_targetCount]->SetSinglePosX();//ターゲットにｘ座標をセット

		Score::calcScore(m_onePlaceScore, m_tensPlaceScore);

		m_target[m_targetCount]->Reaction(m_target[m_targetCount], false);

		m_player->Update(_deltaTime);

		m_camera->Update(*m_player);


		if (CheckHitKey(KEY_INPUT_F))
		{
			return new ResultHalf();        //    リザルトシーンに切り替える

		}

		//　練習量分往復が完了したら
		if (m_player->ResultSceneFlag)
		{
			m_finishFlag = TRUE;
		}
		if (m_finishFlag == TRUE)
		{
			m_finishSoundFlag = true;
		}
		if (m_fadeOutFinishFlag)
		{
			// scoreUIのスコアをResultのscore変数にセット
			return new ResultHalf();        //    リザルトシーンに切り替える
		}
		break;

	default:
		break;

	}
	return this;							//	ゲームシーンを表示し続ける
}


void TestSceneSudo::Draw()
{
	//プールの表示位置変更
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));

	//プールの描画
	MV1DrawModel(m_poolModelHandle);

	if (m_player->GetPlayerState() == SWIM)
	{
		DrawGraph(0, 0, m_timingImgHandle, TRUE);

		DrawGraph(0, 0, m_spaceHandle, TRUE);

		DrawGraph(0, 0, m_enterHandle, TRUE);

		// ターゲット(アイコン)
		for (int i = 0; i <= m_targetCount; i++)
		{
			m_target[i]->Draw();
		}
		if (m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spacePushHandle, TRUE);
		}
		if (m_enterPushFlag)
		{
			DrawGraph(0, 0, m_enterPushHandle, TRUE);
		}

		DrawGraph(1920 / 2 + 500 + 20, 400, m_mapChipHandle[m_tensPlaceScore], TRUE);
		DrawGraph(0, 0, m_scoreHandle, TRUE);
		DrawGraph(1920 / 2 + 580, 400, m_mapChipHandle[m_onePlaceScore], TRUE);

	}

	// プレーヤー
	m_player->Draw();

	if (m_finishSoundFlag)
	{
		DrawExtendFormatString(SCREEN_SIZE_W / 2 - GetFontSize(), SCREEN_SIZE_H / 2, 4.0, 4.0, GetColor(0, 0, 0), "終了！");

		m_finishFadeCount = GetNowCount() / 1000;

		if (m_finishFadeCount > 1000)
		{
			m_fadeOutFlag = true;
		}
	}

	// フェードアウト処理
	if (m_fadeOutFlag)
	{
		Fade::FadeOut(false);
		m_fadeOutFinishFlag = true;
	}

	if (m_player->GetPlayerState() == DIVE)
	{
		DrawExtendFormatString(1920 / 2 - 170 - GetFontSize(), 1080 - 100, 4.0, 4.0, GetColor(0, 0, 0), "SPACEで飛びこむ");
	}


}

void TestSceneSudo::Sound()
{
	//練習BGMの再生
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_LOOP, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 50, m_soundHandle);

	if (m_finishSoundFlag)
	{
		PlaySoundMem(m_endSoundHandle, DX_PLAYTYPE_NORMAL);
	}

	switch (Score::SetRank())
	{
	case 0:
		break;
	case 1:

		if (CheckSoundMem(m_rankBHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankBHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	case 2:
		if (CheckSoundMem(m_rankAHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankAHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	case 3:
		if (CheckSoundMem(m_rankHandle) == 0 && m_rankSoundFlag[Score::SetRank() - 1] == false)
		{
			PlaySoundMem(m_rankHandle, DX_PLAYTYPE_BACK);
		}
		else
		{
			m_rankSoundFlag[Score::SetRank() - 1] = true;
		}
		break;
	}

}

void TestSceneSudo::Load()
{
	//	グラフィックハンドルにセット
	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");		//練習BGMハンドル
	m_timingImgHandle = LoadGraph("data/img/gameScene/timing2.png");		//判定バーの画像ハンドル
	m_scoreHandle = LoadGraph("data/img/gameScene/score.png");
	m_spaceHandle = LoadGraph("data/img/gameScene/space.png");
	m_spacePushHandle = LoadGraph("data/img/gameScene/spacePush.png");
	m_enterHandle = LoadGraph("data/img/gameScene/enter.png");
	m_enterPushHandle = LoadGraph("data/img/gameScene/enterPush.png");
	m_rankBHandle = LoadSoundMem("data/sound/Game/01.mp3");		//スコアの効果音ハンドル
	m_rankAHandle = LoadSoundMem("data/sound/Game/01.mp3");		//スコアの効果音ハンドル
	m_rankHandle = LoadSoundMem("data/sound/Game/01.mp3");		//スコアの効果音ハンドル

	LoadDivGraph("data/img/gameScene/suuji.png", 10, 10, 1, 60, 60, m_mapChipHandle);

	//	モデルハンドルにセット
	m_poolModelHandle = MV1LoadModel("data/model/stage/stage2/poolModel2.pmx");
	//	サウンドハンドルにセット
	m_endSoundHandle = LoadSoundMem("data/sound/Game/finish.mp3");//練習のメニュー分の距離を達成した時に流れる効果音

	m_player = new Player;			//	プレイヤークラスのインスタンスを生成
	m_camera = new Camera;			//	カメラクラスのインスタンスを生成
	for (int i = 0; i < (enemyNum + 1); i++)
	{
		m_target[i] = new Target;
		m_target[i]->SetInterval(TARGET_SHOT_INTERVAL);
		m_target[i]->SetAccel(targetSpeed);
	}
}

void TestSceneSudo::DebugKey()
{
	// 確認用
	if (CheckHitKey(KEY_INPUT_A))
	{
	}
	if (CheckHitKey(KEY_INPUT_B))
	{
	}
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
	}
}
