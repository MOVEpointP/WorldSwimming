#pragma once

class Combo
{
private:
	static int m_combo;

public:
	Combo();														//�R���X�g���N�^
	static void SetCombo(int _setCombo) { m_combo += _setCombo; }	//�Q�[���V�[������X�R�A�����炤
	static int GetCombo() { return m_combo; }						//�X�R�A�����U���g�ɓn��

};
