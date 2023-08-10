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
				｛
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
#define WIDHT            (12)  //ブロック設置サイズ（幅）
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
	int State_Mission;
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
			}while (Check != 0);

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
	*シーン管理機能：描画処理
	*引　数：なし
	*戻り値：なし
	**********************************/
	void SceneManager_Draw(void)
	{
		//各画面の描画処理
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
	*シーン管理機能：シーン切替処理
	*引　数：変更するゲームモード
	*戻り値：なし
	********************************/
	void Change_Scene(GAME_MODE mode)
	{
		Nest_Mode = mode;
	}

#include"DxLib.h"
#include"InputControl.h"

	/****************************
	*変数宣言
	****************************/
	int old_button;
	int now_button;
	int mouse_pusition_x;
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
		now_botton = NULL;
		mouse_position_x = NULL;
		mouse_position_y = NULL;
	}

	/**************************************
	*入力制御機能：更新処理
	*引　数：なし
	*戻り値：なし
	**************************************/
	void Input_Update(void)
	{
		//マウス入力情報の取得
		old_button = now_botton;
		now_button = GetMouseInput();

		//マウスカーソル座標の取得
		GetMousePoint(&mouse_position_, &mouse_position_y);

		]

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
			int keyflg = now_button & ～old_button;

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


		
