
//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Camera.h"

#define USE_LERP_CAMERA 1

VECTOR Camera::dir = VGet(0, 0, 0);

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Camera::Camera()
	:m_cameraPosX(0.0f)
	, m_cameraPosY(0.0f)
	, m_cameraPosZ(0.0f)
	, m_playerState(0)
{
	//奥行0.1～1000までをカメラの描画範囲とする
	// ３D空間に何かを描画する際に、カメラからどれだけ離れたところ（Near）から、
	// どこまで（Far）のものを描画するかを設定する(FOV)
	SetCameraNearFar(0.75f, 450.0f);

	// Vgetはベクトルの取得
	// 原点の値をセット
	pos = VGet(0, 0, 0);

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Camera::~Camera()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Camera::Update(const Player& player)
{
#if !USE_LERP_CAMERA
	// z軸上で、プレイヤーから一定距離離れた状態でプレイヤーを常に見続けるよう位置調整
	//pos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f);

#else
	// lerpを使用して実装.
	// lerp(VECTOR a, VECTOR b, float t)は
	// answer = a + ((b-a) * t)
	VECTOR aimPos = VAdd(/*player.GetPos()*/VGet(0,0,0), VScale(/*player.GetDir()*/VGet(0,0,-1), 0.0f));

	// カメラの見る位置を少し上にする
	aimPos = VAdd(aimPos, VGet(0, 0, 0));

	VECTOR posToAim = VSub(aimPos, pos);

	VECTOR scaledPosToAim = VScale(posToAim, 0.1f);

	pos = VAdd(pos, scaledPosToAim);

#endif

	// カメラに位置を反映.
	switch (player.GetPlayerState())
	{

	case DIVE:

		SetCameraPositionAndTarget_UpVecY(VGet(100, 50, player.GetPos().z - 25), /*player.GetPos()*/VGet(0, -10, player.GetPos().z + 50));
		break;


	case SWIM:

		if (player.GetPos().z >= 225)
		{
			SetCameraPositionAndTarget_UpVecY(VGet(-100, 70, 275), /*player.GetPos()*/VGet(0, -10, 275));

		}
		else
		{
			SetCameraPositionAndTarget_UpVecY(VGet(-100, 70, player.GetPos().z + 50), /*player.GetPos()*/VGet(0, -10, player.GetPos().z + 50));
		}

		break;


	case COMPE_FIRST:

	
		m_cameraPosX -= 0.3f;

		SetCameraPositionAndTarget_UpVecY(VGet(m_cameraPosX+50, 70, player.GetPos().z - 35), /*player.GetPos()*/VGet(m_cameraPosX + 50, -10, player.GetPos().z + 50));

		Player::SetCameraPos(m_cameraPosX);
		break;

	case COMPE_DIVE:

 			SetCameraPositionAndTarget_UpVecY(VGet(-100, 70, player.GetPos().z + 50), /*player.GetPos()*/VGet(0, -10, player.GetPos().z + 50));

		break;


	case COMPE_SWIM:

		if (player.GetPos().z >= 225)
		{
			SetCameraPositionAndTarget_UpVecY(VGet(-100, 70, 275), /*player.GetPos()*/VGet(0, -10, 275));
			player.SetEfkDir(VGet(0, 1.5, 0));

		}
		else if (player.GetPos().z >= 225/2)
		{
			SetCameraPositionAndTarget_UpVecY(VGet(0, 50, player.GetPos().z + 90), /*player.GetPos()*/VGet(0, 20, player.GetPos().z + 50));
			player.SetEfkDir(VGet(0, 2.5, 0));
		}
		else
		{
			SetCameraPositionAndTarget_UpVecY(VGet(-100, 70, player.GetPos().z + 50), /*player.GetPos()*/VGet(0, -10, player.GetPos().z + 50));
			player.SetEfkDir(VGet(0, 1.5, 0));
		}
		break;

	}


}
