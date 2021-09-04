#pragma once
#include "DxLib.h"

// スコアのUIクラス
class UI final
{
public:
	// コンストラクタ
	UI();
	// デストラクタ
	~UI();
	// ロード
	void UI::Load();
	// 更新
	// 引数にエネミーからscoreとターゲットカウントを持ってくる
	void UI::ScoreUpdate(int _playerPos);
	// 描画
	void UI::Draw();
	// getter
	int GetScore() { return m_score; }

private:
	// 操作説明画像ハンドル
	static int m_help_graphHandle;
	// スコア画像ハンドル
	static int  m_score_graphHandle;
	// 当たった時の画像ハンドル
	static int m_hit_graphHandle;
	// 外れた時の画像ハンドル
	static int m_miss_graphHandle;

	int m_gaugeImgHandle;
	int m_boyImgHandle;

	// スコア保存用変数
	static int m_score;

	// ポインターの座標
	int m_posX;
	int m_posY;

	// ハンドル格納変数
	int m_handle;
};