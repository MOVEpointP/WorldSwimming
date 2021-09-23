#include "Fade.h"
#include "Dxlib.h"


const int FADE_SPEED = 3;


Fade::Fade()
{
}

//フェードイン関数。フェードインしたい画像があればハンドルを入れる。なければfalseを入れる
void Fade::FadeIn(int _fadeOutFinishFlag,int _graphHandle)
{
	for (int i = 0; i < 255; i += FADE_SPEED)
	{
		// 描画輝度をセット
		SetDrawBright(i, i, i);

		DrawGraph(0, 0, _graphHandle, TRUE);

		// グラフィックを描画
		ScreenFlip();
	}
}

//フェードアウト関数。フェードアウトしたい画像があればハンドルを入れる。なければfalseを入れる
void Fade::FadeOut(int _fadeOutFinishFlag, int _graphHandle)
{
	for (int i = 0; i < 255; i += FADE_SPEED)
	{
		// 描画輝度をセット
		SetDrawBright(255 - i, 255 - i, 255 - i);

		DrawGraph(0, 0, _graphHandle, TRUE);

		// グラフィックを描画
		ScreenFlip();
	}

}
