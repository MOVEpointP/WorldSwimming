#include "SceneBase.h"
#include "DxLib.h"

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
private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Camera* m_camera;			//	カメラクラスへのポインタメンバ変数
	class NPC* m_npc;                 //  NPCのポインタメンバ変数

	GAME_SCENE_STATE m_state;
	int m_startTime;				//	ゲームの開始時間
	int m_soundHandle;				//	ゲーム画面・サウンドハンドル
	int m_countryGraphHandle;		//国表示画像ハンドル
	int m_poolModelHandle;			//プールのモデルハンドル
	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_finishFlag;				//	ゲーム終了判定フラグ
	bool m_fadeInFinishFlag;		//	フェードインの終了判定フラグ
	bool m_fadeOutFlag;				//	フェードアウト開始用のフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ
	bool m_MaxGorl;					//全員がゴールしたか判定するフラグ
	int m_playerRanking;			// プレイヤーのゴールした順位
	bool m_countrydrawFlag;				//国旗を表示するか判定するフラグ
	int m_timeplayer;                     //プレイヤーが泳いだ時間を格納する変数
	bool m_timeFlag;
}; 
