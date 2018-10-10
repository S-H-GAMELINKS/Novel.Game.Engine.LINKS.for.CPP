// Utility Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "GameMenu.hpp"
#include "SaveData.hpp"
#include "ConfigMenu.hpp"
#include "Skip_Auto.hpp"
#include "BackLog.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <array>
#include <functional>

//ゲームタイトル名
extern const char* GameTitle;

extern int Sp, Cp;	// 参照する文字列番号と文字列中の文字ポインタ

//終了フラグ
extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SpTemp;

extern ConfigData_t ConfigData;

namespace {

	//処理停止時間計算関数
	constexpr int WaitTimeCalc(const int& var) noexcept {
		return var * var;
	}

	//ゲーム画面再描画処理
	void DrawGameScreenAgain() noexcept {
		Sp = SpTemp;
		Cp = DrawPointX = DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}
}

//メッセージボックス(Yes or No)
int MessageBoxYesNo(const LPCTSTR lpText) noexcept
{
	return MessageBox(
		DxLib::GetMainWindowHandle(),
		lpText,
		GameTitle,
		MB_YESNO
	);
}

//メッセージボックス(OK)
int MessageBoxOk(const LPCTSTR lpText) noexcept
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
void GameEndMessageBox() noexcept {

		if (IDYES == MessageBoxYesNo("終了しますか？"))
			EndFlag = 99;

		if (EndFlag != 99 && EndFlag != 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(WaitKeyTaskTime) / WaitGameTime));
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}

//スクリーンショット取得関数
void ScreenShotGet() noexcept {
	if (IDYES == MessageBoxYesNo("スクリーンショットを取得しますか？")) {
		static std::int32_t ScreenShotCount;
		std::string FileName = "DATA/SCREENSHOT/SCREENSHOT" + std::to_string(ScreenShotCount + 1) + ".png";

		DxLib::SaveDrawScreenToPNG(0, 0, 640, 480, FileName.c_str());
		ScreenShotCount++;
	}
}

namespace {

	//ショートカットキーテンプレート
	template <typename F>
	void ShortcutKeyTemplate(F&& func) {
		SpTemp = Sp;
		func();
		DrawGameScreenAgain();
	}

	//セーブデータ関連
	void SaveData(const int Num) {
		SpTemp = Sp;
		SaveDataLoop(Num);
		DrawGameScreenAgain();
	}

	//スキップ・オート関係
	void SkipAndAuto(const int Num, const int EndFlag) {
		SpTemp = Sp;
		SkipAndAutoTask(Num, EndFlag);
		DrawGameScreenAgain();
	}

	//タイトルへ戻る
	void BackToTitle() {
		if (IDYES == MessageBoxYesNo("タイトル画面に戻りますか？")) {
			DxLib::ClearDrawScreen();
			EndFlag = Sp = Cp = 0;
			DrawPointX = DrawPointY = 0;
			BackGroundHandle = CharacterHandle = BackGroundMusicHandle = SoundEffectHandle = 0;
		}
		else
			DrawGameScreenAgain();
	}

	//クイックセーブ関係
	void QuickSave() {
		SpTemp = Sp;
		QuickSaveDataSave();
		DrawGameScreenAgain();
	}

	//スクリーンショット関連
	void ScreenShot() {
		ScreenShotGet();
		Cp = DrawPointX = DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//ゲームメニュー関連
	void GameMenu() {
		SaveDataScreenShotGet();
		DxLib::StopSoundMem(BackGroundMusicHandle);
		DxLib::StopSoundMem(SoundEffectHandle);
		EndFlagTemp = EndFlag;
		SpTemp = Sp;
		EndFlag = 17;
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(WaitKeyTaskTime) / WaitGameTime));
	}
}

//各種ショートカットキー
void ShortCutKey() noexcept {

	constexpr std::array<int, 12> FuncKey = { KEY_INPUT_F1, KEY_INPUT_F2, KEY_INPUT_F3, KEY_INPUT_F4,
											KEY_INPUT_F5, KEY_INPUT_F6, KEY_INPUT_F7, KEY_INPUT_F8,
											KEY_INPUT_F9, KEY_INPUT_F10, KEY_INPUT_F11, KEY_INPUT_F12 };

	constexpr std::array<int, 4> SkipAndAutoFlag = { 3, 1, 2, 0 };

	const std::array<std::function<void()>, 4> Funcs = { BackLogLoop, ConfigMenuLoop, BackToTitle, QuickSaveDataSave };

	for (std::int32_t i = 0; i < 3; i++) {
		if (DxLib::CheckHitKey(FuncKey[i]) == 1)
			SaveData(i + 1);
	}

	for (std::int32_t i = 0; i < 4; i++) {
		if (DxLib::CheckHitKey(FuncKey[i + 3]) == 1)
			SkipAndAuto(SkipAndAutoFlag[i], EndFlag);
	}

	for (std::int32_t i = 0; i < 4; i++) {
		if (DxLib::CheckHitKey(FuncKey[i + 7]) == 1)
			Funcs[i]();
	}

	if (DxLib::CheckHitKey(FuncKey[11] == 1))
		ScreenShot();

	if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1 || (DxLib::GetMouseInput() == MOUSE_INPUT_RIGHT))
		GameMenu();

	//ゲーム終了確認ウインドウ
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		GameEndMessageBox();
}

//タイトルムービー処理
void TitleMoviePlay() noexcept {
	PlayMovie("DATA/MOVIE/OP.wmv", 1, DX_MOVIEPLAYTYPE_BCANCEL);
	ScreenClear();
}

//ウェイト処理
void WaitTime() noexcept {
	std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}