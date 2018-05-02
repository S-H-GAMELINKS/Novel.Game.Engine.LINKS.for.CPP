// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include "ConfigMenu.h"
#include "Skip_Auto.h"
#include "BackLog.h"
#include "MouseAndKeyState.hpp"
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//行管理変数
extern int Sp, Cp;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SpTemp;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

extern ConfigData_t ConfigData;

namespace {
	//ゲームメニュー描画関数
	void GameMenuDraw(const std::int32_t& GameMenuCursorPosY) noexcept {

		static constexpr const char* GameMenuItem[] = {
			"セーブ", "ロード", "セーブデータ削除", "既読スキップ", "スキップ", "オート",
			"オート/スキップ停止", "バックログ参照", "設定", "タイトルに戻る", "ゲームに戻る", "ゲーム終了"
		};

		//各メニュー項目の描画
		for (std::int32_t i = 0; i < 12; i++)
			DxLib::DrawString(SaveDataNamePosX, GameMenuBasePosY * (i + 1), GameMenuItem[i], Color);

		//カーソルの描画
		DxLib::DrawString(SaveDataBasePosX - (CursorMove * 6), GameMenuCursorPosY, "■", Color);
	}

	//ゲームへ戻る
	void GameMenuBackToGamePlay() noexcept {
		EndFlag = EndFlagTemp;
		Sp = SpTemp;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//タイトルへ戻る
	void GameMenuBackToTitle() noexcept {

		if (IDYES == MessageBoxYesNo("タイトルに戻りますか？")) {
			EndFlag = 0;
			Sp = 0;
			Cp = 0;
			DrawPointX = 0;
			DrawPointY = 0;
			BackGroundHandle = 0;
			CharacterHandle = 0;
			BackGroundMusicHandle = 0;
			SoundEffectHandle = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//ゲームメニュー項目選択処理
	void GameMenuSelect(const std::int32_t& GameMenuCursorPosY) noexcept {

		if ((GameMenuCursorPosY == GameMenuBasePosY && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SaveDataLoop(1);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 2 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 2 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SaveDataLoop(2);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 3 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 3 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SaveDataLoop(3);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 4 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 4 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SkipAndAutoTask(3);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 5 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 5 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SkipAndAutoTask(1);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 6 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 6 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SkipAndAutoTask(2);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 7 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 7 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			SkipAndAutoTask(0);

		if ((GameMenuCursorPosY == GameMenuBasePosY * 8 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 8 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			BackLogLoop();

		if ((GameMenuCursorPosY == GameMenuBasePosY * 9 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 9 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			ConfigMenuLoop();

		if ((GameMenuCursorPosY == GameMenuBasePosY * 10 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 10 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			GameMenuBackToTitle();

		if ((GameMenuCursorPosY == GameMenuBasePosY * 11 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 11 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
			GameMenuBackToGamePlay();

		if ((GameMenuCursorPosY == GameMenuBasePosY * 12 && ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (GameMenuCursorPosY == GameMenuBasePosY * 12 && ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT))) {
			if (IDYES == MessageBoxYesNo("ゲームを終了しますか？"))
				EndFlag = 99;
		}
	}
}

//ゲームメニューループ
void GameMenuLoop() noexcept {

	std::int32_t GameMenuCursorPosY = GameMenuBasePosY;

	while (EndFlag == 17) {
		ScreenClear();
		GameMenuDraw(GameMenuCursorPosY);
		GameMenuCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseState::GameMenuMouseMove(GameMenuCursorPosY) : KeyState::GameMenuKeyMove(GameMenuCursorPosY);
		GameMenuSelect(GameMenuCursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}