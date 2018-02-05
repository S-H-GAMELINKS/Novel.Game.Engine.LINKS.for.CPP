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

namespace {

	//タイトルメニューの描画関数
	void GameTitleMenuDraw(unsigned int Color, std::int32_t CursorPosY) {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		DxLib::DrawGraph(0, 0, GameTitleGraph, TRUE);

		//カーソルの描画
		DxLib::DrawString(title_menu_cursor_x, CursorPosY, "■", Color);

		//各メニュー描画
		for (std::int32_t i = 0; i < 6; i++)
			DxLib::DrawString(title_menu_x, title_menu_y + 30 * i, TitleMenuItem[i], Color);
	}

	//タイトルメニューのキー操作
	void GameTitleMenuKeyMove(std::int32_t& CursorPosY) {

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			CursorPosY = (title_menu_exit_y == CursorPosY) ? title_menu_y : CursorPosY + CursorMove;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			CursorPosY = (title_menu_y == CursorPosY) ? title_menu_exit_y : CursorPosY - CursorMove;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//ゲーム終了(タイトルメニュー)
	void GameTitleMenuExit() {
		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;
	}

	//タイトルメニューの項目選択関数
	void GameTitleMenuChoice(const std::int32_t& CursorPosY) {

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_y)
			EndFlag = 1;

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_load_y)
			SaveDataLoop(2);

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_config_y)
			ConfigMenuLoop();

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_quick_load_y)
			QuickSaveDataLoad();

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_continue_y)
			ContinueSaveDataLoad();

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && CursorPosY == title_menu_exit_y)
			GameTitleMenuExit();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(unsigned int Color, std::int32_t& CursorPosY) {

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(Color, CursorPosY);
		GameTitleMenuKeyMove(CursorPosY);
		GameTitleMenuChoice(CursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
