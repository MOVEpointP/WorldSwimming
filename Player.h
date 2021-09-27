//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "DxLib.h"

#define DIVE	0
#define SWIM	1
#define TURN	2
#define RESULT	3

class ObstructBase;
class NPC;

class Player final
{
public:

	Player();				// コンストラクタ.
	~Player();				// デストラクタ.

	bool KeyPush;			//キーを押したかどうか

	void Update(float _deltaTime);			// 更新.
	void Draw();			// 描画.


	// 衝突処理.
	void OnHitObstruct(ObstructBase& obstruct);

	// モデルハンドルの取得.
	int GetModelHandle(){ return m_modelHandle[m_playerState]; }

	// モーション状態の取得
	const int GetPlayerState() const { return m_playerState; }

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// ディレクションのgetter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }

	//セッター
	void SetScene(bool _sceneFlag) { m_moveFlag = _sceneFlag; }//ゲームシーンからシーンフラグをもらう

	// プレイヤーのランキングを示す変数をプレイヤーのランキング保存変数に入れる
	void SetPlayerRankingNum(const int playerRankingCount) { PlayerRank = playerRankingCount; }

	// 現在のプレイヤーのランキングを取得するgetter
	int GetPlayerNowRankingNum() { return PlayerRank; }

	// ゴールしたかどうかを返すgetter
	bool GetGoalFlag() { return GoalFlag; }

	bool ResultSceneFlag;         //　往復数によるゲーム終了判定フラグ

	bool GoalFlag;	//プレイヤーがゴールしたときはtrue、ゴールしてないときはfalseになる


private:

	int		m_modelHandle[4];			//	モデルハンドル
	int		m_diveModelHandle;		//	ダイブモデルハンドル.
	int		m_swimModelHandle;		//　泳ぎモデルハンドル.
	int		m_resultModelHandle;	//	ガイナ立ちモデルハンドル.
	int		m_playerState;
	int     m_modeCount;			// 往復した回数
	bool    m_moveFlag;
	int		m_trainingMaxCount;			//　練習での往復最大数
	float   m_motionSpeed;				//　モデルのモーションスピード
	VECTOR	pos;			// ポジション.
	VECTOR	velocity;		// 移動力.
	VECTOR	dir;			// 回転方向.
	VECTOR  playerDir;
	float	hitRadius;		// あたり判定の半径.
	float TotalTime[3], PlayTime;//モーションの再生時間
	int AttachIndex;//モーションの
	int m_speedDisplay;		//倍速表示の矢印を動かす
	bool m_moveAnimFlag;//モーションを一時停止するフラグ
	int m_moveCount;
	int m_RoundTrip;//往復数の画像
	int m_mapchipHandle[10];//マップチップの画像
	//　プレイヤーのランキングの順位を保存する変数
	int PlayerRank;

	// player用サウンド変数
	static int m_sHandle;

	// 静的定数.
	static const float ACCEL;
	static const float MAX_SPEED;
	static const float TRANING_SPEED;
	static const float DEFAULT_DECEL;
	static const float BREAK_DECEL;
	static const float GRIP_DECEL;
	static const float GRIP_POWER;
	static const float COLIDE_DECEL_FAC;
};

#endif // _PLAYER_H_