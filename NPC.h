//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _NPC_H_
#define _NPC_H_

#define NPC_NUMBER 7

#include "DxLib.h"

#define DIVE		0		//本番最初のカメラ
#define SWIM		1		//練習本編
#define TURN		2       //ターン
#define RESULT 		3		//リザルト
#define COMPE_FIRST	4		//練習中ののダイブ
#define COMPE_DIVE  5		//本番中のダイブ
#define COMPE_SWIM  6		//本番のSWIM

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

	static void SetDiveFlag(int _diveFlag) { m_diveFlag = _diveFlag; }
	// ゴールをしたNPCの人数
private:
	int		m_modelHandle[NPC_NUMBER][7];			//	モデルハンドル  [NPCの人数] [モーション]
	int		m_NPCState;
	float   m_motionSpeed[NPC_NUMBER];					//　モデルのモーションスピード
	VECTOR	pos[NPC_NUMBER];							// ポジション.
	VECTOR	velocity[NPC_NUMBER];					// 移動力.
	VECTOR	dir[NPC_NUMBER];							// 回転方向.
	VECTOR  NPCDir[NPC_NUMBER];
	float TotalTime[NPC_NUMBER], PlayTime[NPC_NUMBER];			//モーションの再生時間
	int AttachIndex;						//モーションの
	class Player* m_player;					//プレイヤークラスへのポインタメンバ変数
	// 静的定数.
	static const float ACCEL;
	bool m_goalFlag[NPC_NUMBER];
	int m_rankcount;
	float m_npcGoalAccel[NPC_NUMBER];					// NPCのスピード調整
	int		m_npcState;
	int m_startTime;
	static bool m_diveFlag;
	bool m_measureDistanceFlag;

	//水しぶきのエフェクト
	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;
	VECTOR m_efkPos;
	bool efkFlag;
	int m_efkTime;
	int m_efkstartTime;
	float m_npcScoreSpeed[NPC_NUMBER];
	float m_npcMotionAccelTmp[NPC_NUMBER + 10];
	float m_npcMptionAccel[NPC_NUMBER];

};

#endif //_NPC_H_