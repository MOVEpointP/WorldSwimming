
#include "GameScene_easy.h"
#include "ResultHalf.h"
#include "Target.h"
#include "Player.h"
#include "ObstructManager.h"
#include "Hitchecker.h"
#include "UI.h"
#include "Camera.h"

#include "DxLib.h"
#include "Effect.h"

//	エネミーの数
static int enemyNum = 19;					
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

GameSceneEasy::GameSceneEasy()
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
{
	// 次のシーンへ移行するかどうか
	m_finishFlag = FALSE;

	// エネミー・スコアUI初期化
	for (int i = 0; i < enemyNum; i++)
	{
		m_target[i] = nullptr;
	}
	m_target[enemyNum] = nullptr;


	// 開始時のタイムを取得
	m_startTime = GetNowCount() / 1000;

	// ステートセット(カウントダウンから)
	m_state = GAME_SCENE_STATE::COUNTDOWN;

	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");

	srand(time(NULL));  //乱数の種を初期化
}

GameSceneEasy::~GameSceneEasy()
{
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
	delete m_camera;	//	カメラのポインタメンバ変数を消去

	//	メモリの解放処理
	DeleteSoundMem(m_soundHandle);


	for (int i = 0; i < enemyNum; i++)
	{
		delete m_target[i];
	}


	delete m_target[enemyNum];

}

SceneBase* GameSceneEasy::Update(float _deltaTime)
{
	// デバッグビルドのみデバッグ関数を呼び出す
#ifdef _DEBUG

	DebugKey();

#endif

	m_targetSpeed = rand() % 50 + 1;//ターゲットの速度を変える

	m_player->SetScene(false);

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
		if (CheckHitKey(KEY_INPUT_LSHIFT))
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

		// 現在の番号に応じてエネミーの更新
		m_target[m_targetCount]->Update(_deltaTime);
		m_target[m_targetCount]->SetTargetCount(m_targetCount);

		// アイコンのx軸のポジションを取得
		m_target[m_targetCount]->SetSinglePosX();//ターゲットにｘ座標をセット
		m_target[m_targetCount]->Reaction(m_target[m_targetCount], false);
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);



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


void GameSceneEasy::Draw()
{

	//プールの表示位置変更
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));

	//プールの描画
	MV1DrawModel(m_poolModelHandle);

	DrawGraph(0, 0, m_timingImgHandle, TRUE);

	// ターゲット(アイス)
	for (int i = 0; i <= m_targetCount; i++)
	{
		m_target[i]->Draw();
	}


	// プレーヤー
	m_player->Draw();


	if (m_finishSoundFlag)
	{
		PlaySoundMem(m_endSoundHandle, DX_PLAYTYPE_BACK);

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
		for (int i = 0; i < 255; i += FADE_OUT_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(255 - i, 255 - i, 255 - i);

			// グラフィックを描画
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;

	}

}

void GameSceneEasy::Sound()
{

	//練習BGMの再生
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_LOOP, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 50, m_soundHandle);

}

void GameSceneEasy::Load()
{
	//	グラフィックハンドルにセット
	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");		//練習BGMハンドル
	m_timingImgHandle = LoadGraph("data/img/gameScene/timing.png");		//判定バーの画像ハンドル

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


