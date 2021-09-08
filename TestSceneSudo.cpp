#include "TestSceneSudo.h"
#include "Result.h"
#include "ResultHalf.h"
//#include "Mark.h"
#include "Target.h"
#include "Player.h"
#include "ObstructManager.h"
#include "Hitchecker.h"
#include "UI.h"
#include "Camera.h"

#include "DxLib.h"
#include "Effect.h"

static int enemyNum = 19;					//	エネミーの数
static int GIRL_Y = 0;						//	中華女子の初期Y座標
static int LADY_Y = 0;						//	中華女性のY座標
static int GIRL_MIN_Y = -80;				//	中華女子の最小Y座標
static int COUNTDOWN = 7;					//	カウントダウンの秒数（+2）

////中華少女の速度
//static float girlSpeed = 80.0f;

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
	/*, m_mark(nullptr)*/
	, m_effect(nullptr)
	, m_targetCount(0)
	, m_startTime(0)
	, m_iceThrowFlag(false)
	, m_iceHitFlagBuffer(false)
	, m_girl_Y(GIRL_Y)
	, m_lady_Y(LADY_Y)
	, m_girlUpFlag(false)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_girl_hitReactionFlag(false)
	, m_girl_missReactionFlag(false)
	, m_girl_ReactionFlag(false)
	//　確認用
	, m_hitCount(0)
	, m_hitFlag(false)
	, m_endSoundHandle(0)
	, m_finishSoundFlag(false)
	, m_soundCount(0)
{
	// 次のシーンへ移行するかどうか
	m_finishFlag = FALSE;
	// エネミー・スコアUI初期化
	for (int i = 0; i < enemyNum; i++)
	{
		m_target[i] = nullptr;
		m_hit_ui[i] = nullptr;
	}
	m_target[enemyNum] = nullptr;

	m_ui = nullptr;

	// 開始時のタイムを取得
	m_startTime = GetNowCount() / 1000;
	// ステートセット(カウントダウンから)
	m_state = GAME_SCENE_STATE::COUNTDOWN;

	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");


}

TestSceneSudo::~TestSceneSudo()
{
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
	delete m_camera;	//	カメラのポインタメンバ変数を消去
	//delete m_mark;		//	マークのポインタメンバ変数を消去
	//	メモリの解放処理
	StopSoundMem(m_finishSoundHandle);
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_finishGraphHandle);
	DeleteGraph(m_manualGraphHandle);
	DeleteGraph(m_girlGraphHandle);
	DeleteGraph(m_girl_missReaction_GraphHandle);	//  女の子の画像メモリを開放する
	DeleteGraph(m_girl_hitReaction_GraphHandle);	//  女の子の画像メモリを開放する
	DeleteGraph(m_ladyGraphHandle);
	DeleteSoundMem(m_soundHandle);
	DeleteSoundMem(m_finishSoundHandle);
	DeleteSoundMem(m_iceSoundHandle);
	DeleteSoundMem(m_missSoundHandle);
	DeleteSoundMem(m_hitSoundHandle);
	DeleteSoundMem(m_doorSoundHandle);

	for (int i = 0; i < enemyNum; i++)
	{
		delete m_target[i];
		delete m_hit_ui[i];
	}

	delete m_ui;

	delete m_target[enemyNum];

	m_effect->Delete();
	delete m_effect;
}

SceneBase* TestSceneSudo::Update(float _deltaTime)
{
	// デバッグビルドのみデバッグ関数を呼び出す
#ifdef _DEBUG
	DebugKey();
#endif

	m_player->SetScene(false);

	switch (m_state)
	{
	case GAME_SCENE_STATE::COUNTDOWN:
		if ((COUNTDOWN + 1) - (GetNowCount() / 1000 - m_startTime) <= 1)
		{
			m_startTime = GetNowCount() / 1000;
			m_state = GAME_SCENE_STATE::GAME;
		}
		break;
	case GAME_SCENE_STATE::GAME:
		//// 机の更新
		//m_mark->Mark_Update();

		if (m_targetCount == 0)
		{
			m_target[m_targetCount]->SetSetTime(m_startTime);
		}


		// エネミー射出管理
		if (GetNowCount() / 1000 - m_startTime > TARGET_SHOT_INTERVAL)
		{
			m_startTime = GetNowCount() / 1000;
			if (m_target[m_targetCount]->GetIceState() == NO_SHOT)
			{
				m_target[m_targetCount]->SetIceState(NOW_SHOT);
				PlaySoundMem(m_iceSoundHandle, DX_PLAYTYPE_BACK);
				ChangeVolumeSoundMem(m_volumePal + 20, m_iceSoundHandle);
			}
			if (m_target[m_targetCount]->GetIceState() == END_SHOT)
			{
				m_girl_ReactionFlag = false;						// 女の子がリアクションしないようにする
				m_girl_hitReactionFlag = false;
				m_girl_missReactionFlag = false;
				m_target[m_targetCount + 1]->SetSetTime(m_startTime);
				m_targetCount++;
			}
		}

		// 現在の番号に応じてエネミーの更新
		m_target[m_targetCount]->Update(_deltaTime);
		m_target[m_targetCount]->SetTargetCount(m_targetCount);
		m_iceHitFlagBuffer = HitChecker::Check(*m_player, *m_target[m_targetCount]);
		// x,y,z軸のそれぞれのポジションを取得
                                      		m_target[m_targetCount]->SetSinglePosX();
		// m_iceHitFlagBufferがtrueになったら
		if (!m_girl_ReactionFlag && m_target[m_targetCount]->GetIceState() == NOW_SHOT)
		{
			if (m_iceHitFlagBuffer)
			{
				m_girl_ReactionFlag = true;			// 女の子がリアクションする
				m_girl_hitReactionFlag = true;		// 女の子がHITした時のリアクションをする
			}
			else if (m_target[m_targetCount]->GetPosX() <= -80)
			{
				m_girl_ReactionFlag = true;			// 女の子がリアクションする
				m_girl_missReactionFlag = true;		// 女の子がmissした時のリアクションをする
			}
		}
		m_target[m_targetCount]->Reaction(m_hit_ui[m_targetCount], m_iceHitFlagBuffer);

		m_player->Update(_deltaTime);

		m_camera->Update(*m_player);



		//if (CheckHitKey(KEY_INPUT_LSHIFT) && !KeyPush)
		//{
		//	m_finishFlag = TRUE;
		//}
		//if (m_finishFlag == TRUE)
		//{
		//	m_fadeOutFlag = true;
		//}
		//if (m_fadeOutFinishFlag)
		//{
		//	// scoreUIのスコアをResultのscore変数にセット
		//	return new ResultHalf();				//	リザルトシーンに切り替える
		//}

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
			return new ResultHalf();                //    リザルトシーンに切り替える
		}
		break;
	default:
		break;
	}
	return this;						//	ゲームシーンを表示し続ける
}


void TestSceneSudo::Draw()
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

	m_ui->Draw();


	if (m_finishSoundFlag)
{
		PlaySoundMem(m_endSoundHandle, DX_PLAYTYPE_BACK);
		ChangeVolumeSoundMem(m_volumePal + 20, m_iceSoundHandle);

		DrawExtendFormatString(SCREEN_SIZE_W / 2 - GetFontSize(), SCREEN_SIZE_H / 2, 4.0, 4.0, GetColor(0, 0, 0), "終了！");

		m_soundCount = GetNowCount() / 1000;

		if (m_soundCount > 1000)
		{
			m_fadeOutFlag = true;
			//m_finishSoundFlag = false;
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
			DrawGraph(0, 0, m_backGraphHandle, FALSE);
			DrawGraph(0, 0, m_finishGraphHandle, TRUE);
			DrawGraph(0, m_girl_Y, m_girlGraphHandle, TRUE);
			DrawGraph(0, m_lady_Y, m_ladyGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;

	}


}

void TestSceneSudo::Sound()
{
	//	ゲーム終了時に効果音を流す
	//if (m_target[enemyNum - 1]->GetIceState() == Target_State::END_SHOT)
	//{
	//	StopSoundMem(m_soundHandle);
	//	PlaySoundMem(m_finishSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	//	ChangeVolumeSoundMem(m_volumePal + GONG_VOLUME_PAL, m_finishSoundHandle);
	//}

	//練習BGMの再生
		PlaySoundMem(m_soundHandle, DX_PLAYTYPE_LOOP, FALSE);
		ChangeVolumeSoundMem(m_volumePal+50, m_soundHandle);
	
}

void TestSceneSudo::Load()
{
	//	グラフィックハンドルにセット
	m_finishGraphHandle = LoadGraph("data/img/gameEnd.png");
	m_backGraphHandle = LoadGraph("data/img/gameBack.png");
	m_soundHandle = LoadSoundMem("data/sound/Game/rensyuu.mp3");
	m_finishSoundHandle = LoadSoundMem("data/sound/gameEnd.wav");
	m_girlGraphHandle = LoadGraph("data/img/chinaGirl.png");
	m_girl_missReaction_GraphHandle = LoadGraph("data/img/chinaGirl_aseri(01).png");	//  女の子の反応の画像ハンドルをロード
	m_girl_hitReaction_GraphHandle = LoadGraph("data/img/chinaGirl_iine.png");			//  女の子の反応の画像ハンドルをロード
	m_ladyGraphHandle = LoadGraph("data/img/chinaLady.png");
	m_manualGraphHandle = LoadGraph("data/img/manual.png");
	m_timingImgHandle = LoadGraph("data/img/gameScene/timing.png");

	//	モデルハンドルにセット
	m_poolModelHandle = MV1LoadModel("data/model/stage/stage2/poolModel2.pmx");
	//	サウンドハンドルにセット
	m_iceSoundHandle = LoadSoundMem("data/sound/throwIce.mp3");
	m_hitSoundHandle = LoadSoundMem("data/sound/hitIce.mp3");
	m_missSoundHandle = LoadSoundMem("data/sound/missIce.mp3");
	m_doorSoundHandle = LoadSoundMem("data/sound/door.ogg");
	m_endSoundHandle = LoadSoundMem("data/sound/Game/finish.mp3");//練習のメニュー分の距離を達成した時に流れる効果音

	int scoreHandle = LoadGraph("data/model/score_ui/score(1).png");
	m_player = new Player;			//	プレイヤークラスのインスタンスを生成
	m_camera = new Camera;			//	カメラクラスのインスタンスを生成
	//m_mark = new Mark;				//	マーククラスのインスタンスを生成
	for (int i = 0; i < (enemyNum + 1); i++)
	{
		m_target[i] = new Target;
		m_target[i]->SetInterval(TARGET_SHOT_INTERVAL);
		m_target[i]->SetAccel(targetSpeed);
		m_target[i]->SetThrowSound(m_iceSoundHandle);
		m_target[i]->SetHitSound(m_hitSoundHandle);
		m_target[i]->SetMissSound(m_missSoundHandle);
		
	}

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			m_ui = new UI();
			m_hit_ui[j + (i * 5)] = new UI();
		}
	}
	// UIクラスのprivateメンバ変数に画像ハンドルをロード
	m_ui->Load();

	m_effect = new PlayEffect("data/effects/FeatherBomb.efk", 5.0f);
}

void TestSceneSudo::DebugKey()
{
	// 確認用
	if (CheckHitKey(KEY_INPUT_A))
	{
		if (m_hitCount < enemyNum)
		{
			m_hitFlag = true;
			m_hitCount++;
		}
	}
	if (CheckHitKey(KEY_INPUT_B))
	{
		if (m_hitCount < enemyNum)
		{
			m_hitFlag = false;
			m_hitCount++;
		}
	}
	if (m_iceThrowFlag)
	{
		DrawString(1800, 980, "発射", GetColor(255, 255, 255));
	}
	else
	{
		DrawString(1800, 980, "未発射", GetColor(255, 255, 255));
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
