#pragma once

class Score 
{
private:
	static int m_score;
	int m_rank;			

public:
	Score();//�R���X�g���N�^
	static void SetScore(int _setScore) { m_score += _setScore; }//�Q�[���V�[������X�R�A�����炤
	static int GetScore() { return m_score; }			 //�X�R�A�����U���g�ɓn��
	
};

