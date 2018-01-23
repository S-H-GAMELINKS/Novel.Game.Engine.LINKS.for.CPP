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

		if (EndFlag != 99 && EndFlag != -1)
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(wait_key_task_time) / wait_game_time));
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}
}