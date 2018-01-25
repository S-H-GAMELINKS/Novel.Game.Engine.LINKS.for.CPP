// Utility Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include <thread>
#include <chrono>

//ゲームタイトル名
extern const char* GameTitle;

//終了フラグ
extern int EndFlag;

namespace {

	//処理停止時間計算関数
	template <typename T>
	decltype(auto) WaitTimeCalc(const T var) {
		return var * var;
	}
}

//メッセージボックス(Yes or No)
int MessageBoxYesNo(LPCTSTR lpText) noexcept
{
	return MessageBox(
		DxLib::GetMainWindowHandle(),
		lpText,
		GameTitle,
		MB_YESNO
	);
}

//メッセージボックス(OK)
int MessageBoxOk(LPCTSTR lpText) noexcept
{
	return MessageBox(
		DxLib::GetMainWindowHandle(),
		lpText,
		GameTitle,
		MB_OK
	);
}

//画面クリア処理関数
void ScreenClear() noexcept {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_FRONT);
}

//ゲーム終了確認ウインドウ
void GameEndMessageBox() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;

		if (EndFlag != 99 && EndFlag != 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(wait_key_task_time) / wait_game_time));
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}
}

//各種ショートカットキー
void ShortCutKey() {

	if (DxLib::CheckHitKey(KEY_INPUT_F1) == 1)

	if (DxLib::CheckHitKey(KEY_INPUT_F2) == 1)

	if (DxLib::CheckHitKey(KEY_INPUT_F3) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F4) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F5) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F6) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F7) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F8) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F9) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F10) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F11) == 1)
	
	if (DxLib::CheckHitKey(KEY_INPUT_F12) == 1)

	//ゲーム終了確認ウインドウ
	GameEndMessageBox();
}