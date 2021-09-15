#include "GameScene_Compe.h"	
#include "Result.h"
#include "Player.h"
#include "ObstructManager.h"
#include "Hitchecker.h"
#include "UI.h"
#include "Camera.h"
#include "NPC.h"
#include "DxLib.h"
#include "Effect.h"
static int enemyNum = 10;					//	エネミーの数
static int COUNTDOWN = 7;					//	カウントダウンの秒数（+2）
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
GameSceneCompe::GameSceneCompe()
	: m_player(nullptr)
	, m_camera(nullptr)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_npc(nullptr)
	, m_MaxGorl(false)
	, m_playerRanking(0)
	, m_liveY(0)
	, m_countryDraw(false)
{
	// 開始時のタイムを取得
	//m_startTime = GetNowCount() / 1000;
	// ステートセット(カウントダウンから)
	//m_state = GAME_SCENE_STATE::COUNTDOWN;
	KeyPush = false;
}

GameSceneCompe::~GameSceneCompe()
{
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
	delete m_camera;	//	カメラのポインタメンバ変数を消去
	delete m_npc;       //  NPCのポインタメンバ変数を消去
	//	メモリの解放処理
	StopSoundMem(m_countryGraphHandle);
	DeleteSoundMem(m_soundHandle);
}

SceneBase* GameSceneCompe::Update(float _deltaTime)
{
	// デバッグビルドのみデバッグ関数を呼び出す
#ifdef _DEBUG
	DebugKey();
#endif
	m_player->SetScene(true);

	//switch (m_state)
	//{
	//case GAME_SCENE_STATE::COUNTDOWN:
	//	if ((COUNTDOWN + 1) - (GetNowCount() / 1000 - m_startTime) <= 1)
	//	{
	//		m_startTime = GetNowCount() / 5000;
	//		m_state = GAME_SCENE_STATE::GAME;
	//	}
	//	break;
	//case GAME_SCENE_STATE::GAME:
	//{
		//プレイヤーが泳ぎ始めたら国家の表示
		if (m_player->GetPlayerState() == SWIM)
		{
			m_countryDraw = true;
		}
		m_player->Update(_deltaTime);
		m_camera->Update(*m_player);
		m_npc->Update(_deltaTime);
		// プレイヤーのランキング保存変数にNPCが何体ゴールしたかの数を入れる
		// NPCが何体ゴールしたかの数を入れる理由
		// プレイヤーのランキングを決める際にNPCが何体着いたかによってランキングを決めるため
		static bool isProcess = false;
		if (m_player->GetGoalFlag() && !isProcess)
		{
			m_npc->addRankCount();
			// 加算したRankCountを取得してリザルトに順位として表示
			m_playerRanking = m_npc->GetRankCount();
			isProcess = true;
		}
		// シーン遷移
		//プレイヤーとNPCが全員ゴールしたらリザルトシーンに移行
		if (m_npc->GetRankCount() >= 4)
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
			//	リザルトシーンに切り替える
			return new Result(m_playerRanking);
		}
	//}
	//	break;

	//default:
	//	break;
	//}

	//	ゲームシーンを表示し続ける
	return this;
}

void GameSceneCompe::Draw()
{
	//プールの表示位置変更
	MV1SetPosition(m_poolModelHandle, VGet(0.0f, 0.0f, 180.0f));
	//プールの描画
	MV1DrawModel(m_poolModelHandle);

	m_player->Draw();
	m_npc->Draw();

	//LIVEの文字を表示
	DrawExtendFormatString(50, m_liveY, 4.0, 4.0, GetColor(255, 0, 0), "LIVE");
	//国の画像を表示
	if (m_countryDraw == true)
	{
		DrawGraph(0, 0, m_countryGraphHandle, TRUE);
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
	//	サウンドハンドルにセット
	m_soundHandle = LoadSoundMem("data/sound/Game/honban.mp3");

	m_player = new Player;			//	プレイヤークラスのインスタンスを生成
	m_camera = new Camera;			//	カメラクラスのインスタンスを生成
	m_npc = new NPC;				//	ＮＰＣクラスのインスタンスを生成
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
}