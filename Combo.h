#pragma once

class Combo
{
private:
	static int m_combo;
	static int m_tenCombo;

public:
	Combo();														//コンストラクタ
	~Combo();														//デストラクタ
	static void AddCombo(int _setCombo) { m_combo += _setCombo; }	//ゲームシーンからスコアをもらう
	static int GetCombo() { return m_combo; }						//スコアをリザルトに渡す

	static void AddTenCombo(int _setTenCombo) { m_tenCombo += _setTenCombo; }
	static int GetTenCombo() { return m_tenCombo; }
	static void ResetCombo(int _resetCombo) { m_combo = _resetCombo; }
	static void ResetTenCombo(int _resetCombo) { m_tenCombo = _resetCombo; }

};