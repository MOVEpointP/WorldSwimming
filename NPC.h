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
	// モーション状態の取得
	const int GetPlayerState() const { return m_NPCState; }
	// 外部からm_RankCountを加算するときに使用する
	void addRankCount();
	// プレイヤーのランキングを更新するための変数を返す
	int GetRankCount() { return m_rankcount; }
	// ゴールをしたNPCの人数
private:
	int		m_modelHandle[3][4];			//	モデルハンドル  [NPCの人数] [モーション]
	int		m_NPCState;
	float   m_motionSpeed;					//　モデルのモーションスピード
	VECTOR	pos[3];							// ポジション.
	VECTOR	velocity[3];					// 移動力.
	VECTOR	dir[3];							// 回転方向.
	float TotalTime[3], PlayTime;			//モーションの再生時間
	int AttachIndex;						//モーションの
	class Player* m_player;					//プレイヤークラスへのポインタメンバ変数
	// 静的定数.
	static const float ACCEL;
	bool m_gorlflag[3];
	int m_rankcount;
};

#endif //_NPC_H_