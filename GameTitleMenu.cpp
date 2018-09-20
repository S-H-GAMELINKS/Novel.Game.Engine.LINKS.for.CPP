// Game Title Menu Source 
#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "SaveData.hpp"
#include "ConfigMenu.hpp"
#include "MouseAndKeyState.hpp"
#include <thread>
#include <chrono>

//�I���t���O
extern int EndFlag;

//�^�C�g�����
extern std::int32_t GameTitleGraph;

extern ConfigData_t ConfigData;

namespace {

	using namespace MouseAndKey;

	//�^�C�g�����j���[�̕`��֐�
	void GameTitleMenuDraw(std::int32_t CursorPosY) noexcept {

		const char* TitleMenuItem[] = { "Start", "Load", "Config", "QuickLoad", "Continue", "Exit" };

		DxLib::DrawGraph(0, 0, GameTitleGraph, TRUE);

		//�J�[�\���̕`��
		DxLib::DrawString(TitleMenuCursorPosX, CursorPosY, "��", Color);

		//�e���j���[�`��
		for (std::int32_t i = 0; i < 6; i++)
			DxLib::DrawString(TitleMenuPosX, TitleMenuPosY + 30 * i, TitleMenuItem[i], Color);
	}

	//�^�C�g�����j���[�̍��ڑI���֐�
	constexpr void GameTitleMenuChoice(const std::int32_t& CursorPosY) noexcept {

		if (CursorPosY == TitleMenuPosY && CheckMouseAndKeyEnter())
			EndFlag = 1;

		if (CursorPosY == TitleMenuLoadPosY && CheckMouseAndKeyEnter())
			SaveDataLoop(2);

		if (CursorPosY == TitleMenuConfigPosY && CheckMouseAndKeyEnter())
			ConfigMenuLoop();

		if (CursorPosY == TitleMenuQuickLoadPosY && CheckMouseAndKeyEnter())
			QuickSaveDataLoad();

		if (CursorPosY == TitleMenuContinuePosY && CheckMouseAndKeyEnter())
			ContinueSaveDataLoad();

		if (CursorPosY == TitleMenuExitPosY && CheckMouseAndKeyEnter())
			GameEndMessageBox();
	}
}

//�^�C�g�����j���[�̃��[�v�֐�
void GameTitleMenuLoop(std::int32_t& CursorPosY) noexcept {

	//�l�[���X�y�[�X�̏ȗ�
	using namespace MouseState;
	using namespace KeyState;

	while (EndFlag == 0) {
		ScreenClear();
		GameTitleMenuDraw(CursorPosY);
		CursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseMoveTemplate(CursorPosY, TitleMenuPosY, CursorMove, 6, ConfigData.MouseAndKeyFlag)
														: KeyMoveTemplate(CursorPosY, TitleMenuPosY, TitleMenuExitPosY, CursorMove, ConfigData.MouseAndKeyFlag);
		GameTitleMenuChoice(CursorPosY);

		//�Q�[���I���m�F�E�C���h�E
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}
