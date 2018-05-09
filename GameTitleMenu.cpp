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

		if (CursorPosY == TitleMenuPosY && MouseAndKey::CheckMouseAndKeyEnter())
			EndFlag = 1;

		if (CursorPosY == TitleMenuLoadPosY && MouseAndKey::CheckMouseAndKeyEnter())
			SaveDataLoop(2);

		if (CursorPosY == TitleMenuConfigPosY && MouseAndKey::CheckMouseAndKeyEnter())
			ConfigMenuLoop();

		if (CursorPosY == TitleMenuQuickLoadPosY && MouseAndKey::CheckMouseAndKeyEnter())
			QuickSaveDataLoad();

		if (CursorPosY == TitleMenuContinuePosY && MouseAndKey::CheckMouseAndKeyEnter())
			ContinueSaveDataLoad();

		if (CursorPosY == TitleMenuExitPosY && MouseAndKey::CheckMouseAndKeyEnter())
			GameEndMessageBox();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(std::int32_t& CursorPosY) noexcept {

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(CursorPosY);
		CursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseState::GameTitleMenuMouseMove(CursorPosY) : KeyState::GameTitleMenuKeyMove(CursorPosY);
		GameTitleMenuChoice(CursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
