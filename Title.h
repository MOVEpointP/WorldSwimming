#pragma once
#include "SceneBase.h"


class Title : public SceneBase
{
public:


	Title();					//	コンストラクタ
	~Title();					//	デストラクタ

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();

	//	デルタタイムのセッター
	void SetDeltaTime(float _deltaTime) { m_deltaTime = _deltaTime; }
	//	デルタタイムのゲッター
	float GetDeltaTime() { return m_deltaTime; }
private:
	int m_backGraphHandle;				//	タイトル画面・背景のグラフィックハンドル
	int m_logoGraphHandle;				//	タイトル画面・ロゴのグラフィックハンドル
	int m_guidanceGraphHandle;			//	タイトル画面・ガイドのグラフィックハンドル
	int m_soundHandle;					//	タイトル画面・サウンドハンドル
	int m_water;
	int m_click_sound_handle;			//  ENTERで進むときのサウンドハンドル						
	int m_manualGraphHandle;			//マニュアルのグラフハンドル

	int m_permeationAmount;			// 毎フレーム透過量
	int m_transpVal;				// 透過量
	int m_transVal_Enter;				//	フェードアウト中の透過量
	float m_deltaTime;				//	デルタタイム

	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeInFinishFlag;		//	フェードインの終了判定フラグ
	bool m_fadeOutFlag;				//	フェードアウト開始用のフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ

	float m_guidanceY;               //	タイトルロゴのy座標
	float m_TitleLogox;               //	タイトルロゴのx座標
	bool m_TitleLogoxFlag;
	bool m_guidanceYFlag;
	float m_Waterx;               //	水画像のx座標
	bool m_WaterxFlag;
	bool m_gameSceneFlag;
};