#pragma once
#include "DxLib.h"

// スコアのUIクラス
class UI final
{
public:
	// コンストラクタ
	UI();
	// 描画
	void UI::Draw();
	
private:

	int m_gaugeImgHandle;
	int m_boyImgHandle;

};