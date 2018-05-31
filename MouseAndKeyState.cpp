
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "ConfigMenu.h"
#include <cstdint>
#include <thread>
#include <chrono>

extern ConfigData_t ConfigData;

namespace KeyState {

	//キー操作判定のテンプレート
	std::int32_t KeyMoveTemplate(std::int32_t CursorPosY, std::int32_t TopPosY, std::int32_t ButtomPosY, std::int32_t Move, std::int32_t Flag) {
		if (Flag == 0) {
			if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
				CursorPosY = (ButtomPosY == CursorPosY) ? TopPosY : CursorPosY + Move;

			if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
				CursorPosY = (TopPosY == CursorPosY) ? ButtomPosY : CursorPosY - Move;

			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return CursorPosY;
	}

	//タイトルメニューのキー操作
	std::int32_t GameTitleMenuKeyMove(std::int32_t CursorPosY) noexcept {
		return KeyMoveTemplate(CursorPosY, TitleMenuPosY, TitleMenuExitPosY, CursorMove, ConfigData.MouseAndKeyFlag);
	}

	//コンフィグ画面キー操作
	std::int32_t ConfigMenuKeyMove(std::int32_t ConfigCursorPosY) noexcept {
		return KeyMoveTemplate(ConfigCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 7, CursorMove, ConfigData.MouseAndKeyFlag);
	}

	//ゲームメニューキー操作
	std::int32_t GameMenuKeyMove(std::int32_t GameMenuCursorPosY) noexcept {
		return KeyMoveTemplate(GameMenuCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 12, CursorMove, ConfigData.MouseAndKeyFlag);
	}

	//選択肢キー操作
	std::int32_t ChoiceKeyMove(std::int32_t ChoiceCursorPosY) noexcept {
		return KeyMoveTemplate(ChoiceCursorPosY, ChoicePosY[0], ChoicePosY[1], CursorMove, ConfigData.MouseAndKeyFlag);
	}

	//セーブデータメニューキー操作
	std::int32_t SaveDataMenuKeyMove(std::int32_t SaveDataMenuPosY) noexcept {
		return KeyMoveTemplate(SaveDataMenuPosY, SaveDataBasePosY, SaveDataPosButtom, SaveDataCursorMove, ConfigData.MouseAndKeyFlag);
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

namespace MouseAndKey {

	//マウス＆キー確認（右）
	bool CheckMouseAndKeyRight() {
		return (ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RIGHT) == 1) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_RIGHT)) ? true : false;
	}

	//マウス＆キー確認（左）
	bool CheckMouseAndKeyLeft() {
		return (ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_LEFT) == 1) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)) ? true : false;
	}

	//マウス＆キー確認（決定）
	bool CheckMouseAndKeyEnter() {
		return (ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT) ? true : false;
	}
}