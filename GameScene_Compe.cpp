#include "GameScene_Compe.h"
#include "Result.h"
#include "Target.h"
#include "Player.h"
#include "Hitchecker.h"
#include "Camera.h"
#include "NPC.h"
#include "DxLib.h"
#include "Effect.h"
#include "Time.h"
#include "Fade.h"


//static int COUNTDOWN = 7;					//	カウントダウンの秒数（+2）
//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;
const int m_liveX = 45;
const int m_liveY = 38;//ライブ表記の座標
const int m_countryTime = 2;//国の画像を表示する時間

int GameSceneCompe::m_turnFlag= false;


GameSceneCompe::GameSceneCompe()
	: m_player(nullptr)
	, m_camera(nullptr)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_npc(nullptr)
	, m_MaxGorl(false)
	, m_playerRanking(0)
	, m_countryDrawFlag(false)
	,m_timeplayer(0)
	,m_timeFlag(false)
	, m_countryTime(0)
	, m_countryFadeFlag(false)
	, m_countryTopX(0)
	, m_countryUnderX(SCREEN_SIZE_W)
	, m_turnGraphHandle(0)
	, m_spaceDiveGraph(0)
	, m_spacePushFlag(false)
	, m_spaceDivePushGraph(false)
	, m_liveGraph(0)
{

	// 次のシーンへ移行するかどうか
	m_finishFlag = FALSE;
	// ステートセット(カウントダウンから)
	m_state = GAME_SCENE_STATE::COUNTDOWN;
	KeyPush = false;
	
}

GameSceneCompe::~GameSceneCompe()
{
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
	delete m_camera;	//	カメラのポインタメンバ変数を消去
	//	メモリの解放処理
	DeleteSoundMem(m_soundHandle);

	delete m_npc;
	DeleteGraph(m_countryGraphHandle);
}

SceneBase* GameSceneCompe::Update(float _deltaTime)
{
	// デバッグビルドのみデバッグ関数を呼び出す
#ifdef _DEBUG
	DebugKey();
#endif


		m_player->SetScene(true);
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);
		m_npc->Update(_deltaTime);
		// プレイヤーのランキング保存変数にNPCが何体ゴールしたかの数を入れる
		// プレイヤーがゴールした時(フラグをgetterで取得しています)
		static bool isProcess = false;
		if (m_player->GetGoalFlag() && !isProcess)
		{
			m_npc->addRankCount();
			// 加算したRankCountを取得してリザルトに順位として表示
			m_playerRanking = m_npc->GetRankCount();
			isProcess = true;
			m_timeFlag = false;
		}

		//プレイヤーが泳ぎ始めたら国家の表示
		if (m_player->GetPlayerState() == COMPE_SWIM)
		{
			if (m_timeFlag == false&& !m_player->GetGoalFlag())
			{
				// 開始時のタイムを取得
				m_startTime = GetNowCount() / 100;
				m_timeFlag = true;
			}
		}
		//プレイヤーがゴールした瞬間タイムがリセットされている
		if (m_timeFlag == true)
		{
			m_timeplayer = GetNowCount() / 100- m_startTime;
			Time::SetTime(m_timeplayer);			//タイムの値をセットする
		}

		// 国表示の判定
		if (m_player->GetPlayerState() == COMPE_SWIM)
		{
			if (m_countryTopX < SCREEN_SIZE_W/2 && m_countryUnderX > SCREEN_SIZE_W / 2)
			{

				m_countryTopX += 30;
				m_countryUnderX -= 30;

			}
		}

		// シーン遷移
		if (m_npc->GetRankCount() >= 6 && m_player->GetGoalFlag())
		{
			m_MaxGorl = true;
		}
		if (m_MaxGorl == true)
		{
			m_fadeOutFlag = true;
		}
		if (m_fadeOutFinishFlag)
		{
			// 処理したかフラグを元に戻す
			isProcess = false;
			return new Result(m_playerRanking);				//	リザルトシーンに切り替える
		}
		//リザルト画面がすぐ見たい用
		if (m_checkKeyFlag)
		{
			return new Result(m_playerRanking);				//	リザルトシーンに切り替える

		}

		//スペースキーを押した間画像表示が変わる
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			m_spacePushFlag = true;

		}

	//}
	//	break;
	//default:
	//	break;
	//}
	return this;						//	ゲームシーンを表示し続ける
}

void GameSceneCompe::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// フェードイン処理
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(i, i, i);
			// グラフィックを描画
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}


	//プールの表示位置変更
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));
	//プールの描画
	MV1DrawModel(m_poolModelHandle);

	if (m_player->GetPlayerState() == COMPE_SWIM)
	{
		// タイム表示
		DrawGraph(0, 0, m_timeFlame,TRUE);
		DrawExtendFormatString(1920 -250, 1080 - 100, 4.0, 4.0, GetColor(255,255,255), "%d.%d", m_timeplayer/10, m_timeplayer%10);

		//国の画像を縮小
		DrawExtendGraph(m_countryTopX, 0, m_countryUnderX, SCREEN_SIZE_H, m_countryGraphHandle, TRUE);
	}
	else if (m_player->GetPlayerState() == COMPE_DIVE)
	{
		//国の画像表示
		DrawGraph(0, 0, m_countryGraphHandle, TRUE);
		if (!m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spaceDiveGraph, TRUE);
		}
		else if (m_spacePushFlag)
		{
			DrawGraph(0, 0, m_spaceDivePushGraph, TRUE);

		}

	
	}	


	// プレーヤー
	m_player->Draw();
	//npc
	m_npc->Draw();

	//50m地点の画像表示
	if (m_turnFlag == true)
	{
		DrawGraph(0, 0, m_turnGraphHandle, TRUE);

	}

	if (m_player->GetGoalFlag())
	{
		DrawGraph(0, 0, m_goalGraphHandle, TRUE);

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


	if (m_MaxGorl == false)
	{
		DrawGraph(0, 0, m_liveGraph, TRUE);

	}

}

void GameSceneCompe::Sound()
{
	//	本番BGM
	PlaySoundMem(m_soundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal + 150, m_soundHandle);	
}

void GameSceneCompe::Load()
{
	//	グラフィックハンドルにセット
	m_countryGraphHandle = LoadGraph("data/img/compe/country.png");
	m_poolModelHandle = MV1LoadModel("data/model/stage/stage2/poolModel2.pmx");
	m_turnGraphHandle= LoadGraph("data/img/compe/50m.png");
	m_goalGraphHandle = LoadGraph("data/img/compe/100m.png");
	m_spaceDiveGraph = LoadGraph("data/img/gameScene/Dive.png");
	m_spaceDivePushGraph = LoadGraph("data/img/gameScene/DivePush.png");
	m_liveGraph=LoadGraph("data/img/compe/sekai.png");

	//	サウンドハンドルにセット
	m_soundHandle = LoadSoundMem("data/sound/Game/honban.mp3");
	m_timeFlame= LoadGraph("data/img/compe/TimeFlame.png");

	m_player = new Player;			//	プレイヤークラスのインスタンスを生成
	m_camera = new Camera;			//	カメラクラスのインスタンスを生成
	m_npc	 = new NPC;				//	NPCクラスのインスタンスを生成

}

void GameSceneCompe::DebugKey()
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
		m_checkKeyFlag = TRUE;
	}
	if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == FALSE)	//	エンターが押されたら
	{
		m_finishFlag = TRUE;
	}
}
