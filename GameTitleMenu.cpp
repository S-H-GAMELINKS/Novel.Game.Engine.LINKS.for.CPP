// Game Title Menu Source 
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include "ConfigMenu.h"
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

	//タイトルメニューのキー操作
	void GameTitleMenuKeyMove(std::int32_t& CursorPosY) noexcept {

		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				CursorPosY = (TitleMenuExitPosY == CursorPosY) ? TitleMenuPosY : CursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				CursorPosY = (TitleMenuPosY == CursorPosY) ? TitleMenuExitPosY : CursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}

	//タイトルメニューのマウス操作
	void GameTitleMenuMouseMove(std::int32_t& CursorPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			CursorPosY = (MousePosY <= 329) ? TitleMenuPosY
				: (MousePosY <= (TitleMenuPosY + CursorMove * 2) - 1) ? TitleMenuPosY + CursorMove
				: (MousePosY <= (TitleMenuPosY + CursorMove * 3) - 1) ? TitleMenuPosY + CursorMove * 2
				: (MousePosY <= (TitleMenuPosY + CursorMove * 4) - 1) ? TitleMenuPosY + CursorMove * 3
				: (MousePosY <= TitleMenuExitPosY - 1) ? TitleMenuPosY + CursorMove * 4
				: TitleMenuExitPosY;
		}
	}

	//ゲーム終了(タイトルメニュー)
	void GameTitleMenuExit() noexcept {
		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;
	}

	//タイトルメニューの項目選択関数
	void GameTitleMenuChoice(const std::int32_t& CursorPosY) noexcept {

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuPosY))
			EndFlag = 1;

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuLoadPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuLoadPosY))
			SaveDataLoop(2);

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuConfigPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuConfigPosY))
			ConfigMenuLoop();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuQuickLoadPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuQuickLoadPosY))
			QuickSaveDataLoad();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuContinuePosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuContinuePosY))
			ContinueSaveDataLoad();

		if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == TitleMenuExitPosY) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() && MOUSE_INPUT_LEFT) == 1 && CursorPosY == TitleMenuExitPosY))
			GameTitleMenuExit();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(std::int32_t& CursorPosY) noexcept {

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(CursorPosY);
		GameTitleMenuKeyMove(CursorPosY);
		GameTitleMenuMouseMove(CursorPosY);
		GameTitleMenuChoice(CursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
