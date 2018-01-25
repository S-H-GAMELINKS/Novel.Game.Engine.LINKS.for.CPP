// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include <thread>
#include <chrono>

//ゲームメニュー描画関数
void GameMenuDraw(std::int32_t cursor_y) {

	static constexpr const char* GameMenuItem[] = {
		"セーブ", "ロード", "セーブデータ削除", "既読スキップ", "スキップ", "オート",
		"オート/スキップ停止", "バックログ参照", "設定", "タイトルに戻る", "ゲームに戻る", "ゲーム終了"
	};

	//各メニュー項目の描画
	for (std::int32_t i = 0; i < 12; i++)
		DxLib::DrawString(save_name_pos_x, game_menu_base_pos_y * (i + 1), GameMenuItem[i], 255);

	//カーソルの描画
	DxLib::DrawString(save_base_pos_x - (cursor_move * 6), cursor_y, "■", 255);
}

//ゲームメニューキー操作
void GameMenuKeyMove(std::int32_t& cursor_y) {
	if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
		cursor_y = (game_menu_base_pos_y * 12 == cursor_y) ? game_menu_base_pos_y : cursor_y + game_menu_base_pos_y;

	if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
		cursor_y = (game_menu_base_pos_y == cursor_y) ? game_menu_base_pos_y * 12 : cursor_y - game_menu_base_pos_y;

	std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
}