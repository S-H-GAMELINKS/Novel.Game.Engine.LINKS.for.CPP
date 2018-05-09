#ifndef MOUSE_AND_KEY_STATE_
#define MOUSE_AND_KEY_STATE_

#include <cstdint>

namespace KeyState {

	//タイトルメニューのキー操作
	std::int32_t GameTitleMenuKeyMove(std::int32_t CursorPosY) noexcept;

	//コンフィグ画面キー操作
	std::int32_t ConfigMenuKeyMove(std::int32_t ConfigCursorPosY) noexcept;

	//ゲームメニューキー操作
	std::int32_t GameMenuKeyMove(std::int32_t GameMenuCursorPosY) noexcept;

	//選択肢キー操作
	std::int32_t ChoiceKeyMove(std::int32_t ChoiceCursorPosY) noexcept;

	//セーブデータメニューキー操作
	std::int32_t SaveDataMenuKeyMove(std::int32_t SaveDataMenuPosY) noexcept;
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