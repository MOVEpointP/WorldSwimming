#include "Time.h"			

int Time::m_time = 0;	//スタティックのメンバ変数の初期化

Time::Time()
{
}


void Time::calcTime(int& m_oneTime, int& m_tenTime,int& m_decimalTime)
{
	m_decimalTime = m_time % 10;								//小数点第一位
	m_tenTime = m_time / 100;									//十の位
	m_oneTime = (m_time % 100- m_decimalTime)/10;        //一の位

	if (m_time < 0)
	{
		m_decimalTime = 0;
	}

}