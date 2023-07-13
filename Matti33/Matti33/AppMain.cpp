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





		
