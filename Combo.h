#pragma once

class Combo
{
private:
	static int m_combo;

public:
	Combo();														//コンストラクタ
	static void SetCombo(int _setCombo) { m_combo += _setCombo; }	//ゲームシーンからスコアをもらう
	static int GetCombo() { return m_combo; }						//スコアをリザルトに渡す

};
