#include"TitleScene.h"
#include"DxLib.h"
#include"InputControl.h"
#include"SceneManager.h"

/**********************************
*�}�N����`
**********************************/

/**********************************
*�^��`
**********************************/

/**********************************
*�ϐ��錾
**********************************/

int TitleImage;
int TitleBGM;

/*********************************
*�v���g�^�C�v�錾
**********************************/

/*********************************
*�^�C�g����ʁF����������
*���@���F�Ȃ�
*�߂�l�F�G���[���
**********************************/
int TitleScene_Initialize(void)
{
	int ret = 0;

	//�摜�ǂݍ��ݏ���
	TitleImage = LoadGraph("images/title.png");
	//�����ǂݍ��ݏ���
	TitleBGM = LoadSoundMem("sounds/title_bgm.mp3");

	if (TitleImage == D_ERROR)
	{
		ret = D_ERROR;
	}
	if (TitleBGM == D_ERROR)
	{
		ret = D_ERROR;
	}

	return ret;

}

/*****************************
*�^�C�g����ʁG�X�V����
*���@���F�Ȃ�
*�߂�l�F�Ȃ�
*****************************/

void TitleScene_Update(void)
{
	//�^�C�g��BGM
	if (CheckSoundMem(TitleBGM) == 0)
	{
		PlaySoundMem(TitleBGM, DX_PLAYTYPE_BACK);

	}

	if (GetKeyFlg(MOUSE_INPUT_LEFT) == TRUE)
	{
			if (GetMousePositionX() > 120 && GetMousePositionX() < 290 &&
			GetMousePositionY() > 260 && GetMousePositionY() < 315)
				�o
				Change_Scene(E_GAMEMAIN);

	            StopSoundMem(TitleBGM);
                 }
if (GetMousepositionX() > 120 && GetMousePositionX() < 220 &&
	GetMousePositionY() > 345 && GetMousePositionY() < 400)
     {
	Change_Scene(E_END);
     }
    }
}

	/*********************
	*�^�C�g����ʁF�`�揈��
	*�� ���F�Ȃ�
	*�߂�l�F�Ȃ�
	*********************/

	void TitleScene_Draw(void)
	{
		//�^�C�g����ʂ�\��
		DrawGraph(0, 0, TitleImage, FALSE);
	}

#include"Stage.h"
#include"DxLib.h"
#include"InputControl.h"

/***************************
*�}�N����`
***************************/

#define HEIGHT           (12)  //�u���b�N�ݒu�T�C�Y�i�����j
#define WIDHT            (12)  //�u���b�N�ݒu�T�C�Y�i���j
#define BLOCKSIZE        (48)  //�u���b�N�T�C�Y
#define BLOCK_IMAGE_MAX  (10)  //�u���b�N�摜��

#define ITEM_MAX         (8)  //�A�C�e���ő吔

#define SELECT_CURSOR    (8)
#define NEXT_CURSOR      (1)
#define TMP_CURSOR       (2)

/************************
*�^��`
************************/

	typedef struct
	{
		int flg;
		int x, y;
		int width, height;
		int image;
		int backup;
	}T_Object;

	typedef struct
	{
		int x;
		int y;
	}T_CURSOR;

	enum
	{
		E_NONE,
		E_ONCE,
		E_SECOND
	};





		
