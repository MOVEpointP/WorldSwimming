#include "SceneBase.h"
#include "DxLib.h"

class GameSceneCompe :public SceneBase
{
public:

	// �Q�[���̏��
	enum class GAME_SCENE_STATE
	{
		COUNTDOWN,
		GAME
	};

	GameSceneCompe();	//	�R���X�g���N�^
	~GameSceneCompe();	//	�f�X�g���N�^

	bool KeyPush;	//�L�[�����������ǂ���

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;							//	�`��
	void Sound()override;							//	���y
	void Load()override;							//	������
	void DebugKey();

private:
	class Player*		m_player;			//	�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�
	class Camera*		m_camera;			//	�J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Mark*			m_mark;				//	�}�[�N�N���X�ւ̃|�C���^�����o�ϐ�
	class NPC*			m_npc;				//	NPC�̃|�C���^�����o�ϐ�

	GAME_SCENE_STATE m_state;
	//int m_startTime;				//	�Q�[���̊J�n����
	int m_soundHandle;				//	�Q�[����ʁE�T�E���h�n���h��
	int m_countryGraphHandle;		//	���\���摜�n���h��
	int m_poolModelHandle;			//	�v�[���̃��f���n���h��

	bool m_fadeInFinishFlag;		//	�t�F�[�h�C���̏I������t���O
	bool m_fadeOutFlag;				//	�t�F�[�h�A�E�g�J�n�p�̃t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
	bool m_MaxGorl;                 //  �v���C���[��npc���S���S�[�����������肷��t���O

	
	int m_playerRanking;			// �v���C���[�̃S�[����������

	
	int m_liveY;					// ���C�u�̕����𓮂����ϐ�

	bool m_countryDraw;				// ���̍�����\�����邩�̔���
}; 
