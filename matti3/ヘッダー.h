#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

int TitleScene_Initialize(void);
void TitleScene_Update(void);
void TitleScene_Draw(void);

#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

int StageInitialize(void);
void CreateBlock(void);
void SelectBlock(void);
void StageDraw(void);
void FadeOutBlock(void);
void MoveBlock(void);
void CheckBlock(void);
void CheckClear(void);

int Get_StageState(void);
int Get_StageClearFlag(void);
int Get_StageScore(void);

void Set_StageMission(int mission);

#pragma once

/******************************
*マクロ定義
******************************/

#define D_ERROR      (-1)
#define D_NORMALITY  (0)

/******************************
*型定義
******************************/

enum GAME_MODE
{
	E_TITLE,
	E_GAMEMAIN,
	E_GAME_CLEAR,
	E_GAME_OVER,
	E_END,
	E_MODE_MAX
};

/*******************************
*プロトタイプ宣言
*******************************/

int SceneManager_Initialize(GAME_MODE mode);      //初期化処理
void SceneManager_Update(void);             //更新処理
void SceneManager_Draw(void);              //描画処理

void Change_Scene(GAME_MODE mode);                 //シーン変更処理

#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

void Input_Initialize(void);
void Input_Update(void);
int Input_Escape(void);

int GetOldKey(int key);
int GetNowKey(int key);
int GetKeyFlg(int key);
int GetMousePositionX(void);
int GetMousePositionY(void);

#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

int GameOverScene_Initialize(void);
void GameOverScene_Update(void);
void GameOverScene_Draw(void);

#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

int GameMainScene_Initialize(void);
void GameMainScene_Update(void);
void GameMainScene_Draw(void);

#pragma once

/******************************
*マクロ定義
******************************/

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

int GameClearScene_Initialize(void);
void GameClearScene_Update(void);
void GameClearScene_Draw(void);

#pragma once

/******************************
*マクロ定義
******************************/

#define FREAM_RATE  (60U)

/*****************************
*型定義
******************************/

/******************************
*プロトタイプ宣言
******************************/

void FreamControl_Initialize(void);
void FreamControl_Update(void);

