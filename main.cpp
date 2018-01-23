// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include "Material_Manager.h"
#include "ScriptTask_Manager.h"
#include "Variable.h"
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

//DxLib初期化前処理
void DxLibInitPreProccessing() {
	//ログの抽出
	DxLib::SetOutApplicationLogValidFlag(FALSE);

	//アイコンの指定
	DxLib::SetWindowIconID(101);

	//ウインドウのサイズを任意に変更可能
	DxLib::SetWindowSizeChangeEnableFlag(TRUE, TRUE);

	//ウインドウサイズ指定
	DxLib::SetGraphMode(640, 480, 16);

	//ウインドウで描画
	DxLib::ChangeWindowMode(TRUE);

	//ゲームタイトルの指定
	DxLib::SetMainWindowText(GameTitle);

	//ウインドウスタイルの指定
	DxLib::SetWindowStyleMode(7);
}

//DxLib初期化後処理
void DxLibInitPostProccessing() {
	// フォントのサイズセット
	SetFontSize(moji_size);

	//スクリプト読込関数
	ScriptRead(Script, EndFlag);

	//各種素材の読込関数
	MaterialLoad(BackGround, Character, BackGroundMusic, SoundEffect, Movie);
}

//画面クリア処理関数
void ScreenClear() noexcept {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_FRONT);
}

//タイトルメニュー位置
const std::int32_t title_menu_y = 300;
const std::int32_t title_menu_x = 430;
const std::int32_t title_menu_load_y = 330;
const std::int32_t title_menu_config_y = 360;
const std::int32_t title_menu_quick_load_y = 390;
const std::int32_t title_menu_continue_y = 420;
const std::int32_t title_menu_exit_y = 450;

//タイトルメニューのカーソル位置
const std::int32_t title_menu_cursor_x = 400;

//キーカーソルの移動量
const std::int32_t cursor_move = 30;

//処理停止時間
const std::int32_t wait_task_time = 70;

//タイトルメニューの描画関数
void GameTitleMenuDraw(unsigned int color, std::int32_t cursor_y) {

	const char* TitleMenuItem[] = {"Start", "Load", "Config", "QuickLoad", "Continue", "Exit"};

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

//タイトルメニューの項目選択関数
void GameTitleMenuChoice(const std::int32_t& cursor_y) {

	if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_y)
		EndFlag = 0;

	//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_load_y)

	//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_config_y)

	//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_quick_load_y)

	//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_continue_y)

	//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_exit_y)
}

//タイトルメニューのループ関数
void GameTitleMenuLoop(unsigned int color, std::int32_t& cursor_y) {

	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && EndFlag == -1) {
		GameTitleMenuDraw(color, cursor_y);
		GameTitleMenuKeyMove(cursor_y);
		GameTitleMenuChoice(cursor_y);
		ScreenClear();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//初期化前処理
	DxLibInitPreProccessing();

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//初期化後処理
	DxLibInitPostProccessing();

	std::int32_t color = DxLib::GetColor(255, 255, 255);
	std::int32_t cursor_y = 300;

	GameTitleMenuLoop(color, cursor_y);

	ScriptRead(Script, EndFlag);

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//スクリプトタグ処理管理関数
		ScriptTagTaskManager(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie);

		// 終了フラグが１だったら終了する
		if (EndFlag == 1) break;

		//参照文字列の終端まで行っていたら参照文字列を進める
		if (0 < CP && (Script[SP].size() == std::size_t(CP)))
		{
			SP++;
			CP = 0;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
