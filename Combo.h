#pragma once

class Combo
{
private:
	static int m_combo;
	static int m_tenCombo;
	static int m_maxCombo;

public:
	Combo();														//�R���X�g���N�^
	~Combo();														//�f�X�g���N�^
	static void AddCombo(int _setCombo) { m_combo += _setCombo; }	//�Q�[���V�[������X�R�A�����炤
	static int GetCombo() { return m_combo; }						//�X�R�A�����U���g�ɓn��

	static void AddTenCombo(int _setTenCombo) { m_tenCombo += _setTenCombo; }
	static int GetTenCombo() { return m_tenCombo; }

	static int MaxCombo();
};