#include "UI.h"

// �X�S�N�J�^�C�A�C�X�̉摜�T�C�Y
const int score_pixel = 320;
// �摜�̏k������傫��
const double score_shrink = 0.4f;
// RotaGraph���g���̂Ő^�񒆂���̃X�S�N�J�^�C�A�C�X�̉摜�T�C�Y
const int score_graph_size = score_pixel*score_shrink;

// �n���h��������ϐ��̎��̂����
int UI::m_help_graphHandle;
int UI::m_score_graphHandle;
int UI::m_hit_graphHandle;
int UI::m_miss_graphHandle;
int UI::m_score;

// ScoreUI�R���X�g���N�^
UI::UI()
{
	m_posX = 0;
	m_posY = 0;
}

// ScoreUI�f�X�g���N�^
UI::~UI()
{

}

void UI::Load()
{
	// ��������摜�̓ǂݍ���
	m_help_graphHandle = LoadGraph("data/model/score_ui/");
	// �X�R�A�摜�̓ǂݍ���
	m_score_graphHandle = LoadGraph("data/model/score_ui/hit.png");
	// HIT�摜�̓ǂݍ���
	m_hit_graphHandle = LoadGraph("data/model/score_ui/hit.png");
	// MISS�摜�̓ǂݍ���
	m_miss_graphHandle = LoadGraph("data/model/score_ui/miss.png");

	//m_gaugeImgHandle = LoadGraph("data/img/UI/nami.png");		//���ݒn��\�����邽�߂̃Q�[�W
	//m_boyImgHandle = LoadGraph("data/img/UI/boy.png");			//���ݒn��\���j�̎q
}

// �X�V
// �����Ƀv���C���[������W�������Ă���
//void UI::ScoreUpdate(int _playerPos)
//{
//	// x���W�ɉ摜�̈ʒu���W���i�[����
//	//m_posX = (_playerPos * score_graph_size) + 100;
//
//}

// �`��(�������̒l�ŌĂяo���摜��ς���)
void UI::Draw()
{
	//DrawGraph(1300, -100, m_boyImgHandle, TRUE);
	DrawExtendGraph(1300, 30, 1300+200, 30+110, m_boyImgHandle, TRUE);
	DrawGraph(1300, -150, m_gaugeImgHandle, TRUE);
	
	//DrawGraph(m_posX, m_posY, m_handle, TRUE);
 	DrawRotaGraph(m_posX, m_posY, score_shrink, 0, m_handle, TRUE, FALSE);
}
