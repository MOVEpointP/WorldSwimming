#include "Time.h"			

int Time::m_time = 0;	//�X�^�e�B�b�N�̃����o�ϐ��̏�����

Time::Time()
{
}


void Time::calcTime(int& m_oneTime, int& m_tenTime,int& m_decimalTime)
{
	m_decimalTime = m_time % 10;								//�����_����
	m_tenTime = m_time / 100;									//�\�̈�
	m_oneTime = (m_time % 100- m_decimalTime)/10;        //��̈�

	if (m_time < 0)
	{
		m_decimalTime = 0;
	}

}