#include "Combo.h"			

//�X�^�e�B�b�N�̃����o�ϐ��̏�����
int Combo::m_combo = 0;
int Combo::m_tenCombo = 0;
int Combo::m_maxCombo = 0;

Combo::Combo()
{
}

Combo::~Combo()
{
}

//�ő�R���{�����i�[����
int Combo::MaxCombo()
{
	if (m_maxCombo < m_combo)
	{
		m_maxCombo = m_combo;
	}
	return m_maxCombo;
}
