//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "DxLib.h"

#define DIVE	0
#define SWIM	1
#define RESULT	2


class ObstructBase;

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
	int GetPlayerState() { return m_playerState; }

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// ディレクションのgetter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }

private:
	int		m_modelHandle[3];			//	モデルハンドル
	int		m_diveModelHandle;		//	ダイブモデルハンドル.
	int		m_swimModelHandle;		//　泳ぎモデルハンドル.
	int		m_resultModelHandle;	//	ガイナ立ちモデルハンドル.
	int		m_playerState;
	VECTOR	pos;			// ポジション.
	VECTOR	velocity;		// 移動力.
	VECTOR	dir;			// 回転方向.
	float	hitRadius;		// あたり判定の半径.
	float TotalTime[3], PlayTime;//モーションの再生時間
	int AttachIndex;//モーションの

	// 発射エフェクト
	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;

	// player用サウンド変数
	static int m_sHandle;

	// 静的定数.
	static const float ACCEL;
	static const float MAX_SPEED;
	static const float DEFAULT_DECEL;
	static const float BREAK_DECEL;
	static const float GRIP_DECEL;
	static const float GRIP_POWER;
	static const float COLIDE_DECEL_FAC;
};

#endif // _PLAYER_H_