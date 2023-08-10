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
	/*******************
	*�ϐ��錾
	*******************/

	T_Object Block[HEIGHT][WIDTH];     //�u���b�N�I�u�W�F�N�g�f�[�^
	T_CURSOR Select[3];
	int Item[ITEM_MAX];
	int ClickStatus;
	int Stage_State;
	int State_Mission;
	int Stage_Score;
	int ClearFlag;

	int BlockImage[BLOCK_IMAGE_MAX];      //�u���b�N�摜
	int StageImage;                       //�w�i�p�摜
	int ClickSE;                          //�N���b�NSE
	int FadeOutSE;                         //�t�F�[�h�A�E�gSE
	int MoveBlockSE;                      //�u���b�N�ړ�SE

	/*************************
	*�v���g�^�C�v�錾
	*************************/

	int combo_check(int x, int y);
	void combo_check_h(int y, int x, int* cnt, int* col);
	void combo_check_w(int y, int x, int* cnt, int* col);
	void save_block(void);
	void restore_block(void);

	/***********************************
	*�X�e�[�W����@�\�F����������
	*���@���F�Ȃ�
	*�߂�l�F�G���[���
	***********************************/

	int StageInitialize(void)
	{
		int ret = 0;
		int i;

		//�摜�ǂݍ���
		LoadDivGraph("images/block.png", BLOCK_IMAGE_MAX, BLOCK_IMAGE_MAX, 1,
			BLOCKSIZE, BLOCKSIZE, BlockImage);
		StageImage = LoadGraph("images/stage.png");

		//�����ǂݍ���
		ClickSE = LoadSoundMem("sounds/click_se.mp3");
		FadeOutSE = LoadSoundMem("sounds/fadeout_se.mp3");
		MoveBlockSE = LoadSoundMem("sounds/moveblock_se.mp3");

		//�u���b�N��������
		CreateBlock();

		ClickStatus = E_NONE;
		Stage_State = 0;
		Stage_Score = 0;
		ClearFlag = FALSE;

		for (i = 0; i < 3; i++)
		{
			Select[i].x = 0;
			Select[i].y = 0;
		}

		//�G���[�`�F�b�N
		for (i = 0; i < BLOCK_IMAGE_MAX; i++)
		{
			if (BlockImage[i] == -1)
			{
				ret = -1;
				break;
			}
		}
		if (StageImage == -1)
		{
			ret = -1;
		}
		if (ClickSE == -1)
		{
			ret = -1;
		}
		if (FadeOutSE == -1)
		{
			ret = -1;
		}
		if (MoveBlockSE == -1)
		{
			ret = -1;
		}

		return ret;
	}

	/*************************************
	*�X�e�[�W����@�\�F�X�e�[�W�̕`��
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	*************************************/

	void StageDraw(void) {
		DrawGraph(0, 0, StageImage, FALSE);

		//�A�C�e���̎擾����`��
		for (int i = 0; i < ITEM_MAX; i++)
		{
			DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
			DrawFormatString(580, 235 + i * 30, 0xffffff, "%3d", Item[i]);

		}

		//�u���b�N��`��
		for (int i = 0; i < HEIGHT; i++)
		{
			for (int j = 0; j < WIDTH; j++)
			{
				if (Block[i][j].flg == TRUE && Block[i][j].image != NULL)
				{
					DrawGraph(Block[i][j].x, Block[i][j].y,
						BlockImage[Block[i][j].image], TRUE);
				}
			}
		}

		//�I���u���b�N��`��
		DrawGraph(Select[SELECT_CURSOR].x * BLOCKSIZE, Select[SELECT_CURSOR].y *
			BLOCKSIZE, BlockImage[9], TRUE);
		if (ClickStatus != E_NONE)
		{
			DrawGraph(Select[NEXT_CURSOR].x * BLOCKSIZE,
				Select[NEXT_CURSOR].y * BLOCKSIZE, BlockImage[9], TRUE);
		}

		//�~�b�V������`��
		SetFontSize(20);
		DrawFormatString(590, 211, GetColor(255, 255, 255), "%3d", Stage_Mission);

		//�A�C�e���̎擾����`��
		for (int i = 0; i < ITEM_MAX; i++)
		{
			DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
			DrawFormatString(580, 235 + i * 30, GetColor(255, 255, 255), "%3d", Item[i]);
		}
	}

	/***********************************
	*�X�e�[�W����@�\�F�u���b�N��������
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	************************************/

	void CreateBlock(void)
	{
		int Check = 0;
		int i, j;

		do
		{
			Check = 0;
			for (i = 0; i < HEIGHT; i++)
			{
				for (j = 0; j < WIDTH; j++)
				{
					if (j == 0 || j == WIDHT - 1 || i == HEIGHT - 1 || I == 0)
					{
						Block[i][j].flg = FALSE;
						Block[i][j].image = NULL;
					}
					else
					{
						Block[i][j].flg = TRUE;
						Block[i][j].x = (j - 1) * BLOCKSIZE;
						Block[i][j].y = (i - 1) * BLOCKSIZE;
						Block[i][j].width = BLOCKSIZE;
						Block[i][j].height = BLOCKSIZE;
						Block[i][j].image = GetRand(7) + 1;//1�`8�̗���

					}
				}
			}

			/*for(i=1;i<HEIGHT-1;i++)
			{
				for(j=1;j<WIDHT-1;j++)
				  {
					  if(Block[i][j].image==NULL)
					  {
					  Block[i][j].image=GetRand(7)+1;
					  }
			}

		}*/
		//�u���b�N�A���`�F�b�N
			for (i = 1; i < HEIGHT - 1; i++)
			{
				for (j = 1; j < WIDTH - 1; j++)
				{
					for (j = 1; j < WIDTH - 1; j++)
					{
						Check += combo_check(i, j);
					}
				}
			}while (Check != 0);

			for (i = 0; i < ITEM_MAX; i++)
			{
				Item[i] = 0;
			}
		}

		/***************************************
		*�X�e�[�W����@�\�G�u���b�N�I������
		*���@���F�Ȃ�
		*�߂�l�F�Ȃ�
		****************************************/

		void SelectBlock(void)
		{
			int TmpBlock;
			int Result;

			//�J�[�\�����W�̎擾
			Select[SELECT_CURSOR].x = GetMousePositionX() / BLOCKSIZE;
			Select[SELECT_CURSOR].y = GetMousePositionY() / BLOCKSIZE;

			//�I���u���b�N�͈̔͂𐧌�
			if (Select[SELECT_CURSOR].x < 0)
			{
				Select[SELECT_CURSOR].x = 0;
			}
			if (Select[SELECT_CURSOR].x > WIDTH - 3)
			{
				Select[SELECT_CURSOR].x = WIDTH - 3;
			}
			if (Select[SELECT_CURSOR].y < 0)
			{
				Select[SELECT_CURSOR].y = 0;
			}
			if (Select[SELECT_CURSOR].y > HEIGHT - 3)
			{
				Select[SELECT_CURSOR].y = HEIGHT - 3;
			}

			//�N���b�N�Ńu���b�N��I��
			if (GetKeyFlg(MOUSE_INPUT_LEFT)) {
				//�N���b�N���ʉ�
				PlaySoundMem(ClickSE, DX_PLAYTYPE_BACK);

				if (ClickStatus == E_NONE) {
					Select[NEXT_CURSOR].x = Select[SELECT_CURSOR].x;
					Select[NEXT_CURSOR].y = Select[SELECT_CURSOR].y;
					ClickStatus = E_ONCE; 4
				}
				else if (ClickStatus == E_ONCE &&
					((abs(Select[NEXT_CURSOR].x - Select[SELECT_CURSOR].x)
						== 1 &&
						(abs(Select[NEXT_CURSOR].y - Select[SELECT_CURSOR].y)
							== 0)) ||
						(abs(Select[NEXT_CURSOR].x - Select[SELECT_CURSOR].x)
							== 0 &&
							abs(Select[NEXT_CURSOR].y - Select[SELECT_CURSOR].y) ==
							1)))
				{
					Select[TMP_CURSOR].x = Select[SELECT_CURSOR].x;
					Select[TMP_CURSOR].y = Select[SELECT_CURSOR].y;
					lickStatus = E_SECOND;
				}
			}

			//�I���u���b�N����������B
			if (ClickStatus == E_SECOND)
			{
				TMPBlock = Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image;
				Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image =
					Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
				Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = TmpBlock;

				//�A����3�ȏォ���ׂ�B
				Result = 0;
				Result += combo_check(Select[NEXT_CURSOR].y + 1, Select[NEXT_CURSOR].x + 1);
				Result += combo_check(Select[TMP_CURSOR].y + 1, Select[TMP_CURSOR].x + 1);

				//�A����3�����Ȃ�I���u���b�N�����ɖ߂�
				if (Result == 0)
				{
					int TmpBlock = Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image;
					Block[Select[NEXT_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
					Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = TmpBlock;
				}
				else
				{
					//�A�����R�ȏ�Ȃ�u���b�N�������u���b�N�ړ������ֈڍs����
					Stage_State = 1;
				}

				//���ɃN���b�N�ł���悤��ClockFlag��0�ɂ���
				ClickStatus = E_NONE;
			}
		}

		/**************************************
		*�X�e�[�W����@�\�F�t�F�[�h�A�E�g����
		*���@���F�Ȃ�
		*�߂�l�F�Ȃ�
		**************************************/
		void FadeOutBlock(void)
		{
			static int BlendMode = 255;
			int i, j;

			//�t�F�[�h�A�E�g���ʉ�
			if (CheckSoundMem(FadeOutSE) == 0)
			{
				PlaySoundMem(FadeOutSE, DX_PALYTYPE_BACK);
			}

			//�`�惂�[�h���A���t�@�u�����h�ɂ���
			SetDrawBlendMode(DX_BLENDGRAPHTYPE_ALPHA, BlendMode);
			for (i = 1; i < HEIGHT - 1; i++)
			{
				for (j = 1; j < WIDTH - 1; j++)
				{
					if (Block[i][j].image == 0)
					{
						DrawGraph(Block[i][j].x, Block[i][j].y, BlockImage[Block[i][j].backup], TRUE);
					}
				}
			}

			//�`�惂�[�h���m�[�u�����h�ɂ���
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			BlendMode -= 5;

			if (BlendMode == 0)
			{
				BlendMode = 255;
				Stage_State = 2;
				StopSoundMem(FadeOutSE);
			}
		}
		/*************************************
		*�X�e�[�W����@�\�F�u���b�N�ړ�����
		*���@���F�Ȃ�
		*�߂�l�F�Ȃ�
		*************************************/
		void MoveBlock(void)
		{
			int i, j, k;
			//�u���b�N�ړ����ʉ�
			PlaySoundMem(MoveBlockSE, DX_PLAYTYPE_BACK);

			//���ֈړ����鏈��
			for (i = 1; i < HEIGHT - 1; i++)
			{
				for (j = 1; j < WIDTH - 1; j++)
				{
					if (Block[i][j].image == 0)
					{
						for (k = i; k > 0; k--)
						{
							Block[k][j].image = Block[k - 1][j].image;
							Block[k - 1][j].image = 0;
						}
					}
				}
			}


			//��̃u���b�N�𐶐����鏈��
			for (i = 1; i < HEIGHT - 1; i++)
			{
				for (j = 1; j < WIDHT - 1; j++)
				{
					if (Block[i][j].image == 0)
					{
						Block[i][j].image = GetRand(7) + 1;
					}
				}
			}

			//�A���`�F�b�N�ֈڍs����
			Stage_State = 3;
		}

		/***********************************
		*�X�e�[�W����@�\�F�A���`�F�b�N����
		*�� ���F�Ȃ�
		*�߂�l�F�Ȃ�
		*************************************/

		void CheckBlock(void)
		{
			int Result = 0;
			int i, j;

			//�u���b�N�A���`�F�b�N
			for (i = 1; i < HEIGHT - 1; j++)
			{
				Result += combo_check(i, j);
			}
		}

		//�A�����Ȃ��Ȃ�΃u���b�N�I����
		//�����łȂ���΃u���b�N�ړ��ֈڍs���ĘA���`�F�b�N���p������
		if (Result == 0)
		{
			//�N���A�`�F�b�N�����ֈڍs����B
			Stage_State = 4;
		}
		else
		{
			//�A����3�ȏ�Ȃ�u���b�N�������u���b�N�ړ������ֈڍs����
			Stage_State = 1;
		}
	}

	/******************************************************
	*�X�e�[�W����@�\�F�N���A�����`�F�b�N����
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	*���@�l�F�N���A�����t���O��0�Ƃ��A�e�X�N�[���̍폜�u���b�N��
	�@�@�@�@�@�@���x�������������Ȃ�������`�F�b�N�����𒆒f���ăQ�[���𑱍s����B
	********************************************************/

	void CheckClear(void)
	{
		int i;
		for (i = 0; i < ITEM_MAX; i++)
		{
			if (Item[i] >= Stage_MIssion)
			{
				ClearFlag = TRUE;
				break;
			}
		}
		if (ClearFlag != TRUE)
		{
			Stage_State = 0;
		}

	}

	/************************************************
	*�X�e�[�W����@�\�F�X�e�[�W�X�e�[�^�X���擾����
	*���@���F�Ȃ�
	*�߂�l�F�X�e�[�W�̃X�e�[�^�X���
	************************************************/

	int Get_StageState(void)
	{
		return Stage_State;
	}

	/***********************************************
	*�X�e�[�W����@�\�F�~�b�V�������擾����
	*���@���F�Ȃ�
	*�߂�l�F�~�b�V�������N���A���Ă��邩
	************************************************/
	int Get_StageClearFlag(void)
	{
		return ClearFlag;
	}

	/***********************************************
	*�X�e�[�W����@�\�F�~�b�V�������擾����
	*���@���F�Ȃ�
	*�߂�l�F�~�b�V�������N���A���Ă��邩
	************************************************/
	int Get_StageScore(void)
	{
		return Stage_Score;
	}

	/***********************************************
	*�X�e�[�W����@�\�F�~�b�V�������擾����
	*���@���F���~�b�V�����ɕK�v�Ȑ��l
	*�߂�l�F�Ȃ�
	************************************************/
	void Set_StageMission(int mission)
	{
		Stage_Mission += mission;
	}

	/***********************************
	*�X�e�[�W����@�\�F�A���`�F�b�N����
	*�����P�F�u���b�NY�}�X
	*�����Q�F�u���b�NX�}�X
	*�߂�l�F�A���L���i0:�����@1:�L��j
	***********************************/
	int combo_check(int y, int x)
	{
		int ret = FALSE;

		//�c�����̃`�F�b�N
		int CountH = 0;
		int ColorH = 0;
		save_block();
		combo_check_h(y, x, &CountH, &ColorH);
		if (CountH < 3)
		{
			restore_block();  //�R�����Ȃ�߂�
		}

		//�c�����̃`�F�b�N
		int CountW = 0;
		int ColorW = 0;
		save_block();
		combo_check_w(y, x, &CountW, &ColorW);
		if (CountW < 3)
		{
			restore_block();
		}

		//�R�ȏ�ŕ���ł��邩�H
		if ((CountH >= 3 || CountW >= 3))
		{
			if (CountH >= 3)
			{
				Item[ColorH - 1] += CountH;
				Stage_Score += CountH * 10;
			}
			if (CountW >= 3)
			{
				Item[ColorW - 1] += CountW;
				Stage_Score += CountW * 10;
			}
			ret = TRUE;
		}

		return ret;
	}
	/******************************************
	*�X�e�[�W����@�\�F�A���`�F�b�N�����i�c�����j
	*���@���F�Ȃ�
	*�߂�l���A���L���i0:���� 1:�L��j
	*******************************************/
	void combo_check_h(int y, int x, int* cnt, int* col)
	{
		int Color = 0;
		//�Ώ��̃u���b�N���O�g�̏ꍇ��return�ŏ����𔲂���
		if (Block[y][x].image == 0)
		{
			return;
		}
		*col = Block[y][x].image;
		Color = Block[y][x].image;
		Block[y][x].image = 0;
		(*cnt)++;

		if (Block[y + 1][x].image == Color)
		{
			combo_check_h(y + 1, x, cnt, col);
		}
		if (Block[y - 1][x].image == Color)
		{
			combo_check_h(y - 1, x, cnt, col);
		}
	}
	/**********************************************
	*�X�e�[�W����@�\�F�A���`�F�b�N�����i�������j
	*���@���F�Ȃ�
	*�߂�l�F�A���L���i0:���� 1:�L��j
	**********************************************/
	void combo_check_w(int y, int x, int* cnt, int* col)
	{

		int Color = 0;
		//�Ώۃu���b�N���O�g�̏ꍇreturn�ŏ����𔲂���
		if (Block[y][x].image == 0)
		{
			return;
		}

		*col = Block[y][x].image;
		Color = Block[y][x].image;   //�F�擾
		Block[y][x].image = 0;
		(*cnt)++;

		if (Block[y][x + 1].image == Color)
		{
			combo_check_w(y, x + 1, cnt, col);
		}
		if (Block[y][x - 1].image == Color)
		{
			combo_check_w(y, x - 1, cnt, col);
		}
	}

	/*******************************************
	*�X�e�[�W����@�\�F�u���b�N���̕ۑ�����
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	*******************************************/

	void save_block(void)
	{
		int i, j;

		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				Block[i][j].backup = Block[i][j].image;
			}
		}
	}

	/*******************************************
	*�X�e�[�W����@�\�F�u���b�N����߂�����
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	*******************************************/
	void restore_block(void)
	{
		int i, j;

		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				Block[i][j].image = Block[i][j].backup;
			}
		}
	}

#include"SceneManager.h"
#include"TitleScene.h"
#include"GameMainScene.h"
#include"GameClearScene.h"
#include"GameOverScene.h"

	/**********************
	*�}�N����`
	**********************/

	/*********************
	*�^��`
	*********************/

	/*******************
	*�ϐ��錾
	*******************/

	GAME_MODE Game_Mode;       //�Q�[�����[�h���i���݁j
	GAME_MODE Next_Mode;       // �Q�[�����[�h���i���j

	/*********************
	*�v���g�^�C�v�錾
	*********************/

	/****************************************
	*�V�[���Ǘ��@�\�F����������
	* ���@���F�Q�[�����[�h���
	* �߂�l�F�Ȃ�
	****************************************/
	int SceneManager_Initialize(GAME_MODE mode)
	{
		int Read_Error;

		//�V�[���ǂݍ��ݏ���
		//�^�C�g�����
		Read_Error == TitleScene_Initialize();
		if (Read_Error == D_ERROR)
		{
			return D_ERROR;
		}

		//�Q�[�����C�����
		Read_Error = GameMainScene_Initialize();
		if (Read_Error == D_ERROR)
		{
			return D_ERROR;
		}

		//�Q�[���N���A���
		Read_Error = GameClearScene_Initialize();
		if (Read_Error == D_ERROR)
		{
			return D_ERROR;
		}

		//�Q�[���I�[�o�[���
		Read_Error = GameOverScene_Initialize();
		if (Read_Error == D_ERROR)
		{
			return D_ERROR;
		}

		Game_Mode = mode;
		Next_Mode = Game_Mode;

		return Read_Error;
	}

	/*************************************
	*�V�[���Ǘ��@�\�F�X�V����
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	**************************************/
	void SceneManager_Updete(void)
	{
		//�O�t���[���ƃQ�[�����[�h������Ă�����V�[����؂�ւ���
		if (Game_Mode != Next_Mode)
		{
			SceneManager_Initialize(Next_Mode);
		}

		//�e��ʂ̍X�V����
		switch (Game_Mode)
		{
			case E_TITLE;
				TitleScene_Update();
				break;
				case E_GAMEMAIN;
					GameMainScene_Updete();
					break;
					case E_GAME_CLEAR;
						GameClearScene_Update();
						break;
						case E_GAME_OVER;
							GameOverScene_Update();
							break;
							default;
							break;
		}
	}

	/**********************************
	*�V�[���Ǘ��@�\�F�`�揈��
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	**********************************/
	void SceneManager_Draw(void)
	{
		//�e��ʂ̕`�揈��
		switch (Game_Mode)
		{
			case E_TITLE;
				TitleScene_Draw();
				break;
				case E_GAMEMAIN;
					GameMainScene_Draw();
					break;
					case E_GAME_CLEAR;
						GameClearScene_Draw();
						break;
						default;
						break;
		}
	}

	/*********************************
	*�V�[���Ǘ��@�\�F�V�[���ؑ֏���
	*���@���F�ύX����Q�[�����[�h
	*�߂�l�F�Ȃ�
	********************************/
	void Change_Scene(GAME_MODE mode)
	{
		Nest_Mode = mode;
	}

#include"DxLib.h"
#include"InputControl.h"

	/****************************
	*�ϐ��錾
	****************************/
	int old_button;
	int now_button;
	int mouse_pusition_x;
	int mouse_position_y;

	/***************************
	*�v���g�^�C�v�錾
	***************************/

	/***************************************
	*���͐���@�\�F����������
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	***************************************/
	void Input_Initialize(void)
	{
		old_button = NULL;
		now_botton = NULL;
		mouse_position_x = NULL;
		mouse_position_y = NULL;
	}

	/**************************************
	*���͐���@�\�F�X�V����
	*���@���F�Ȃ�
	*�߂�l�F�Ȃ�
	**************************************/
	void Input_Update(void)
	{
		//�}�E�X���͏��̎擾
		old_button = now_botton;
		now_button = GetMouseInput();

		//�}�E�X�J�[�\�����W�̎擾
		GetMousePoint(&mouse_position_, &mouse_position_y);

		]

		/*******************************************
		*���͐���@�\�FESC�L�[���̓`�F�b�N
		*���@���F�Ȃ�
		*�߂�l�FTRUE�i���͂��ꂽ�j�AFALSE�i�����́j
		********************************************/
		int Input_Escape(void)
		{
			int ret = FALSE;

			//ESC�L�[�������ꂽ�烋�[�v���甲����
			if (CheckHitKey(KEY_INPUT_ESCAPE))
			{
				ret = TRUE;

			}
			return ret;
		}

		/*******************************************
		*���͐���@�\�F���͏��擾�����i�������u�ԁj
		*���@���F�w�肷��}�E�X�̃{�^��
		*�߂�l�FTRUE�i���͂��ꂽ�j�AFALSE�i�����́j
		*******************************************/
		int GetOldKey(int key)
		{
			int ret = FALSE;

			if ((key & old_button) != FALSE)
			{
				ret = TRUE;
			}
			return ret;
		}

		/******************************************
		*���͐���@�\�F���͏��擾�����i�����Ă���j
		*���@���F�w�肷��}�E�X�̃{�^��
		*�߂�l�FTRUE�i���͂��ꂽ�j�AFALSE(�����́j
		*******************************************/
		int GetNowKey(int key)
		{
			int ret = FALSE;

			if ((key & now_button) != FALSE)
			{
				ret = TRUE;
			}
			return ret;
		}

		/*******************************************
		*���͐���@�\�F���͏��擾�����i�������u�ԁj
		*���@���F�w�肷��}�E�X�̃{�^��
		*�߂�l�FTRUE�i���͂��ꂽ�j�AFALSE�i�����́j
		*******************************************/
		int GetKeyFlg(int key)
		{
			int ret = FALSE;
			int keyflg = now_button & �`old_button;

			if ((key & keyflg) != FALSE)
			{
				ret = TRUE;
			}
			return ret;
		}

		/************************************
		*���͐���@�\�}�E�X���W�擾
		*���@���F�Ȃ�
		*�߂�l�F�}�E�X�J�[�\��X���W���
		************************************/
		int GetMousePositionX(void)
		{
			return mouse_position_x;
		}

		/************************************
		*���͐���@�\�}�E�X���W�擾
		*���@���F�Ȃ�
		*�߂�l�F�}�E�X�J�[�\��Y���W���
		************************************/
		int GetMousePositionY(void)
		{
			return mouse_position_y;
		}


		
