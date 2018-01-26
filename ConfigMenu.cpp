// Config Menu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <thread>
#include <chrono>


namespace {

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

	//コンフィグ画面選択処理
	void ConfigMenuSelect(std::int32_t& cursor_y, std::int32_t& ConfigFlag) {

		if (game_menu_base_pos_y * 8 == cursor_y && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			if (IDYES == MessageBoxYesNo("戻りますか？")) {
				ConfigFlag = 0;
			}
		}
	}
}

//コンフィグ画面ループ
void ConfigMenuLoop() {

	std::int32_t ConfigFlag = 1;

	std::int32_t config_y = game_menu_base_pos_y;

	while (ConfigFlag == 1) {
		ScreenClear();
		ConfigMenuDraw(config_y);
		ConfigMenuKeyMove(config_y);
		ConfigMenuSelect(config_y, ConfigFlag);
	}
}
