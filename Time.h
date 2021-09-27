#pragma once
class Time
{
private:
	static int m_time;

public:
	Time();														//コンストラクタ
	static void SetTime(int _setTime) { m_time = _setTime; }	//ゲームシーンからスコアをもらう
	static int GetTime() { return m_time; }						//スコアをリザルトに渡す

};
