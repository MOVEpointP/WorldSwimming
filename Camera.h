//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"

class Player;

class Camera
{
public:
	Camera();							// コンストラクタ.
	~Camera();							// デストラクタ.

	void Update(const Player& player);	// 更新.

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }

	// プレイヤーの泳ぎ状態のgetter/setter.
	void SetPlayerState(int _playerState) { m_playerState = _playerState; }

private:

	VECTOR	pos;						// ポジション.
	int m_playerState;					// プレイヤーの泳ぎ状態
	float m_cameraPosX;
	float m_cameraPosY;
	float m_cameraPosZ;
};

#endif // _CAMERA_H_