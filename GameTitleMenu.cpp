// Game Title Menu Source 
#include "DxLib.h"
#include "Utility.h"
#include <thread>
#include <chrono>

//�^�C�g�����j���[�ʒu
extern const std::int32_t title_menu_y;
extern const std::int32_t title_menu_x;
extern const std::int32_t title_menu_load_y;
extern const std::int32_t title_menu_config_y;
extern const std::int32_t title_menu_quick_load_y;
extern const std::int32_t title_menu_continue_y;
extern const std::int32_t title_menu_exit_y;

//�^�C�g�����j���[�̃J�[�\���ʒu
extern const std::int32_t title_menu_cursor_x;

//�L�[�J�[�\���̈ړ���
extern const std::int32_t cursor_move;

//������~����
extern const std::int32_t wait_task_time;

//�I���t���O
extern int EndFlag;

namespace {

	//�^�C�g�����j���[�̕`��֐�
	void GameTitleMenuDraw(unsigned int color, std::int32_t cursor_y) {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		//�J�[�\���̕`��
		DxLib::DrawString(title_menu_cursor_x, cursor_y, "��", color);

		//�e���j���[�`��
		for (std::int32_t i = 0; i < 6; i++)
			DxLib::DrawString(title_menu_x, title_menu_y + 30 * i, TitleMenuItem[i], color);
	}

	//�^�C�g�����j���[�̃L�[����
	void GameTitleMenuKeyMove(std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (title_menu_exit_y == cursor_y) ? title_menu_y : cursor_y + cursor_move;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (title_menu_y == cursor_y) ? title_menu_exit_y : cursor_y - cursor_move;

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_task_time));
	}

	//�^�C�g�����j���[�̍��ڑI���֐�
	void GameTitleMenuChoice(const std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_y)
			EndFlag = 0;

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_load_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_config_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_quick_load_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_continue_y)

		//if (DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1 && cursor_y == title_menu_exit_y)
	}
}

//�^�C�g�����j���[�̃��[�v�֐�
void GameTitleMenuLoop(unsigned int color, std::int32_t& cursor_y) {

	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0 && EndFlag == -1) {
		GameTitleMenuDraw(color, cursor_y);
		GameTitleMenuKeyMove(cursor_y);
		GameTitleMenuChoice(cursor_y);
		ScreenClear();
	}
}