#include "Score.h"

//スタティックのメンバ変数の初期化
int Score::m_score = 0;			 
int Score::m_rank = 0;			 


const int B_P = 10;				 //ランクBのスコア
const int A_P = 20;				//ランクAのスコア
const int S_P = 30;				//ランクSのスコア


Score::Score()
{
	
}

int Score::SetRank()
{

	if (m_score < B_P)
	{
		m_rank = 0;
	}
	if (m_score <= B_P && A_P > m_score)
	{
		m_rank = 1;
	}
	if (m_score >= A_P && m_score < S_P)
	{
		m_rank = 2;
	}
	if (m_score > S_P)
	{
		m_rank = 3;
	}


	return m_rank;
}
