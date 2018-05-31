#ifndef MOUSE_AND_KEY_STATE_
#define MOUSE_AND_KEY_STATE_

#include <cstdint>

namespace KeyState {

	//キー操作判定のテンプレート
	std::int32_t KeyMoveTemplate(std::int32_t CursorPosY, std::int32_t TopPosY, std::int32_t ButtomPosY, std::int32_t Move, std::int32_t Flag);
}

namespace MouseState {

	//マウス操作判定のテンプレート
	std::int32_t MouseMoveTemplate(std::int32_t CursorPosY, std::int32_t TopPosY, std::int32_t Move, std::int32_t Num, std::int32_t Flag);
}

namespace MouseAndKey {

	//マウス＆キー確認（右）
	bool CheckMouseAndKeyRight();

	//マウス＆キー確認（左）
	bool CheckMouseAndKeyLeft();

	//マウス＆キー確認（決定）
	bool CheckMouseAndKeyEnter();
}

#endif // !MOUSE_AND_KEY_STATE_