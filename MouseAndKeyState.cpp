
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "ConfigMenu.h"
#include <cstdint>
#include <thread>
#include <chrono>

extern ConfigData_t ConfigData;

namespace KeyState {

	//タイトルメニューのキー操作
	void GameTitleMenuKeyMove(std::int32_t& CursorPosY) noexcept {

		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				CursorPosY = (TitleMenuExitPosY == CursorPosY) ? TitleMenuPosY : CursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				CursorPosY = (TitleMenuPosY == CursorPosY) ? TitleMenuExitPosY : CursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}

	//コンフィグ画面キー操作
	void ConfigMenuKeyMove(std::int32_t& ConfigCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				ConfigCursorPosY = (GameMenuBasePosY * 7 == ConfigCursorPosY) ? GameMenuBasePosY : ConfigCursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				ConfigCursorPosY = (GameMenuBasePosY == ConfigCursorPosY) ? GameMenuBasePosY * 7 : ConfigCursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}

	//ゲームメニューキー操作
	void GameMenuKeyMove(std::int32_t& GameMenuCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				GameMenuCursorPosY = (GameMenuBasePosY * 12 == GameMenuCursorPosY) ? GameMenuBasePosY : GameMenuCursorPosY + GameMenuBasePosY;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				GameMenuCursorPosY = (GameMenuBasePosY == GameMenuCursorPosY) ? GameMenuBasePosY * 12 : GameMenuCursorPosY - GameMenuBasePosY;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}

	//選択肢キー操作
	void ChoiceKeyMove(std::int32_t& ChoiceCursorPosY) noexcept {
		if (ConfigData.MouseAndKeyFlag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				ChoiceCursorPosY = (ChoicePosY[1] == ChoiceCursorPosY) ? ChoicePosY[0] : ChoiceCursorPosY + CursorMove;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				ChoiceCursorPosY = (ChoicePosY[0] == ChoiceCursorPosY) ? ChoicePosY[1] : ChoiceCursorPosY - CursorMove;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}

	//セーブデータメニューキー操作
	void SaveDataMenuKeyMove(std::int32_t& SaveDataMenuPosY) noexcept {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			SaveDataMenuPosY = (SaveDataPosButtom == SaveDataMenuPosY) ? SaveDataBasePosY : SaveDataMenuPosY + SaveDataCursorMove;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			SaveDataMenuPosY = (SaveDataBasePosY == SaveDataMenuPosY) ? SaveDataPosButtom : SaveDataMenuPosY - SaveDataCursorMove;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

}

namespace MouseState {

	//タイトルメニューのマウス操作
	void GameTitleMenuMouseMove(std::int32_t& CursorPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			CursorPosY = (MousePosY <= 329) ? TitleMenuPosY
				: (MousePosY <= (TitleMenuPosY + CursorMove * 2) - 1) ? TitleMenuPosY + CursorMove
				: (MousePosY <= (TitleMenuPosY + CursorMove * 3) - 1) ? TitleMenuPosY + CursorMove * 2
				: (MousePosY <= (TitleMenuPosY + CursorMove * 4) - 1) ? TitleMenuPosY + CursorMove * 3
				: (MousePosY <= TitleMenuExitPosY - 1) ? TitleMenuPosY + CursorMove * 4
				: TitleMenuExitPosY;
		}
	}

	//コンフィグ画面マウス操作
	void ConfigMenuMouseMove(std::int32_t& ConfigCursorPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

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
	}

	//ゲームメニューのマウス操作
	void GameMenuMouseMove(std::int32_t& GameMenuCursorPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

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
	}

	//選択肢マウス操作
	void ChoiceMouseMove(std::int32_t& ChoiceCursorPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			ChoiceCursorPosY = (MousePosY <= (ChoicePosY[1] - 1)) ? ChoicePosY[0] : ChoicePosY[1];
		}
	}

	//セーブデータメニューマウス操作
	void SaveDataMenuMouseMove(std::int32_t& SaveDataMenuPosY) noexcept {

		std::int32_t MousePosY, MousePosX;

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (ConfigData.MouseAndKeyFlag == 1) {
			SaveDataMenuPosY = (MousePosY <= (SaveDataBasePosY * 2) - 1) ? SaveDataBasePosY
				: (MousePosY <= (SaveDataBasePosY * 3) - 1) ? SaveDataBasePosY * 2
				: (MousePosY <= (SaveDataBasePosY * 4) - 1) ? SaveDataBasePosY * 3
				: SaveDataPosButtom;
		}
	}

}