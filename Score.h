#pragma once
#include "SceneBase.h"
class Score :public SceneBase
{
private:
	static int m_score;
	int m_rank;			
public:
	Score();//コンストラクタ
	static void SetScore(int _setScore) { m_score = _setScore; }//ゲームシーンからスコアをもらう
	static int GetScore() { return m_score; }			 //スコアをリザルトに渡す
	void Draw() override;
	void Sound() override;
	void Load()	 override;
	SceneBase* Update(float _deltaTime);


};

