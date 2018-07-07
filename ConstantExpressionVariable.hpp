#ifndef CONSTANT_EXPRESSION_VARIABLE_H_
#define CONSTANT_EXPRESSION_VARIABLE_H_

#include "DxLib.h"

//各種素材最大数
constexpr int MaterialMax = 99;

// 文字のサイズ
constexpr int MojiSize = 21;

//タイトルメニュー位置
constexpr int TitleMenuPosY			 = 300;
constexpr int TitleMenuPosX			 = 430;
constexpr int TitleMenuLoadPosY		 = 330;
constexpr int TitleMenuConfigPosY	 = 360;
constexpr int TitleMenuQuickLoadPosY = 390;
constexpr int TitleMenuContinuePosY  = 420;
constexpr int TitleMenuExitPosY		 = 450;

//タイトルメニューのカーソル位置
constexpr int TitleMenuCursorPosX = 400;

//選択肢
constexpr int ChoicePosX   = 100;
constexpr int ChoicePosY[] = { 200, 230 };

//キーカーソルの移動量
constexpr int CursorMove = 30;

//キー処理停止時間
constexpr int WaitKeyTaskTime = 70;

//ゲームプレイ時の遅延時間
constexpr int WaitGameTime = 3;

//ゲームメニュー位置関係
constexpr int GameMenuBasePosY = 30;

//セーブ・ロード画面関係
constexpr int SaveDataSlotNum    = 3;
constexpr int SaveDataBasePosX   = 400;
constexpr int SaveDataBasePosY   = 100;
constexpr int SaveDataCursorMove = 100;
constexpr int SaveDataPosButtom  = SaveDataBasePosY + SaveDataCursorMove * SaveDataSlotNum;
constexpr int SaveSnapDrawPosX   = 150;
constexpr int SaveDataNamePosX   = 250;

//文字描画速度関係
constexpr int StringSpeedSkip = 10;
constexpr int StringSpeedAuto = 30;
constexpr int StringSpeedWait = 600;

//立ち絵位置関係
constexpr int CharacterPosY = 130;
constexpr int CharacterPosX = 150;
constexpr int CharacterGraphSizeWidth  = 300;
constexpr int CharacterGraphSizeHeight = 400;

//文字などの色
constexpr int Color = 0xFFFFFF;

#endif // CONSTANT_EXPRESSION_VARIABLE_H_