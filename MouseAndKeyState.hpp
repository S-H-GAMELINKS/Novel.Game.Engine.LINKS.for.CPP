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
	void GameTitleMenuMouseMove(std::int32_t& CursorPosY) noexcept;

	//コンフィグ画面マウス操作
	void ConfigMenuMouseMove(std::int32_t& ConfigCursorPosY) noexcept;

	//ゲームメニューのマウス操作
	void GameMenuMouseMove(std::int32_t& GameMenuCursorPosY) noexcept;

	//選択肢マウス操作
	void ChoiceMouseMove(std::int32_t& ChoiceCursorPosY) noexcept;

	//セーブデータメニューマウス操作
	void SaveDataMenuMouseMove(std::int32_t& SaveDataMenuPosY) noexcept;
}

#endif // !MOUSE_AND_KEY_STATE_