#include "SceneBase.h"
#include "DxLib.h"

class SceneChange :public SceneBase
{
public:
	SceneChange();				//	コンストラクタ
	~SceneChange();				//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化
	void DebugKey();
	
	void FeedIn();
	void FeedOut();

private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Target* m_target[11];			//
	class Camera* m_camera;			//	カメラクラスへのポインタメンバ変数
	class Mark* m_mark;				//	マーククラスへのポインタメンバ変数
	class UI* m_score_ui[10];		//  UIクラスへのポインタメンバ変数
	class UI* m_hit_ui[10];			//	ヒット判定UIクラスへのポインタメンバ変数
	class PlayEffect* m_effect;     //  エフェクトプレーヤー

};

