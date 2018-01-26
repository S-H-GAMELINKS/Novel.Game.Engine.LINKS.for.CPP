// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//行管理変数
extern int SP, CP;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SP_Temp;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

namespace {
	//ゲームメニュー描画関数
	void GameMenuDraw(std::int32_t& cursor_y, unsigned int color) {

		static constexpr const char* GameMenuItem[] = {
			"セーブ", "ロード", "セーブデータ削除", "既読スキップ", "スキップ", "オート",
			"オート/スキップ停止", "バックログ参照", "設定", "タイトルに戻る", "ゲームに戻る", "ゲーム終了"
		};

		//各メニュー項目の描画
		for (std::int32_t i = 0; i < 12; i++)
			DxLib::DrawString(save_name_pos_x, game_menu_base_pos_y * (i + 1), GameMenuItem[i], color);

		//カーソルの描画
		DxLib::DrawString(save_base_pos_x - (cursor_move * 6), cursor_y, "■", color);
	}

	//ゲームメニューキー操作
	void GameMenuKeyMove(std::int32_t& cursor_y) {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (game_menu_base_pos_y * 12 == cursor_y) ? game_menu_base_pos_y : cursor_y + game_menu_base_pos_y;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (game_menu_base_pos_y == cursor_y) ? game_menu_base_pos_y * 12 : cursor_y - game_menu_base_pos_y;

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	//ゲームへ戻る
	void GameMenuBackToGamePlay() {
		EndFlag = EndFlagTemp;
		SP = SP_Temp;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//ゲームメニュー項目選択処理
	void GameMenuSelect(std::int32_t& cursor_y) {

		if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(1);

		if (cursor_y == game_menu_base_pos_y * 2 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(2);

		if (cursor_y == game_menu_base_pos_y * 3 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(3);

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		//if (cursor_y == game_menu_base_pos_y && CheckHitKey(KEY_INPUT_RETURN) == 1)

		if (cursor_y == game_menu_base_pos_y * 11 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			GameMenuBackToGamePlay();

		if (cursor_y == game_menu_base_pos_y * 12 && CheckHitKey(KEY_INPUT_RETURN) == 1) {
			if (IDYES == MessageBoxYesNo("ゲームを終了しますか？"))
				EndFlag = 99;
		}

	}
}

//ゲームメニューループ
void GameMenuLoop() {

	std::int32_t gamemenu_y = game_menu_base_pos_y;
	unsigned int color = DxLib::GetColor(255, 255, 255);

	DxLib::ClearDrawScreen();

	while (EndFlag == 17) {
		ScreenClear();
		GameMenuDraw(gamemenu_y, color);
		GameMenuKeyMove(gamemenu_y);
		GameMenuSelect(gamemenu_y);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}