#pragma once

#include "SceneBase.h"
#include "DxLib.h"

class TestSceneSudo :public SceneBase
{
public:

	enum class GAME_SCENE_STATE
	{
		COUNTDOWN,
		GAME
	};

	TestSceneSudo();				//	コンストラクタ
	~TestSceneSudo();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Target* m_target[25];		//  ターゲットを格納する配列
	class Camera* m_camera;			//	カメラクラスへのポインタメンバ変数
	GAME_SCENE_STATE m_state;		//  ゲームシーンのステータスを入れる

	int m_targetCount;				//	アイスの飛ばした個数
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
};