#pragma once

class Score 
{
private:
	static int m_score;
	static int m_tenScore;
	static int m_rank;			

public:
	Score();//�R���X�g���N�^
	static void AddScore(int _setScore) { m_score += _setScore; }//�Q�[���V�[������X�R�A�����炤
	static int GetScore() { return m_score; }			 //�X�R�A�����U���g�ɓn��
	static int SetRank();
	
	static void AddTenScore(int _setTenCombo) { m_tenScore += _setTenCombo; }
	static int GetTenScore() { return m_tenScore; }

};

