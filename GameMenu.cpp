// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "SaveData.hpp"
#include "ConfigMenu.hpp"
#include "Skip_Auto.hpp"
#include "BackLog.hpp"
#include "MouseAndKeyState.hpp"
#include <thread>
#include <chrono>
#include <array>
#include <functional>

//�I���t���O
extern int EndFlag;

//�s�Ǘ��ϐ�
extern int Sp, Cp;

//������`��ʒu
extern int DrawPointX, DrawPointY;

//temp�f�[�^
extern int EndFlagTemp, SpTemp;

//�e��f�ރn���h��
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle;

extern ConfigData_t ConfigData;

namespace {
	//�Q�[�����j���[�`��֐�
	void GameMenuDraw(const std::int32_t& GameMenuCursorPosY) noexcept {

		static constexpr const char* GameMenuItem[] = {
			"�Z�[�u", "���[�h", "�Z�[�u�f�[�^�폜", "���ǃX�L�b�v", "�X�L�b�v", "�I�[�g",
			"�I�[�g/�X�L�b�v��~", "�o�b�N���O�Q��", "�ݒ�", "�^�C�g���ɖ߂�", "�Q�[���ɖ߂�", "�Q�[���I��"
		};

		//�e���j���[���ڂ̕`��
		for (std::int32_t i = 0; i < 12; i++)
			DxLib::DrawString(SaveDataNamePosX, GameMenuBasePosY * (i + 1), GameMenuItem[i], Color);

		//�J�[�\���̕`��
		DxLib::DrawString(SaveDataBasePosX - (CursorMove * 6), GameMenuCursorPosY, "��", Color);
	}

	//�Q�[���֖߂�
	void GameMenuBackToGamePlay() noexcept {
		EndFlag = EndFlagTemp;
		Sp = SpTemp;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//�^�C�g���֖߂�
	void GameMenuBackToTitle() noexcept {

		if (IDYES == MessageBoxYesNo("�^�C�g���ɖ߂�܂����H")) {
			EndFlag = 0;
			Sp = 0;
			Cp = 0;
			DrawPointX = 0;
			DrawPointY = 0;
			BackGroundHandle = 0;
			CharacterHandle = 0;
			BackGroundMusicHandle = 0;
			SoundEffectHandle = 0;
		}
		WaitTime();
	}

	constexpr std::array<int, 4> SkipAutoTaskFlag = { 3, 1, 2, 0 };

	const std::array<std::function<void()>, 4> Funcs = { BackLogLoop, ConfigMenuLoop, GameMenuBackToTitle, GameMenuBackToGamePlay };

	//�Q�[�����j���[���ڑI������
	constexpr void GameMenuSelect(const std::int32_t& GameMenuCursorPosY) noexcept {

		for (std::int32_t i = 0; i < 3; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 1) && MouseAndKey::CheckMouseAndKeyEnter())
				SaveDataLoop(i + 1);
		}

		for (std::int32_t i = 0; i < 3; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 4) && MouseAndKey::CheckMouseAndKeyEnter())
				SkipAndAutoTask(SkipAutoTaskFlag[i], EndFlagTemp);
		}

		for (std::int32_t i = 0; i < 4; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 8) && MouseAndKey::CheckMouseAndKeyEnter())
				Funcs[i]();
		}

		if (GameMenuCursorPosY == GameMenuBasePosY * 12 && MouseAndKey::CheckMouseAndKeyEnter()) {
			if (IDYES == MessageBoxYesNo("�Q�[�����I�����܂����H"))
				EndFlag = 99;
		}
	}
}

//�Q�[�����j���[���[�v
void GameMenuLoop() noexcept {

	std::int32_t GameMenuCursorPosY = GameMenuBasePosY;

	//�l�[���X�y�[�X�̏ȗ�
	using namespace MouseState;
	using namespace KeyState;

	while (EndFlag == 17) {
		ScreenClear();
		GameMenuDraw(GameMenuCursorPosY);
		GameMenuCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseMoveTemplate(GameMenuCursorPosY, GameMenuBasePosY, CursorMove, 12, ConfigData.MouseAndKeyFlag)
																: KeyMoveTemplate(GameMenuCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 12, CursorMove, ConfigData.MouseAndKeyFlag);
		GameMenuSelect(GameMenuCursorPosY);

		//�Q�[���I���m�F�E�C���h�E
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}