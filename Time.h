#pragma once
class Time
{
private:
	static int m_time;

public:
	Time();														//�R���X�g���N�^
	static void SetTime(int _setTime) { m_time = _setTime; }	//�Q�[���V�[������X�R�A�����炤
	static int GetTime() { return m_time; }						//�X�R�A�����U���g�ɓn��
	static void calcTime(int& m_oneTime, int& m_tenTime, int& m_decimalTime);

};
