//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _NPC_H_
#define _NPC_H_

#include "DxLib.h"

#define DIVE	0
#define SWIM	1
#define TURN	2
#define RESULT	3



class NPC final
{
public:

	NPC();				// コンストラクタ.
	~NPC();				// デストラクタ.

	bool KeyPush;			//キーを押したかどうか

	void Update(float _deltaTime);			// 更新.
	void Draw();			// 描画.


	// モデルハンドルの取得.
	//int GetModelHandle() { return m_modelHandle[m_NPCState]; }

	// モーション状態の取得
	const int GetPlayerState() const { return m_NPCState; }


	// ディレクションのgetter/setter.
	//void SetDir(const VECTOR set) { dir = set; }

	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }

	// 外部からm_RankCountを加算するときに使用する
	void addRankCount();

	// プレイヤーのランキングを更新するための変数を返す
	int GetRankCount() { return m_RankCount; }

	// ゴールをしたNPCの人数
	int m_RankCount;


private:
	int		m_modelHandle[3][4];			//	モデルハンドル  [NPCの人数] [モーション]
	int		m_diveModelHandle;		//	ダイブモデルハンドル.
	int		m_swimModelHandle;		//　泳ぎモデルハンドル.
	int		m_resultModelHandle;	//	ガイナ立ちモデルハンドル.
	int		m_NPCState;
	bool    m_moveFlag;
	float   m_motionSpeed;				//　モデルのモーションスピード
	VECTOR	pos[3];			// ポジション.
	VECTOR	velocity[3];		// 移動力.
	VECTOR	dir[3];			// 回転方向.
	float	hitRadius;		// あたり判定の半径.
	float TotalTime[3], PlayTime;//モーションの再生時間
	int AttachIndex;//モーションの
	int m_speedDisplay;		//倍速表示の矢印を動かす
	bool m_moveAnimFlag;//モーションを一時停止するフラグ
	int m_moveCount;

	// 発射エフェクト
	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数

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

	bool GorlFlag[3];
};

#endif // _PLAYER_H_