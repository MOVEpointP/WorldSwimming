#pragma once

class Score 
{
private:
	static int m_score;
	static int m_rank;			

public:
	Score();//コンストラクタ
	static void AddScore(int _setScore) { m_score += _setScore; }//ゲームシーンからスコアをもらう
	static int GetScore() { return m_score; }			 //スコアをリザルトに渡す
	static int SetRank();
	
};

