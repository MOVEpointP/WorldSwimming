#include "UI.h"


// ScoreUI�R���X�g���N�^
UI::UI()
{
}


// �`��
void UI::Draw()
{
	//���ݒn������
	//DrawGraph(1300, -100, m_boyImgHandle, TRUE);
	DrawExtendGraph(1300, 30, 1300+200, 30+110, m_boyImgHandle, TRUE);
	DrawGraph(1300, -150, m_gaugeImgHandle, TRUE);
}
