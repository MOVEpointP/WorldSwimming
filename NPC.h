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

	NPC();				                    // コンストラクタ.
	~NPC();				                    // デストラクタ.

	bool KeyPush;			                //キーを押したかどうか

	void Update(float _deltaTime);			// 更新.
	void Draw();			                // 描画

	// 外部からm_RankCountを加算するときに使用する
	void addRankCount();
	// プレイヤーのランキングを更新するための変数を返す
	int GetRankCount() { return m_RankCount; }
	// ゴールをしたNPCの人数
	int m_RankCount;
private:
	int		m_modelHandle[3][4];			//	モデルハンドル  [NPCの人数] [モーション]
	int		m_NPCState;
	VECTOR	pos[3];							// ポジション.
	VECTOR	velocity[3];					// 移動力.
	VECTOR	dir[3];							// 回転方向
	bool GorlFlag[3];						//それぞれのNPCがゴールしたか判定するフラグ
	
	float   m_motionSpeed;					//　モデルのモーションスピード
	float TotalTime[3], PlayTime;			//モーションの再生時間
	int AttachIndex;						//アニメーションの総再生時間

	class Player* m_player;					//	プレイヤークラスへのポインタメンバ変数

	// 静的定数.
	static const float ACCEL;
};

#endif // _NPC_H_