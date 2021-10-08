#include "Combo.h"			

//スタティックのメンバ変数の初期化
int Combo::m_combo = 0;
int Combo::m_tenCombo = 0;
int Combo::m_maxCombo = 0;

Combo::Combo()
{
}

Combo::~Combo()
{
}

//最大コンボ数を格納する
int Combo::MaxCombo()
{
	if (m_maxCombo < m_combo)
	{
		m_maxCombo = m_combo;
	}
	return m_maxCombo;
}
