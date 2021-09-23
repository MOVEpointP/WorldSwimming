#ifndef _SCENEMGR_H_
#define _SCENEMGR_H_

#include "DxLib.h"

class SceneMgr
{
public:
	SceneMgr();			//	コンストラクタ
	~SceneMgr();		//	デストラクタ

	void Update(float _deltaTime);		//	更新
	void Draw();						//	描画
	void Sound();						//　音楽

	void SetScene(class SceneBase* _Scene);		//	シーンをセットする				

private:
	class SceneBase* m_scene;					//	シーンベースへのポインタメンバ変数
};

#endif // _SCENEMGR_H_
