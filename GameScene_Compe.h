#include "SceneBase.h"
#include "DxLib.h"
#define PI    3.1415926535897932384626433832795f

class GameSceneCompe :public SceneBase
{
public:
	enum class GAME_SCENE_STATE
	{
		COUNTDOWN,
		GAME
	};
	GameSceneCompe();				//	コンストラクタ
	~GameSceneCompe();			//	デストラクタ
	bool KeyPush;			//キーを押したかどうか
	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化
	void DebugKey();

	static void SetTurnFlag(int _turnFlag) { m_turnFlag = _turnFlag; }

private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Camera* m_camera;			//	カメラクラスへのポインタメンバ変数
	class NPC* m_npc;                 //  NPCのポインタメンバ変数

	GAME_SCENE_STATE m_state;
	int m_startTime;				//	ゲームの開始時間
	int m_soundHandle;				//	ゲーム画面・サウンドハンドル
	int m_countryGraphHandle;		//国表示画像ハンドル
	int m_turnGraphHandle;					//ターンした時の50m表記用のハンドル
	int m_goalGraphHandle;
	int m_poolModelHandle;			//プールのモデルハンドル
	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_finishFlag;				//	ゲーム終了判定フラグ
	bool m_fadeInFinishFlag;		//	フェードインの終了判定フラグ
	bool m_fadeOutFlag;				//	フェードアウト開始用のフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ
	bool m_MaxGorl;					//全員がゴールしたか判定するフラグ
	int m_playerRanking;			// プレイヤーのゴールした順位
	bool m_countryDrawFlag;				//国旗を表示するか判定するフラグ
	int m_timeplayer;                     //プレイヤーが泳いだ時間を格納する変数
	bool m_timeFlag;
	int m_countryTime;				//国を表示する時間
	bool m_countryFadeFlag;				//国を表示する時間
	int m_countryTopX;				//国の画像での、左上のｘ座標
	int m_countryUnderX;				//国の画像での、右下のｘ座標
	int m_timeFlame;				//タイム表示用のフレーム
	static int m_turnFlag;
	int m_spaceDiveGraph;
	bool m_spacePushFlag;
	bool m_spaceDivePushGraph;
	int m_liveGraph;

}; 
