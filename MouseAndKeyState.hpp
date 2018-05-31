#ifndef MOUSE_AND_KEY_STATE_
#define MOUSE_AND_KEY_STATE_

#include <cstdint>

namespace KeyState {

	//キー操作判定のテンプレート
	std::int32_t KeyMoveTemplate(std::int32_t CursorPosY, std::int32_t TopPosY, std::int32_t ButtomPosY, std::int32_t Move, std::int32_t Flag);
}

namespace MouseState {

	//タイトルメニューのマウス操作
	std::int32_t GameTitleMenuMouseMove(std::int32_t CursorPosY) noexcept;

	//コンフィグ画面マウス操作
	std::int32_t ConfigMenuMouseMove(std::int32_t ConfigCursorPosY) noexcept;

	//ゲームメニューのマウス操作
	std::int32_t GameMenuMouseMove(std::int32_t GameMenuCursorPosY) noexcept;

	//選択肢マウス操作
	std::int32_t ChoiceMouseMove(std::int32_t ChoiceCursorPosY) noexcept;

	//セーブデータメニューマウス操作
	std::int32_t SaveDataMenuMouseMove(std::int32_t SaveDataMenuPosY) noexcept;
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