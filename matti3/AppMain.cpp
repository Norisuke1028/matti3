#include"TitleScene.h"
#include"DxLib.h"
#include"InputControl.h"
#include"SceneManager.h"

/**********************************
*マクロ定義
**********************************/

/**********************************
*型定義
**********************************/

/**********************************
*変数宣言
**********************************/

int TitleImage;
int TitleBGM;

/*********************************
*プロトタイプ宣言
**********************************/

/*********************************
*タイトル画面：初期化処理
*引　数：なし
*戻り値：エラー情報
**********************************/
int TitleScene_Initialize(void)
{
	int ret = 0;

	//画像読み込み処理
	TitleImage = LoadGraph("images/title.png");
	//音源読み込み処理
	TitleBGM = LoadSoundMem("sounds/title_bgm.mp3");

	if(TitleImage == D_ERROR)
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
*タイトル画面；更新処理
*引　数：なし
*戻り値：なし
*****************************/

void TitleScene_Update(void)
{
	//タイトルBGM
	if (CheckSoundMem(TitleBGM) == 0)
	{
		PlaySoundMem(TitleBGM, DX_PLAYTYPE_BACK);

	}

	if (GetKeyFlg(MOUSE_INPUT_LEFT) == TRUE)
	{
		if (GetMousePositionX() > 120 && GetMousePositionX() < 290 &&
			GetMousePositionY() > 260 && GetMousePositionY() < 315)
		{
			Change_Scene(E_GAMEMAIN);

		StopSoundMem(TitleBGM);
	}
	if (GetMousePositionX() > 120 && GetMousePositionX() < 220 &&
		GetMousePositionY() > 345 && GetMousePositionY() < 400)
	{
		Change_Scene(E_END);
	}
}
}

/*********************
*タイトル画面：描画処理
*引 数：なし
*戻り値：なし
*********************/

void TitleScene_Draw(void)
{
	//タイトル画面を表示
	DrawGraph(0, 0, TitleImage, FALSE);
}

#include"Stage.h"
#include"DxLib.h"
#include"InputControl.h"

/***************************
*マクロ定義
***************************/

#define HEIGHT           (12)  //ブロック設置サイズ（高さ）
#define WIDTH            (12)  //ブロック設置サイズ（幅）
#define BLOCKSIZE        (48)  //ブロックサイズ
#define BLOCK_IMAGE_MAX  (10)  //ブロック画像数

#define ITEM_MAX         (8)  //アイテム最大数

#define SELECT_CURSOR    (8)
#define NEXT_CURSOR      (1)
#define TMP_CURSOR       (2)

/************************
*型定義
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
*変数宣言
*******************/

T_Object Block[HEIGHT][WIDTH];     //ブロックオブジェクトデータ
T_CURSOR Select[3];
int Item[ITEM_MAX];
int ClickStatus;
int Stage_State;
int Stage_Mission;
int Stage_Score;
int ClearFlag;

int BlockImage[BLOCK_IMAGE_MAX];      //ブロック画像
int StageImage;                       //背景用画像
int ClickSE;                          //クリックSE
int FadeOutSE;                         //フェードアウトSE
int MoveBlockSE;                      //ブロック移動SE

/*************************
*プロトタイプ宣言
*************************/

int combo_check(int x, int y);
void combo_check_h(int y, int x, int* cnt, int* col);
void combo_check_w(int y, int x, int* cnt, int* col);
void save_block(void);
void restore_block(void);

/***********************************
*ステージ制御機能：初期化処理
*引　数：なし
*戻り値：エラー情報
***********************************/

int StageInitialize(void)
{
	int ret = 0;
	int i;

	//画像読み込み
	LoadDivGraph("images/block.png", BLOCK_IMAGE_MAX, BLOCK_IMAGE_MAX, 1,
		BLOCKSIZE, BLOCKSIZE, BlockImage);
	StageImage = LoadGraph("images/stage.png");

	//音源読み込み
	ClickSE = LoadSoundMem("sounds/click_se.mp3");
	FadeOutSE = LoadSoundMem("sounds/fadeout_se.mp3");
	MoveBlockSE = LoadSoundMem("sounds/moveblock_se.mp3");

	//ブロック生成処理
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

	//エラーチェック
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

void CreateBlock(void)
{
}

/*************************************
*ステージ制御機能：ステージの描画
*引　数：なし
*戻り値：なし
*************************************/

void StageDraw(void) {
	DrawGraph(0, 0, StageImage, FALSE);

	//アイテムの取得個数を描画
	for (int i = 0; i < ITEM_MAX; i++)
	{
		DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
		DrawFormatString(580, 235 + i * 30, 0xffffff, "%3d", Item[i]);

	}

	//ブロックを描画
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

	//選択ブロックを描画
	DrawGraph(Select[SELECT_CURSOR].x * BLOCKSIZE, Select[SELECT_CURSOR].y *
		BLOCKSIZE, BlockImage[9], TRUE);
	if (ClickStatus != E_NONE)
	{
		DrawGraph(Select[NEXT_CURSOR].x * BLOCKSIZE,
			Select[NEXT_CURSOR].y * BLOCKSIZE, BlockImage[9], TRUE);
	}

	//ミッションを描画
	SetFontSize(20);
	DrawFormatString(590, 211, GetColor(255, 255, 255), "%3d", Stage_Mission);

	//アイテムの取得個数を描画
	for (int i = 0; i < ITEM_MAX; i++)
	{
		DrawRotaGraph(540, 245 + i * 30, 0.5f, 0, BlockImage[i + 1], TRUE, 0);
		DrawFormatString(580, 235 + i * 30, GetColor(255, 255, 255), "%3d", Item[i]);
	}
}

/***********************************
*ステージ制御機能：ブロック生成処理
*引　数：なし
*戻り値：なし
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
				if (j == 0 || j == WIDTH - 1 || i == HEIGHT - 1 || I == 0)
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
					Block[i][j].image = GetRand(7) + 1;//1～8の乱数

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
	//ブロック連鎖チェック
		for (i = 1; i < HEIGHT - 1; i++)
		{
			for (j = 1; j < WIDTH - 1; j++)
			{
				for (j = 1; j < WIDTH - 1; j++)
				{
					Check += combo_check(i, j);
				}
			}
		}while(Check != 0);

		for (i = 0; i < ITEM_MAX; i++)
		{
			Item[i] = 0;
		}
	}

	/***************************************
	*ステージ制御機能；ブロック選択処理
	*引　数：なし
	*戻り値：なし
	****************************************/

    void SelectBlock(void)
	{
		int TmpBlock;
		int Result;

		//カーソル座標の取得
		Select[SELECT_CURSOR].x = GetMousePositionX() / BLOCKSIZE;
		Select[SELECT_CURSOR].y = GetMousePositionY() / BLOCKSIZE;

		//選択ブロックの範囲を制御
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

		//クリックでブロックを選択
		if (GetKeyFlg(MOUSE_INPUT_LEFT)) {
			//クリック効果音
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
					(abs(Select[NEXT_CURSOR].x - Select[SELECT_CURSOR].x)== 0 &&
						abs(Select[NEXT_CURSOR].y - Select[SELECT_CURSOR].y) ==1)))
			{
				Select[TMP_CURSOR].x = Select[SELECT_CURSOR].x;
				Select[TMP_CURSOR].y = Select[SELECT_CURSOR].y;
				lickStatus = E_SECOND;
			}
		}

		//選択ブロックを交換する。
		if (ClickStatus == E_SECOND)
		{
			TMPBlock = Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image;
			Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image =
				Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
			Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = TmpBlock;

			//連鎖が3つ以上か調べる。
			Result = 0;
			Result += combo_check(Select[NEXT_CURSOR].y + 1, Select[NEXT_CURSOR].x + 1);
			Result += combo_check(Select[TMP_CURSOR].y + 1, Select[TMP_CURSOR].x + 1);

			//連鎖が3未満なら選択ブロックを元に戻す
			if (Result == 0)
			{
				int TmpBlock = Block[Select[NEXT_CURSOR].y + 1][Select[NEXT_CURSOR].x + 1].image;
				Block[Select[NEXT_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image;
				Block[Select[TMP_CURSOR].y + 1][Select[TMP_CURSOR].x + 1].image = TmpBlock;
			}
			else
			{
				//連鎖が３つ以上ならブロックを消しブロック移動処理へ移行する
				Stage_State = 1;
			}

			//次にクリックできるようにClockFlagを0にする
			ClickStatus = E_NONE;
		}
	}

	/**************************************
	*ステージ制御機能：フェードアウト処理
	*引　数：なし
	*戻り値：なし
	**************************************/
	void FadeOutBlock(void)
	{
		static int BlendMode = 255;
		int i, j;

		//フェードアウト効果音
		if (CheckSoundMem(FadeOutSE) == 0)
		{
			PlaySoundMem(FadeOutSE, DX_PALYTYPE_BACK);
		}

		//描画モードをアルファブレンドにする
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

		//描画モードをノーブレンドにする
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
	*ステージ制御機能：ブロック移動処理
	*引　数：なし
	*戻り値：なし
	*************************************/
	void MoveBlock(void)
	{
		int i, j, k;
		//ブロック移動効果音
		PlaySoundMem(MoveBlockSE, DX_PLAYTYPE_BACK);

		//↓へ移動する処理
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


		//空のブロックを生成する処理
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

		//連鎖チェックへ移行する
		Stage_State = 3;
	}

	/***********************************
	*ステージ制御機能：連鎖チェック処理
	*引 数：なし
	*戻り値：なし
	*************************************/

	void CheckBlock(void)
	{
		int Result = 0;
		int i, j;

		//ブロック連鎖チェック
		for (i = 1; i < HEIGHT - 1; j++)
		{
			Result += combo_check(i, j);
		}
	}

	//連鎖がなくなればブロック選択へ
	//そうでなければブロック移動へ移行して連鎖チェックを継続する
	if (Result == 0)
	{
		//クリアチェック処理へ移行する。
		Stage_State = 4;
	}
	else
	{
		//連鎖が3つ以上ならブロックを消しブロック移動処理へ移行する
		Stage_State = 1;
	}
}

/******************************************************
*ステージ制御機能：クリア条件チェック処理
*引　数：なし
*戻り値：なし
*備　考：クリア条件フラグを0とし、各スクールの削除ブロックが
　　　　　　レベルよりも数が少なかったらチェック処理を中断してゲームを続行する。
********************************************************/

void CheckClear(void)
{
	int i;
	for (i = 0; i < ITEM_MAX; i++)
	{
		if (Item[i] >= Stage_Mission)
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
*ステージ制御機能：ステージステータス情報取得処理
*引　数：なし
*戻り値：ステージのステータス情報
************************************************/

int Get_StageState(void)
{
	return Stage_State;
}

/***********************************************
*ステージ制御機能：ミッション情報取得処理
*引　数：なし
*戻り値：ミッションがクリアしているか
************************************************/
int Get_StageClearFlag(void)
{
	return ClearFlag;
}

/***********************************************
*ステージ制御機能：ミッション情報取得処理
*引　数：なし
*戻り値：ミッションがクリアしているか
************************************************/
int Get_StageScore(void)
{
	return Stage_Score;
}

/***********************************************
*ステージ制御機能：ミッション情報取得処理
*引　数：次ミッションに必要な数値
*戻り値：なし
************************************************/
void Set_StageMission(int mission)
{
	Stage_Mission += mission;
}

/***********************************
*ステージ制御機能：連鎖チェック処理
*引数１：ブロックYマス
*引数２：ブロックXマス
*戻り値：連鎖有無（0:無し　1:有り）
***********************************/
int combo_check(int y, int x)
{
	int ret = FALSE;

	//縦方向のチェック
	int CountH = 0;
	int ColorH = 0;
	save_block();
	combo_check_h(y, x, &CountH, &ColorH);
	if (CountH < 3)
	{
		restore_block();  //３個未満なら戻す
	}

	//縦方向のチェック
	int CountW = 0;
	int ColorW = 0;
	save_block();
	combo_check_w(y, x, &CountW, &ColorW);
	if (CountW < 3)
	{
		restore_block();
	}

	//３つ以上で並んでいるか？
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
*ステージ制御機能：連鎖チェック処理（縦方向）
*引　数：なし
*戻り値＊連鎖有無（0:無し 1:有り）
*******************************************/
void combo_check_h(int y, int x, int* cnt, int* col)
{
	int Color = 0;
	//対処のブロックが外枠の場合はreturnで処理を抜ける
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
*ステージ制御機能：連鎖チェック処理（横方向）
*引　数：なし
*戻り値：連鎖有無（0:無し 1:有り）
**********************************************/
void combo_check_w(int y, int x, int* cnt, int* col)
{

	int Color = 0;
	//対象ブロックが外枠の場合returnで処理を抜ける
	if (Block[y][x].image == 0)
	{
		return;
	}

	*col = Block[y][x].image;
	Color = Block[y][x].image;   //色取得
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
*ステージ制御機能：ブロック情報の保存処理
*引　数：なし
*戻り値：なし
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
*ステージ制御機能：ブロック情報を戻す処理
*引　数：なし
*戻り値：なし
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
*マクロ定義
**********************/

/*********************
*型定義
*********************/

/*******************
*変数宣言
*******************/

GAME_MODE Game_Mode;       //ゲームモード情報（現在）
GAME_MODE Next_Mode;       // ゲームモード情報（次）

/*********************
*プロトタイプ宣言
*********************/

/****************************************
*シーン管理機能：初期化処理
* 引　数：ゲームモード情報
* 戻り値：なし
****************************************/
int SceneManager_Initialize(GAME_MODE mode)
{
	int Read_Error;

	//シーン読み込み処理
	//タイトル画面
	Read_Error == TitleScene_Initialize();
	if (Read_Error == D_ERROR)
	{
		return D_ERROR;
	}

	//ゲームメイン画面
	Read_Error = GameMainScene_Initialize();
	if (Read_Error == D_ERROR)
	{
		return D_ERROR;
	}

	//ゲームクリア画面
	Read_Error = GameClearScene_Initialize();
	if (Read_Error == D_ERROR)
	{
		return D_ERROR;
	}

	//ゲームオーバー画面
	Read_Error = GameOverScene_Initialize();
	if (Read_Error == D_ERROR)
	{
		return D_ERROR;
	}

	Game_Mode = mode;
	Next_Mode = Game_Mode;

	return Read_Error;
}

void SceneManager_Update(void)
{
}

/*************************************
*シーン管理機能：更新処理
*引　数：なし
*戻り値：なし
**************************************/
void SceneManager_Updete(void)
{
	//前フレームとゲームモードが違っていたらシーンを切り替える
	if (Game_Mode != Next_Mode)
	{
		SceneManager_Initialize(Next_Mode);
	}

	//各画面の更新処理
	switch (Game_Mode)
	{
	case E_TITLE:
			TitleScene_Update();
			break;
	case E_GAMEMAIN:
				GameMainScene_Update();
				break;
	case E_GAME_CLEAR:
					GameClearScene_Update();
					break;
	case E_GAME_OVER:
						GameOverScene_Update();
						break;
	default:
						break;
	}
}

/**********************************
*シーン管理機能：描画処理
*引　数：なし
*戻り値：なし
**********************************/
void SceneManager_Draw(void)
{
	//各画面の描画処理
	switch (Game_Mode)
	{
	case E_TITLE:
			TitleScene_Draw();
			break;
	case E_GAMEMAIN:
				GameMainScene_Draw();
				break;
	case E_GAME_CLEAR:
				GameClearScene_Draw();
				break;
	default:
				break;
	}
}

/*********************************
*シーン管理機能：シーン切替処理
*引　数：変更するゲームモード
*戻り値：なし
********************************/
void Change_Scene(GAME_MODE mode)
{
	Next_Mode = mode;
}

#include"DxLib.h"
#include"InputControl.h"

/****************************
*変数宣言
****************************/
int old_button;
int now_button;
int mouse_position_x;
int mouse_position_y;

/***************************
*プロトタイプ宣言
***************************/

/***************************************
*入力制御機能：初期化処理
*引　数：なし
*戻り値：なし
***************************************/
void Input_Initialize(void)
{
	old_button = NULL;
	now_button = NULL;
	mouse_position_x = NULL;
	mouse_position_y = NULL;
}

void Input_Update(void)
{
}

/**************************************
*入力制御機能：更新処理
*引　数：なし
*戻り値：なし
**************************************/
void Input_Update(void)
{
	//マウス入力情報の取得
	old_button = now_button;
	now_button = GetMouseInput();

	//マウスカーソル座標の取得
	GetMousePoint(&mouse_position_x, &mouse_position_y);

}

		/*******************************************
		*入力制御機能：ESCキー入力チェック
		*引　数：なし
		*戻り値：TRUE（入力された）、FALSE（未入力）
		********************************************/
		int Input_Escape(void)
		{
			int ret = FALSE;

			//ESCキーが押されたらループから抜ける
			if (CheckHitKey(KEY_INPUT_ESCAPE))
			{
				ret = TRUE;

			}
			return ret;
		}

		/*******************************************
		*入力制御機能：入力情報取得処理（離した瞬間）
		*引　数：指定するマウスのボタン
		*戻り値：TRUE（入力された）、FALSE（未入力）
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
		*入力制御機能：入力情報取得処理（押している）
		*引　数：指定するマウスのボタン
		*戻り値：TRUE（入力された）、FALSE(未入力）
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
		*入力制御機能：入力情報取得処理（押した瞬間）
		*引　数：指定するマウスのボタン
		*戻り値：TRUE（入力された）、FALSE（未入力）
		*******************************************/
		int GetKeyFlg(int key)
		{
			int ret = FALSE;
			int keyflg = now_button & old_button;

			if ((key & keyflg) != FALSE)
			{
				ret = TRUE;
			}
			return ret;
		}

		/************************************
		*入力制御機能マウス座標取得
		*引　数：なし
		*戻り値：マウスカーソルX座標情報
		************************************/
		int GetMousePositionX(void)
		{
			return mouse_position_x;
		}

		/************************************
		*入力制御機能マウス座標取得
		*引　数：なし
		*戻り値：マウスカーソルY座標情報
		************************************/
		int GetMousePositionY(void)
		{
			return mouse_position_y;
		}

#include"GameOverScene.h"
#include"DxLib.h"
#include"SceneManager.h"

		/******************************
		*変数宣言
		******************************/
		int GameOverImage;
		int GameOverSE;
		int GameOverFlag;

		/********************************
		*プロトタイプ宣言
		********************************/

		/**************************************
		*ゲームオーバー画面：初期化処理
		*引　数：なし
		*戻り値：エラー情報
		**************************************/
		int GameOverScene_Initialize(void)
		{
			int ret = 0;

			//画像読み込み
			GameOverImage = LoadGraph("images/gameover.png");
			//音源読み込み
			GameOverSE = LoadSoundMem("sounds/gameover_se.mp3");
			GameOverFlag = FALSE;

			//エラーチェック
			if (GameOverImage == -1)
			{
				ret = -1;
			}
			if (GameOverSE == -1)
			{
				ret = -1;
			}

			return ret;
		}

		void GameOverScene_Update(void)
		{
		}

		/***************************************************
		*ゲームオーバー画面：更新処理
		*引　数：なし
		*戻り値：なし
		***************************************************/
		void GameOverScene_Updete(void)
		{
			//ゲームオーバー効果音再生チェック
			if (CheckSoundMem(GameOverSE) == 0)
			{
				if (GameOverFlag == TRUE)
				{
					Change_Scene(E_GAME_OVER);
				}
				else
				{
					PlaySoundMem(GameOverSE, DX_PLAYTYPE_BACK);
					GameOverFlag = TRUE;
				}
			}
		}

		/*************************************************
		*ゲームオーバー画面：描画処理
		*引　数：なし
		*戻り値：なし
		*************************************************/
		void GameOverScene_Draw(void)
		{
			//ゲームオーバー画像表示
			DrawGraph(0, 0, GameOverImage, FALSE);
		}

#include"GameMainScene.h"
#include"DxLib.h"
#include"Stage.h"
#include"SceneManager.h"

/*********************************
*マクロ定義
*********************************/

#define TIMELIMIT     (3600*3)  //制限時間
#define NUMBER_IMAGE_MAX (10)   //数字画像数

		/*********************************
		*型定義
		*********************************/

		/*******************************
		*変数宣言
		*******************************/
		int GameScore;
		int GameLevel;
		int GameMission;
		int GameTime;
		int GameCount;    //初期化されないようにするためのカウント
		int ReStartFlag;

		int NumberImage[NUMBER_IMAGE_MAX];   //数字用画像

		/**********************************
		*プロトタイプ宣言
		**********************************/

		/****************************************************
		*ゲームメイン画面：初期化処理
		*引　数：なし
		*戻り値：エラー情報
		****************************************************/
		int GameMainScene_Initialize(void)
		{
			int ret = 0;
			int i;

			//画像読み込み
			LoadDivGraph("images/number.png", NUMBER_IMAGE_MAX,
				NUMBER_IMAGE_MAX, 1, 60, 120, NumberImage);

			//ステージ機能初期化
			ret = StageInitialize();

			//エラーチェック
			for (i = 0; i < NUMBER_IMAGE_MAX; i++)
			{
				if (NumberImage[i] == D_ERROR)
				{
					ret = D_ERROR;
					break;
				}
			}

			//ゲームプレイが初回かどうか？
			if (GameCount == 0)
			{
				GameScore = 0;       //スコアの初期化
				GameLevel = 1;       //ゲームレベルの初期化
				Set_StageMission(3);        //ミッションの初期化
				GameCount++;         //次回の設定
			}
			else
			{
				GameLevel++;          //ゲームレベルの更新
				Set_StageMission(3);     //ミッションを増やす
			}
			GameTime = TIMELIMIT;     //制限時間の初期化

			return ret;
		}

		/**********************************
		*ゲームメイン画面：更新処理
		*引　数：なし
		*戻り値：なし
		**********************************/
		void GameMainScene_Update(void)
		{
			switch (Get_StageState())
			{
			case 0:
					SelectBlock();         //ブロックを選択する。
					break;
			case 2:
					MoveBlock();             //ブロックを移動させる。
					break;
			case 3:
					CheckBlock();        //ブロックの確認
					break;
			case 4:
						CheckClear();     //クリアチェック
						break;
			default:
						break;
			}

			//制限時間の更新
			GameTime--;

			//制限時間がなくなったら、ゲームオーバーに遷移する
			if (GameTime < 0)
			{
				Change_Scene(E_GAME_OVER);
			}

			//ミッションを達成したら、ゲームクリアに遷移する。
			if (Get_StageClearFlag())
			{
				Change_Scene(E_GAME_CLEAR);
			}

		}

		/*********************************
		*ゲームメイン画面：描画処理
		*引　数：なし
		*戻り値：なし
		*********************************/
		void GameMainScene_Draw(void)
		{
			int PosX = 600;
			int tmp_level = GameLevel;
			int tmp_score = Get_StageScore();

			//ステージを描画
			StageDraw();

			//フェードアウト状態か？
			if (Get_StageState() == 1)
			{
				FadeOutBlock();         //フェードアウトする。
			}

			//レベルを描画
			do {
				DrawRotaGraph(PosX, 80, 0.5f, 0, NumberImage[tmp_level % 10], TRUE);
				tmp_level /= 10;
				PosX -= 30;
			} while (tmp_level > 0);

			//スコアの描画
			PosX = 620;
			do {
				DrawRotaGraph(PosX, 160, 0.3f, 0, NumberImage[tmp_score % 10], TRUE);
				tmp_score /= 10;
				PosX -= 20;
			} while (tmp_score > 0);

			//制限時間の描画
			DrawBox(491, 469, 509, 469 - GameTime / 60 * 2, 0x0033ff, TRUE);
		}

#include"GameClearScene.h"
#include"DxLib.h"
#include"SceneManager.h"

		/***********************************
		*マクロ定義
		***********************************/

		/*****************************
		*型定義
		*****************************/

		/*******************************
		*変数宣言
		*******************************/
		int GameClearImage;
		int GameClearSE;
		int GameClearFlag;

		/******************************
		*プロトタイプ宣言
		******************************/
		
		/*************************************
		*ゲームクリア画面：初期化処理
		*引　数：なし
		*戻り値：エラー情報
		*************************************/
		int GameClearScene_Initialize(void)
		{
			int ret = 0;

			//画像の読み込み
			GameClearImage = LoadGraph("images/gameclear.png");
			//音源の読み込み
			GameClearSE = LoadSoundMem("sounds/gemeclear_se.mp3");

			GameClearFlag = 0;

			//エラーチェック
			if (GameClearImage == -1)
			{
				ret = -1;
			}
			if (GameClearSE == -1)
			{
				ret = -1;
			}

			return ret;
		}

		/**************************************
		*ゲームクリア画面：更新処理
		*引　数：なし
		*戻り値：なし
		**************************************/
		void GameClearScene_Update(void)
		{
			//ゲームクリア効果音再生チェック
			if (CheckSoundMem(GameClearSE) == 0)
			{
				if (GameClearFlag == TRUE)
				{
					Change_Scene(E_GAMEMAIN);
				}
				else
				{
					PlaySoundMem(GameClearSE, DX_PLAYTYPE_BACK);
					GameClearFlag = TRUE;
				}
			}
		}

		/**************************************
		*ゲームクリア画面：描画処理
		*引　数：なし
		*戻り値：なし
		**************************************/
		void GameClearScene_Draw(void)
		{
			DrawGraph(0, 0, GameClearImage, FALSE);
		}

#include"FreamControl.h"
#include"DxLib.h"
		/*******************************
		*マクロ定義
		*******************************/

		/*****************************
		*型定義
		*****************************/

		/***************************
		*定数定義
		***************************/

		/***********************
		*変数宣言
		***********************/
		int FreamTime;
		int NowTime;
		int Wait;
		int LastTime;

		/****************************
		*プロトタイプ宣言
		****************************/

		/************************************
		*フレーム制御機能：初期化処理
		*引　数：なし
		*戻り値：なし
		************************************/
		void FreamControl_Initialize(void)
		{
			FreamTime = ((int)1000.0f / FREAM_RATE);
			NowTime = 0;
			Wait = 0;
			LastTime = 0;
		}

		/*******************************
		*フレーム制御機能：更新処理
		*引　数：なし
		*戻り値：なし
		********************************/
		void FreamControl_Update(void)
		{
			NowTime = GetNowCount();
			Wait = FreamTime - (NowTime - LastTime);

			if (Wait > 0)
			{
				WaitTimer(Wait);
			}
			LastTime = GetNowCount();
		}

		/*****************************
		*プログラミング実習
		*マッチ３ゲームの制作
		*****************************/
#include"Dxlib.h"
#include"FreamControl.h"
#include"InputControl.h"
#include"SceneManager.h"
#include "ヘッダー.h"

		/*******************************
		*マクロ定義
		*******************************/
#define SCREEN_HEIGHT     (480)     //スクリーンサイズ（高さ）
#define SCREEN_WIDTH      (640)     //スクリーンサイズ（幅）
#define SCREEN_COLORBIT   (32)      //スクリーンカラービット
#define FONT_SIZE         (20)      //文字サイズ

		/***********************
		*型定義
		***********************/

		/**************************
		*グローバル変数宣言
		**************************/

		/*************************
		*プロトタイプ宣言
		*************************/

		/**************************
		*プログラムの開始
		**************************/
		int WINAPI WinMain(_In_HINSTANCE hInstance, _In_opt_HINSTANCE hPrevInstance,
			_In_LPSTR lpCmdLine, _In_int nShowCmd)
		{

			//ウィンドウタイトル設定
			SetMainWindowText("Match 3 Puzzle");
			//ウィンドモードで起動
			ChangeWindowMode(TRUE);
			//画面サイズの最大サイズ、カラービット数を設定
			SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLORBIT);

			//Dxリブらり初期化処理
			//エラーが発生したら、終了する
			if (DxLib_Init() == D_ERROR)
			{
				return D_ERROR;
			}

			//各機能の初期化処理
			FreamControl_Initialize();           //フレームレート制御機能
			Input_Initialize();                  //入力制御機能

			//シーンマネージャー初期化処理
			//エラーが発生したら、終了する
			if (SceneManager_Initialize(E_TITLE) == D_ERROR)
			{
				return D_ERROR;
			}

			//描画先画面を裏にする
			SetDrawScreen(DX_SCREEN_BACK);

			//文字サイズを設定
			SetFontSize(FONT_SIZE);

			//ゲームループ
			while (ProcessMessage() != D_ERROR && Input_Escape() == FALSE)
			{
				//入力制御機能更新処理
				Input_Update();

				//シーンマネージャー更新処理
				SceneManager_Update();

				//画面クリア
				ClearDrawScreen();

				//シーンマネージャー描画処理
				SceneManager_Draw();

				//フレームレート制御処理
				FreamControl_Update();

				//画面の内容を表画面に反映
				ScreenFlip();

			}
			//Dxライブラリ使用の終了処理
			DxLib_End();

			return 0;
		}



