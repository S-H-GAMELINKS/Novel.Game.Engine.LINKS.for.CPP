#ifndef MOUSE_AND_KEY_STATE_
#define MOUSE_AND_KEY_STATE_

#include <cstdint>

namespace KeyState {

	//�^�C�g�����j���[�̃L�[����
	void GameTitleMenuKeyMove(std::int32_t& CursorPosY) noexcept;

	//�R���t�B�O��ʃL�[����
	void ConfigMenuKeyMove(std::int32_t& ConfigCursorPosY) noexcept;

	//�Q�[�����j���[�L�[����
	void GameMenuKeyMove(std::int32_t& GameMenuCursorPosY) noexcept;

	//�I�����L�[����
	void ChoiceKeyMove(std::int32_t& ChoiceCursorPosY) noexcept;

	//�Z�[�u�f�[�^���j���[�L�[����
	void SaveDataMenuKeyMove(std::int32_t& SaveDataMenuPosY) noexcept;
}

namespace MouseState {

	//�^�C�g�����j���[�̃}�E�X����
	void GameTitleMenuMouseMove(std::int32_t& CursorPosY) noexcept;

	//�R���t�B�O��ʃ}�E�X����
	void ConfigMenuMouseMove(std::int32_t& ConfigCursorPosY) noexcept;

	//�Q�[�����j���[�̃}�E�X����
	void GameMenuMouseMove(std::int32_t& GameMenuCursorPosY) noexcept;

	//�I�����}�E�X����
	void ChoiceMouseMove(std::int32_t& ChoiceCursorPosY) noexcept;

	//�Z�[�u�f�[�^���j���[�}�E�X����
	void SaveDataMenuMouseMove(std::int32_t& SaveDataMenuPosY) noexcept;
}

#endif // !MOUSE_AND_KEY_STATE_