#pragma once

class Combo
{
private:
	static int m_combo;
	static int m_tenCombo;
	static int m_maxCombo;

public:
	Combo();														//コンストラクタ
	~Combo();														//デストラクタ
	static void AddCombo(int _setCombo) { m_combo += _setCombo; }	//ゲームシーンからスコアをもらう
	static int GetCombo() { return m_combo; }						//スコアをリザルトに渡す

	static void AddTenCombo(int _setTenCombo) { m_tenCombo += _setTenCombo; }
	static int GetTenCombo() { return m_tenCombo; }

	static int MaxCombo();
};