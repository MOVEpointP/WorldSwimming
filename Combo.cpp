#include "Combo.h"			

//スタティックのメンバ変数の初期化
int Combo::m_combo = 0;
int Combo::m_tenCombo = 0;
int Combo::m_maxCombo = 0;
int Combo::m_tmpMaxCombo=0;

Combo::Combo()
{
}

Combo::~Combo()
{
}

//最大コンボ数を格納する
int Combo::MaxCombo()
{
	m_tmpMaxCombo = m_tenCombo * 10 + m_combo;

	if (m_maxCombo < m_tmpMaxCombo)
	{
		m_maxCombo = m_tmpMaxCombo;
	}
	return m_maxCombo;
}
