// Utility Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "GameMenu.h"
#include "SaveData.h"
#include "ConfigMenu.h"
#include "Skip_Auto.h"
#include "BackLog.h"
#include <string>
#include <thread>
#include <chrono>

//ゲームタイトル名
extern const char* GameTitle;

extern int SP, CP;	// 参照する文字列番号と文字列中の文字ポインタ

//終了フラグ
extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SP_Temp;

namespace {

	//処理停止時間計算関数
	template <typename T>
	decltype(auto) WaitTimeCalc(const T var) {
		return var * var;
	}

	//ゲーム画面再描画処理
	void DrawGameScreenAgain() {
		SP = SP_Temp;
		CP = 0;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
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

		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;

		if (EndFlag != 99 && EndFlag != 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(wait_key_task_time) / wait_game_time));
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
}

//スクリーンショット取得関数
void ScreenShotGet() {
	if (IDYES == MessageBoxYesNo("スクリーンショットを取得しますか？")) {
		static std::int32_t ScreenShotCount;
		std::string FileName = "DATA/SCREENSHOT/SCREENSHOT";
		FileName += std::to_string(ScreenShotCount + 1);
		FileName += ".png";

		DxLib::SaveDrawScreenToPNG(0, 0, 640, 480, FileName.c_str());
		ScreenShotCount++;
	}
}

//各種ショートカットキー
void ShortCutKey() {

	if (DxLib::CheckHitKey(KEY_INPUT_F1) == 1) {
		SP_Temp = SP;
		SaveDataLoop(1);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F2) == 1) {
		SP_Temp = SP;
		SaveDataLoop(2);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F3) == 1) {
		SP_Temp = SP;
		SaveDataLoop(3);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F4) == 1) {
		SP_Temp = SP;
		SkipAndAutoTask(3);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F5) == 1) {
		SkipAndAutoTask(1);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F6) == 1) {
		SkipAndAutoTask(2);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F7) == 1) {
		SkipAndAutoTask(0);
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F8) == 1) {
		BackLogLoop();
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F9) == 1) {
		SP_Temp = SP;
		ConfigMenuLoop();
		DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F10) == 1) {
		if (IDYES == MessageBoxYesNo("タイトル画面に戻りますか？")) {
			DxLib::ClearDrawScreen();
			EndFlag = 0;
			SP = 0;
			CP = 0;
			DrawPointX = 0;
			DrawPointY = 0;
			BackGroundHandle = 0;
			CharacterHandle = 0;
			BackGroundMusicHandle = 0;
			SoundEffectHandle = 0;
		} else
			DrawGameScreenAgain();
	}

	if (DxLib::CheckHitKey(KEY_INPUT_F11) == 1) {
		ScreenShotGet();
		CP = 0;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1) {
		SaveDataScreenShotGet();
		EndFlagTemp = EndFlag;
		SP_Temp = SP;
		EndFlag = 17;
	}

	//ゲーム終了確認ウインドウ
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		GameEndMessageBox();
}