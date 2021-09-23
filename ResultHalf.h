
#ifndef _RESULTHALF_H_
#define _RESULTHALF_H_

#include "SceneBase.h"
#include "UI.h"

#include "DxLib.h"
class ResultHalf : public SceneBase
{
	public:
		ResultHalf();			//	コンストラクタ
		~ResultHalf();			//　デストラクタ

		SceneBase* Update(float _deltaTime)override;	//	更新
		void Draw()override;			//	描画
		void Sound()override;			//	音楽
		void Load()override;			//	初期化
		// モデルハンドルの取得.
		int GetModelHandle() { return modelHandle; }
	private:
		class Score* m_half_score;		//　 スコアクラスへのポインタメンバ変数
		int m_score;						//	スコア格納変数
		int m_evaluation;
		int m_logoGraphHandle;				//	ロゴのグラフィックハンドル
		int m_guidanceGraphHandle;			//	ガイドのグラフィックハンドル
		int m_scoreGraphHandle;				//	スコアのグラフィックハンドル
		int m_evaluationGraphHandle[3];		//	評価のグラフィックハンドル
		int m_exitdoorGraphHandle;			//　enter to start のドアのハンドル
		// 毎透過量
		int permeationAmount;
		// 透過量
		int transParent;
		int m_click_sound_handle;			//	ENTERで進むときのサウンドハンドル
		int m_bgmSoundHandle;				//	BGMのサウンドハンドル
		int m_scoreSoundHandle;				//	スコアのサウンドハンドル
		int m_evaluationSoundHandle[3];		//	評価のサウンドハンドル
		int m_checkResultFlag = 0;				//	場面管理用フラグ
		bool m_checkKeyFlag;				//	キーが押されたままかを判定するフラグ
		bool m_fadeInFinishFlag;			//	フェードインの終了判定フラグ
		bool m_fadeOutFlag;					//	フェードアウト開始用のフラグ
		bool m_fadeOutFinishFlag;			//	フェードアウトの終了判定フラグ
		int	modelHandle;	// モデルハンドル.
		VECTOR	pos;			// ポジション.
		VECTOR	dir;			// 回転方向.
		float TotalTime, PlayTime;//モーションの再生時間
		int AttachIndex;//モーションの再生時間
		int m_backgroundGraphHandle;
		enum rank
		{
			C,		//0
			B,		//1
			A,		//2
			S 		//3
		};
};

#endif _RESULTHALF_H_
