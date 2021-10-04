#pragma once

#include "SceneBase.h"
#include "DxLib.h"

class TestSceneNakamura :public SceneBase
{
public:

	enum class GAME_SCENE_STATE
	{
		COUNTDOWN,
		GAME
	};

	TestSceneNakamura();				//	コンストラクタ
	~TestSceneNakamura();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化
	void DebugKey();

	int GetTargetCount() { return m_targetCount; }

private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Target* m_target[25];		//  ターゲットを格納する配列
	class Camera* m_camera;			//	カメラクラスへのポインタメンバ変数
	GAME_SCENE_STATE m_state;		//  ゲームシーンのステータスを入れる

	int m_targetCount;				//	アイコンの飛ばした個数
	int m_startTime;				//	ゲームの開始時間

	int m_soundHandle;				//	ゲーム画面・サウンドハンドル
	int m_endSoundHandle;			//　ゴール時のサウンドハンドル
	int m_timingImgHandle;			// 　判定バーの画像ハンドル
	int m_poolModelHandle;			//　プールのモデルハンドル

	bool m_finishFlag;				//	ゲーム終了判定フラグ
	bool m_fadeOutFlag;				//	フェードアウト開始用のフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ

	bool m_finishSoundFlag;			//	ゴール時のサウンド判定フラグ
	int  m_finishFadeCount;			//	ゴール時のフェードアウト用フレームカウント
	int  m_targetSpeed;				//	ターゲットの速度

	int m_tensPlaceScore;
	int m_onePlaceScore;
	int m_scoreHandle;
	int m_mapChipHandle[10];

	int m_rankBHandle;
	int m_rankAHandle;
	int m_rankHandle;//スコアの効果音
	int m_rankSoundFlag[3];
	int m_rankSound;

	int m_spaceHandle;
	int m_enterHandle;
	bool m_enterPushFlag;
	bool m_spacePushFlag;
	int m_spacePushHandle;
	int m_enterPushHandle;
	int m_diveSpaceHandle;
	int m_diveSpacePushHandle;
	int m_diveSpacePushFlag;
	int m_endHandle;
};