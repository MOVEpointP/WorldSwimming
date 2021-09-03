#pragma once
#include "SceneBase.h"
class Score :public SceneBase
{
private:
	static int m_score;
	int m_rank;
public:
	Score();//�R���X�g���N�^
	static void SetScore(int _setScore) { m_score = _setScore; }//�Q�[���V�[������X�R�A�����炤
	static int GetScore() { return m_score; }			 //���U���g�ɓn��
	void Draw() override;//�I����̓o�[�`�������Ɠ`����
	void Sound() override;
	void Load()	 override;
	SceneBase* Update(float _deltaTime);//�I�f���^�^�C���Ńt���[�����[�g���J�E���g���Ăǂ̃p�\�R���ł����������Ɍ�������


};

