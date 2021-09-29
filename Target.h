#ifndef _TARGET_H_
#define _TARGET_H_

#include "DxLib.h"
#include "Score.h"

class TestSceneKoga;
class PlayEffect;
class Player;
class UI;

enum Target_State
{
	NO_SHOT, NOW_SHOT, END_SHOT
};

class Target
{
public:
	Target();				// コンストラクタ.
	~Target();				// デストラクタ.

	void Update(float _deltaTime);			            // 更新.
	void Draw();			                            // 描画.
	void Reaction(Target* _target, bool _hitFlag);		// リアクション.


	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }
	void SetSinglePosX() { m_posX = pos.x; }	                                   //ターゲットの座標をセット
	static void SetTargetSpeedX(int _targetSpeed) { m_targetSpeed = _targetSpeed; }//スピードをセット

	// ディレクションのgetter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// 加速(accelVec)のSetter
	void SetAccel(float _accel) { m_target_accel = _accel; }

	void SetTargetCount(int _targetCount) { m_targetCount = _targetCount; }
	void SetInterval(int _interval) { m_shotInterval = _interval; }

	Target_State GetIceState() { return m_targetState; }
	void SetIceState(Target_State _iceState) { m_targetState = _iceState; }


	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }


private:
	int		m_handImgHandle;	// 手の画像ハンドル
	int		m_legImgHandle;		// 足の画像ハンドル
	int		m_o2ImgHandle;		// 酸素画像ハンドル
	int		m_FontHandle;		// フォントハンドル

	VECTOR	pos;				// ターゲットのポジション.
	int     m_posX;             //ターゲットのｘ座標

	VECTOR	velocity;		    // 移動力.
	VECTOR	accelVec;           // 加速度

	VECTOR	dir;			    // 回転方向.

	int		m_targetCount;		// ターゲットの番号
	int		m_plusX;			// ターゲットのx座標

	float m_target_accel;		// ターゲットの加速度

	//	静的定数
	static const int m_target_X;		// ターゲットの初期x座標
	static const int m_target_Y;		// ターゲットの初期y座標
	static const int m_target_Z;		// ターゲットの初期z座標
	static const int m_score_good;		//　グッド判定の際に入るスコア
	static const int m_score_bad;		//　バッド判定の際に入るスコア
	static const int m_score_perfect;	//　パーフェクト判定の際に入るスコア
	static const int m_before_good;		//	グッド判定（前）
	static const int m_perfect;     	//　パーフェクト判定
	static const int m_after_good;		//　グッド判定（後）
	static const int m_final_good;		//　グッド判定（最後）
	
	Target_State m_targetState;

	int  m_targerJadgeWord;		//タイミングの判定文字
	int  m_timeCount;				//判定表記を表示する時間をカウントする
	int  m_targetScore;			//スコアを格納する変数
	int  m_combo;				//コンボ数を格納する変数
	static int  m_targetSpeed;	//それぞれのターゲットの流れる速度を決める変数
	int     m_shotInterval;     // エネミーを飛ばす間隔

	float	hitRadius;		// あたり判定の半径.
	int m_goodHandle;
	int m_badHandle;
	int m_perfectHandle;//判定の表示をする画像
	int m_comboHandle;//コンボの表記
	int m_mapchipHandle[10];
	int m_tens_place;
};

#endif // _TARGET_H_