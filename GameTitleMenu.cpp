// Game Title Menu Source 
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//タイトル画面
extern std::int32_t GameTitleGraph;

static constexpr const char* ConfigMenuItem[] = {
	"ＢＧＭ音量", "ＳＥ音量", "オート速度", "スキップ速度", "文字描画速度", "描画方法", "マウス/キー操作", "戻る"
};

//コンフィグ画面描画
void ConfigMenuDraw(std::int32_t& cursor_y) {

	//各項目の描画
	for (std::int32_t i = 0; i < 8; i++)
		DxLib::DrawString(save_name_pos_x, game_menu_base_pos_y * (i + 1), ConfigMenuItem[i], 255);

	DxLib::DrawString(save_name_pos_x - cursor_move, cursor_y, "■", 255);
}

//コンフィグ画面キー操作
void ConfigMenuKeyMove(std::int32_t& cursor_y) {
	if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
		cursor_y = (game_menu_base_pos_y * 8 == cursor_y) ? game_menu_base_pos_y : cursor_y + cursor_move;

	if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
		cursor_y = (game_menu_base_pos_y == cursor_y) ? game_menu_base_pos_y * 8 : cursor_y - cursor_move;

	std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
}

//コンフィグ画面ループ
void ConfigMenuLoop() {

	std::int32_t ConfigFlag = 1;

	std::int32_t config_y = game_menu_base_pos_y;

	while (ConfigFlag == 1) {
		ScreenClear();
		ConfigMenuDraw(config_y);
		ConfigMenuKeyMove(config_y);
	}

}

namespace {

	//タイトルメニューの描画関数
	void GameTitleMenuDraw(unsigned int color, std::int32_t cursor_y) {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		DxLib::DrawGraph(0, 0, GameTitleGraph, TRUE);

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

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	//ゲーム終了(タイトルメニュー)
	void GameTitleMenuExit() {
		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;
	}

	//タイトルメニューの項目選択関数
	void GameTitleMenuChoice(const std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_y)
			EndFlag = 1;

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_load_y)
			SaveDataLoop(2);

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_config_y)
			ConfigMenuLoop();

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_quick_load_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_continue_y)

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_exit_y)
			GameTitleMenuExit();
	}
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(unsigned int color, std::int32_t& cursor_y) {

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(color, cursor_y);
		GameTitleMenuKeyMove(cursor_y);
		GameTitleMenuChoice(cursor_y);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
