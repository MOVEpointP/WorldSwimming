#pragma once

class Score 
{
private:
	static int m_score;
	int m_rank;			

public:
	Score();//コンストラクタ
	static void SetScore(int _setScore) { m_score += _setScore; }//ゲームシーンからスコアをもらう
	static int GetScore() { return m_score; }			 //スコアをリザルトに渡す
	
};

