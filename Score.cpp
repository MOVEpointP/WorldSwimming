#include "Score.h"
int Score::m_score = 0;	//スタティックのメンバ変数の初期化

Score::Score()
	:m_rank(0)
{
	
}


void Score::Draw()
{
	
}

void Score::Sound()
{
}

void Score::Load()
{
}

SceneBase* Score::Update(float _deltaTime)
{
	return this;
}
