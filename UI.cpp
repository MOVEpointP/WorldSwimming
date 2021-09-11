#include "UI.h"

// スゴクカタイアイスの画像サイズ
const int score_pixel = 320;
// 画像の縮小する大きさ
const double score_shrink = 0.4f;
// RotaGraphを使うので真ん中からのスゴクカタイアイスの画像サイズ
const int score_graph_size = score_pixel*score_shrink;

// ハンドルを入れる変数の実体を作る
int UI::m_help_graphHandle;
int UI::m_score_graphHandle;
int UI::m_hit_graphHandle;
int UI::m_miss_graphHandle;
int UI::m_score;

// ScoreUIコンストラクタ
UI::UI()
{
	m_posX = 0;
	m_posY = 0;
}

// ScoreUIデストラクタ
UI::~UI()
{

}

void UI::Load()
{
	// 操作説明画像の読み込み
	m_help_graphHandle = LoadGraph("data/model/score_ui/");
	// スコア画像の読み込み
	m_score_graphHandle = LoadGraph("data/model/score_ui/hit.png");
	// HIT画像の読み込み
	m_hit_graphHandle = LoadGraph("data/model/score_ui/hit.png");
	// MISS画像の読み込み
	m_miss_graphHandle = LoadGraph("data/model/score_ui/miss.png");

	//m_gaugeImgHandle = LoadGraph("data/img/UI/nami.png");		//現在地を表示するためのゲージ
	//m_boyImgHandle = LoadGraph("data/img/UI/boy.png");			//現在地を表す男の子
}

// 更新
// 引数にプレイヤーから座標を持ってくる
//void UI::ScoreUpdate(int _playerPos)
//{
//	// x座標に画像の位置座標を格納する
//	//m_posX = (_playerPos * score_graph_size) + 100;
//
//}

// 描画(仮引数の値で呼び出す画像を変える)
void UI::Draw()
{
	//DrawGraph(1300, -100, m_boyImgHandle, TRUE);
	DrawExtendGraph(1300, 30, 1300+200, 30+110, m_boyImgHandle, TRUE);
	DrawGraph(1300, -150, m_gaugeImgHandle, TRUE);
	
	//DrawGraph(m_posX, m_posY, m_handle, TRUE);
 	DrawRotaGraph(m_posX, m_posY, score_shrink, 0, m_handle, TRUE, FALSE);
}
