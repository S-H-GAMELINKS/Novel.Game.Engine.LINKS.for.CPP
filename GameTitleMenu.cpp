// Game Title Menu Source 
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include "ConfigMenu.h"
#include "MouseAndKeyState.hpp"
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//タイトル画面
extern std::int32_t GameTitleGraph;

extern ConfigData_t ConfigData;

namespace {

	//タイトルメニューの描画関数
	void GameTitleMenuDraw(std::int32_t CursorPosY) noexcept {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		DxLib::DrawGraph(0, 0, GameTitleGraph, TRUE);

		//カーソルの描画
		DxLib::DrawString(TitleMenuCursorPosX, CursorPosY, "■", Color);

		//各メニュー描画
		for (std::int32_t i = 0; i < 6; i++)
			DxLib::DrawString(TitleMenuPosX, TitleMenuPosY + 30 * i, TitleMenuItem[i], Color);
	}

	//タイトルメニューの項目選択関数
	void GameTitleMenuChoice(const std::int32_t& CursorPosY) noexcept {

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuPosY))
			EndFlag = 1;

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuLoadPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuLoadPosY))
			SaveDataLoop(2);

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuConfigPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuConfigPosY))
			ConfigMenuLoop();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuQuickLoadPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuQuickLoadPosY))
			QuickSaveDataLoad();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuContinuePosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuContinuePosY))
			ContinueSaveDataLoad();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuExitPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) && CursorPosY == TitleMenuExitPosY))
			GameEndMessageBox();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(std::int32_t& CursorPosY) noexcept {

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(CursorPosY);
		CursorPosY = KeyState::GameTitleMenuKeyMove(CursorPosY);
		MouseState::GameTitleMenuMouseMove(CursorPosY);
		GameTitleMenuChoice(CursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
