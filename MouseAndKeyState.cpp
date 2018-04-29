
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "ConfigMenu.h"
#include <cstdint>
#include <thread>
#include <chrono>

extern ConfigData_t ConfigData;

namespace KeyState {

	//タイトルメニューのキー操作
	std::int32_t GameTitleMenuKeyMove(std::int32_t CursorPosY) noexcept {

		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				CursorPosY = (TitleMenuExitPosY == CursorPosY) ? TitleMenuPosY : CursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				CursorPosY = (TitleMenuPosY == CursorPosY) ? TitleMenuExitPosY : CursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return CursorPosY;
	}

	//コンフィグ画面キー操作
	std::int32_t ConfigMenuKeyMove(std::int32_t ConfigCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				ConfigCursorPosY = (GameMenuBasePosY * 7 == ConfigCursorPosY) ? GameMenuBasePosY : ConfigCursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				ConfigCursorPosY = (GameMenuBasePosY == ConfigCursorPosY) ? GameMenuBasePosY * 7 : ConfigCursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return ConfigCursorPosY;
	}

	//ゲームメニューキー操作
	std::int32_t GameMenuKeyMove(std::int32_t GameMenuCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				GameMenuCursorPosY = (GameMenuBasePosY * 12 == GameMenuCursorPosY) ? GameMenuBasePosY : GameMenuCursorPosY + GameMenuBasePosY;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				GameMenuCursorPosY = (GameMenuBasePosY == GameMenuCursorPosY) ? GameMenuBasePosY * 12 : GameMenuCursorPosY - GameMenuBasePosY;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return GameMenuCursorPosY;
	}

	//選択肢キー操作
	std::int32_t ChoiceKeyMove(std::int32_t ChoiceCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				ChoiceCursorPosY = (ChoicePosY[1] == ChoiceCursorPosY) ? ChoicePosY[0] : ChoiceCursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				ChoiceCursorPosY = (ChoicePosY[0] == ChoiceCursorPosY) ? ChoicePosY[1] : ChoiceCursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return ChoiceCursorPosY;
	}

	//セーブデータメニューキー操作
	std::int32_t SaveDataMenuKeyMove(std::int32_t SaveDataMenuPosY) noexcept {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			SaveDataMenuPosY = (SaveDataPosButtom == SaveDataMenuPosY) ? SaveDataBasePosY : SaveDataMenuPosY + SaveDataCursorMove;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			SaveDataMenuPosY = (SaveDataBasePosY == SaveDataMenuPosY) ? SaveDataPosButtom : SaveDataMenuPosY - SaveDataCursorMove;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

		return SaveDataMenuPosY;
	}

}

namespace MouseState {

	std::int32_t MousePosY, MousePosX;

	//タイトルメニューのマウス操作
	std::int32_t GameTitleMenuMouseMove(std::int32_t CursorPosY) noexcept {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			CursorPosY = (MousePosY <= 329) ? TitleMenuPosY
				: (MousePosY <= (TitleMenuPosY + CursorMove * 2) - 1) ? TitleMenuPosY + CursorMove
				: (MousePosY <= (TitleMenuPosY + CursorMove * 3) - 1) ? TitleMenuPosY + CursorMove * 2
				: (MousePosY <= (TitleMenuPosY + CursorMove * 4) - 1) ? TitleMenuPosY + CursorMove * 3
				: (MousePosY <= TitleMenuExitPosY - 1) ? TitleMenuPosY + CursorMove * 4
				: TitleMenuExitPosY;
		}

		return CursorPosY;
	}

	//コンフィグ画面マウス操作
	std::int32_t ConfigMenuMouseMove(std::int32_t ConfigCursorPosY) noexcept {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			ConfigCursorPosY = (MousePosY <= (GameMenuBasePosY * 2) - 1) ? GameMenuBasePosY
				: (MousePosY <= (GameMenuBasePosY * 3) - 1) ? GameMenuBasePosY * 2
				: (MousePosY <= (GameMenuBasePosY * 4) - 1) ? GameMenuBasePosY * 3
				: (MousePosY <= (GameMenuBasePosY * 5) - 1) ? GameMenuBasePosY * 4
				: (MousePosY <= (GameMenuBasePosY * 6) - 1) ? GameMenuBasePosY * 5
				: (MousePosY <= (GameMenuBasePosY * 7) - 1) ? GameMenuBasePosY * 6
				: GameMenuBasePosY * 7;
		}

		return ConfigCursorPosY;
	}

	//ゲームメニューのマウス操作
	std::int32_t GameMenuMouseMove(std::int32_t GameMenuCursorPosY) noexcept {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			GameMenuCursorPosY = (MousePosY <= (GameMenuBasePosY * 2) - 1) ? GameMenuBasePosY
				: (MousePosY <= (GameMenuBasePosY * 3) - 1) ? (GameMenuBasePosY * 2)
				: (MousePosY <= (GameMenuBasePosY * 4) - 1) ? (GameMenuBasePosY * 3)
				: (MousePosY <= (GameMenuBasePosY * 5) - 1) ? (GameMenuBasePosY * 4)
				: (MousePosY <= (GameMenuBasePosY * 6) - 1) ? (GameMenuBasePosY * 5)
				: (MousePosY <= (GameMenuBasePosY * 7) - 1) ? (GameMenuBasePosY * 6)
				: (MousePosY <= (GameMenuBasePosY * 8) - 1) ? (GameMenuBasePosY * 7)
				: (MousePosY <= (GameMenuBasePosY * 9) - 1) ? (GameMenuBasePosY * 8)
				: (MousePosY <= (GameMenuBasePosY * 10) - 1) ? (GameMenuBasePosY * 9)
				: (MousePosY <= (GameMenuBasePosY * 11) - 1) ? (GameMenuBasePosY * 10)
				: (MousePosY <= (GameMenuBasePosY * 12) - 1) ? (GameMenuBasePosY * 11)
				: (GameMenuBasePosY * 12);
		}

		return GameMenuCursorPosY;
	}

	//選択肢マウス操作
	std::int32_t ChoiceMouseMove(std::int32_t ChoiceCursorPosY) noexcept {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			ChoiceCursorPosY = (MousePosY <= (ChoicePosY[1] - 1)) ? ChoicePosY[0] : ChoicePosY[1];
		}

		return ChoiceCursorPosY;
	}

	//セーブデータメニューマウス操作
	std::int32_t SaveDataMenuMouseMove(std::int32_t SaveDataMenuPosY) noexcept {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			SaveDataMenuPosY = (MousePosY <= (SaveDataBasePosY * 2) - 1) ? SaveDataBasePosY
				: (MousePosY <= (SaveDataBasePosY * 3) - 1) ? SaveDataBasePosY * 2
				: (MousePosY <= (SaveDataBasePosY * 4) - 1) ? SaveDataBasePosY * 3
				: SaveDataPosButtom;
		}

		return SaveDataMenuPosY;
	}

}