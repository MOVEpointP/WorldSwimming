#include "Score.h"

//�X�^�e�B�b�N�̃����o�ϐ��̏�����
int Score::m_score = 0;			 
int Score::m_rank = 0;			 


const int B_P = 10;				 //�����NB�̃X�R�A
const int A_P = 20;				//�����NA�̃X�R�A
const int S_P = 30;				//�����NS�̃X�R�A


Score::Score()
{
	
}

int Score::SetRank()
{
	if (m_score < B_P)//c
	{
		m_rank = 0;
	}
	else if (m_score >= B_P && A_P > m_score)//b
	{
		m_rank = 1;
	}
	else if (m_score >= A_P && m_score < S_P)//a
	{
		m_rank = 2;
	}
	else if (m_score > S_P)//s
	{
		m_rank = 3;
	}


	return m_rank;
}

void Score::calcScore(int &m_oneScore, int &m_tenScore)
{
	m_oneScore = m_score % 10;
	m_tenScore = (m_score - m_oneScore) / 10;
	if (m_score < 0)
	{
		m_oneScore = 0;
	}

}

