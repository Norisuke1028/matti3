#pragma once

/******************************
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
******************************/

int TitleScene_Initialize(void);
void TitleScene_Update(void);
void TitleScene_Draw(void);

#pragma once

/******************************
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
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
*�}�N����`
******************************/

#define D_ERROR      (-1)
#define D_NORMALITY  (0)

/******************************
*�^��`
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
*�v���g�^�C�v�錾
*******************************/

int SceneManager_Initialize(GAME_MODE mode);      //����������
void SceneManager_Update(void);             //�X�V����
void SceneManager_Draw(void);              //�`�揈��

void Change_Scene(GAME_MODE mode);                 //�V�[���ύX����

#pragma once

/******************************
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
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
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
******************************/

int GameOverScene_Initialize(void);
void GameOverScene_Update(void);
void GameOverScene_Draw(void);

#pragma once

/******************************
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
******************************/

int GameMainScene_Initialize(void);
void GameMainScene_Update(void);
void GameMainScene_Draw(void);

#pragma once

/******************************
*�}�N����`
******************************/

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
******************************/

int GameClearScene_Initialize(void);
void GameClearScene_Update(void);
void GameClearScene_Draw(void);

#pragma once

/******************************
*�}�N����`
******************************/

#define FREAM_RATE  (60U)

/*****************************
*�^��`
******************************/

/******************************
*�v���g�^�C�v�錾
******************************/

void FreamControl_Initialize(void);
void FreamControl_Update(void);

