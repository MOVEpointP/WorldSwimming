#include "UI.h"


// ScoreUIコンストラクタ
UI::UI()
{
}


// 描画
void UI::Draw()
{
	//現在地を示す
	//DrawGraph(1300, -100, m_boyImgHandle, TRUE);
	DrawExtendGraph(1300, 30, 1300+200, 30+110, m_boyImgHandle, TRUE);
	DrawGraph(1300, -150, m_gaugeImgHandle, TRUE);
}
