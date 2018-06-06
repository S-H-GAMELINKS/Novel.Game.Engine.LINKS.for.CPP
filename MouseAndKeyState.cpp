
#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "ConfigMenu.hpp"
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
}

namespace MouseState {

	std::int32_t MousePosY, MousePosX;

	//マウス操作判定のテンプレート
	std::int32_t MouseMoveTemplate(std::int32_t CursorPosY, std::int32_t TopPosY, std::int32_t Move, std::int32_t Num, std::int32_t Flag) {

		DxLib::GetMousePoint(&MousePosX, &MousePosY);

		if (Flag == 1) {
			for (std::int32_t i = 0; i < Num; i++) {
				if ((TopPosY + Move * i) <= MousePosY && MousePosY <= (TopPosY + Move * (i + 1) - 1))
					CursorPosY = TopPosY + Move * i;
			}
		}

		return CursorPosY;
	}
}

namespace MouseAndKey {

	//判定用ラムダ式
	auto MouseCheck = [](const int& Flag, const int& Args) {return (Flag == 0 && DxLib::CheckHitKey(Args) == 1); };
	auto KeyCheck = [](const int& Flag, const int& Args) {return (Flag == 1 && DxLib::GetMouseInput() == Args); };

	const int Flag = ConfigData.MouseAndKeyFlag;

	//マウス＆キー確認（右）
	bool CheckMouseAndKeyRight() {
		return (MouseCheck(Flag, KEY_INPUT_RIGHT) || KeyCheck(Flag, MOUSE_INPUT_RIGHT)) ? true : false;
	}

	//マウス＆キー確認（左）
	bool CheckMouseAndKeyLeft() {
		return (MouseCheck(Flag, KEY_INPUT_LEFT) || KeyCheck(Flag, MOUSE_INPUT_LEFT)) ? true : false;
	}

	//マウス＆キー確認（決定）
	bool CheckMouseAndKeyEnter() {
		return (MouseCheck(Flag, KEY_INPUT_RETURN) || KeyCheck(Flag, MOUSE_INPUT_LEFT)) ? true : false;
	}
}