// Game Title Menu Source 
#include "DxLib.h"
#include "Utility.h"
#include <thread>
#include <chrono>

//タイトルメニュー位置
extern const std::int32_t title_menu_y;
extern const std::int32_t title_menu_x;
extern const std::int32_t title_menu_load_y;
extern const std::int32_t title_menu_config_y;
extern const std::int32_t title_menu_quick_load_y;
extern const std::int32_t title_menu_continue_y;
extern const std::int32_t title_menu_exit_y;

//タイトルメニューのカーソル位置
extern const std::int32_t title_menu_cursor_x;

//キーカーソルの移動量
extern const std::int32_t cursor_move;

//処理停止時間
extern const std::int32_t wait_task_time;

//終了フラグ
extern int EndFlag;

namespace {

	//タイトルメニューの描画関数
	void GameTitleMenuDraw(unsigned int color, std::int32_t cursor_y) {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		//カーソルの描画
		DxLib::DrawString(title_menu_cursor_x, cursor_y, "■", color);

		//各メニュー描画
		for (std::int32_t i = 0; i < 6; i++)
			DxLib::DrawString(title_menu_x, title_menu_y + 30 * i, TitleMenuItem[i], color);
	}

	//タイトルメニューのキー操作
	void GameTitleMenuKeyMove(std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (title_menu_exit_y == cursor_y) ? title_menu_y : cursor_y + cursor_move;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (title_menu_y == cursor_y) ? title_menu_exit_y : cursor_y - cursor_move;

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_task_time));
	}

	//ゲーム終了(タイトルメニュー)
	void GameTitleMenuExit() {
		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;
	}

	//タイトルメニューの項目選択関数
	void GameTitleMenuChoice(const std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_y)
			EndFlag = 0;

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_load_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_config_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_quick_load_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_continue_y)

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_exit_y)
			GameTitleMenuExit();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(unsigned int color, std::int32_t& cursor_y) {

	while (EndFlag == -1) {
		GameTitleMenuDraw(color, cursor_y);
		GameTitleMenuKeyMove(cursor_y);
		GameTitleMenuChoice(cursor_y);
		ScreenClear();
	}
}
