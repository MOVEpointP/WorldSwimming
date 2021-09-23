#ifndef _RESULT_H_
#define _RESULT_H_

#include "SceneBase.h"
#include "UI.h"

#include "DxLib.h"

class Result : public SceneBase
{
public:
	Result(int playerRanking);			//	コンストラクタ
	~Result();							//　デストラクタ

	void UpdateTransparent();

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;				//	描画
	void Sound()override;				//	音楽
	void Load()override;				//	初期化
private:
	int m_score;						//	スコア格納変数
	int m_playerRanking;				//	プレイヤーのランキング格納変数
	int m_evaluation;
	int m_backgroundGraphHandle;		//	背景のグラフィックハンドル
	int m_guidanceGraphHandle;			//	ロゴのグラフィックハンドル
	int m_evaluationGraphHandle[3];		//	評価のグラフィックハンドル

	int m_transpVal;				// 透過量
	int m_transVal_Enter;				//	フェードアウト中の透過量
	int permeationAmount;// 毎透過量
	int transParent;// 透過量

	int m_permeationAmount;			// 毎フレーム透過量
	int m_click_sound_handle;			//	ENTERで進むときのサウンドハンドル
	int m_bgmSoundHandle;				//	BGMのサウンドハンドル
	int m_evaluationSoundHandle[3];		//	評価のサウンドハンドル
	int m_checkResultFlag = 0;				//	場面管理用フラグ
	bool m_checkKeyFlag;				//	キーが押されたままかを判定するフラグ
	bool m_fadeInFinishFlag;			//	フェードインの終了判定フラグ
	bool m_fadeOutFlag;					//	フェードアウト開始用のフラグ
	bool m_fadeOutFinishFlag;			//	フェードアウトの終了判定フラグ
	int m_time;

	enum rank
	{
		C,		//0
		B,		//1
		A,		//2
		S 		//3
	};
};

#endif // _RESULT_H_

