// Utility Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "GameMenu.hpp"
#include "SaveData.hpp"
#include "ConfigMenu.hpp"
#include "Skip_Auto.hpp"
#include "BackLog.hpp"
#include <string>
#include <thread>
#include <chrono>
#include <array>
#include <functional>

//�Q�[���^�C�g����
extern const char* GameTitle;

extern int Sp, Cp;	// �Q�Ƃ��镶����ԍ��ƕ����񒆂̕����|�C���^

//�I���t���O
extern int EndFlag;

//�e��f�ރn���h��
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle;

//������`��ʒu
extern int DrawPointX, DrawPointY;

//temp�f�[�^
extern int EndFlagTemp, SpTemp;

extern ConfigData_t ConfigData;

namespace {

	//������~���Ԍv�Z�֐�
	constexpr int WaitTimeCalc(const int& var) noexcept {
		return var * var;
	}

	//�Q�[����ʍĕ`�揈��
	void DrawGameScreenAgain() noexcept {
		Sp = SpTemp;
		Cp = DrawPointX = DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}
}

//���b�Z�[�W�{�b�N�X(Yes or No)
int MessageBoxYesNo(const LPCTSTR lpText) noexcept
{
	return MessageBox(
		DxLib::GetMainWindowHandle(),
		lpText,
		GameTitle,
		MB_YESNO
	);
}

//���b�Z�[�W�{�b�N�X(OK)
int MessageBoxOk(const LPCTSTR lpText) noexcept
{
	return MessageBox(
		DxLib::GetMainWindowHandle(),
		lpText,
		GameTitle,
		MB_OK
	);
}

//��ʃN���A�����֐�
void ScreenClear() noexcept {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_FRONT);
}

//�Q�[���I���m�F�E�C���h�E
void GameEndMessageBox() noexcept {

		if (IDYES == MessageBoxYesNo("�I�����܂����H"))
			EndFlag = 99;

		if (EndFlag != 99 && EndFlag != 0)
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(WaitKeyTaskTime) / WaitGameTime));
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}

//�X�N���[���V���b�g�擾�֐�
void ScreenShotGet() noexcept {
	if (IDYES == MessageBoxYesNo("�X�N���[���V���b�g���擾���܂����H")) {
		static std::int32_t ScreenShotCount;
		std::string FileName = "DATA/SCREENSHOT/SCREENSHOT" + std::to_string(ScreenShotCount + 1) + ".png";

		DxLib::SaveDrawScreenToPNG(0, 0, 640, 480, FileName.c_str());
		ScreenShotCount++;
	}
}

namespace {

	//�V���[�g�J�b�g�L�[�e���v���[�g
	template <typename F>
	void ShortcutKeyTemplate(F&& func) {
		SpTemp = Sp;
		func();
		DrawGameScreenAgain();
	}

	//�Z�[�u�f�[�^�֘A
	void SaveData(const int Num) {
		SpTemp = Sp;
		SaveDataLoop(Num);
		DrawGameScreenAgain();
	}

	//�X�L�b�v�E�I�[�g�֌W
	void SkipAndAuto(const int Num, const int EndFlag) {
		SpTemp = Sp;
		SkipAndAutoTask(Num, EndFlag);
		DrawGameScreenAgain();
	}

	//�^�C�g���֖߂�
	void BackToTitle() {
		if (IDYES == MessageBoxYesNo("�^�C�g����ʂɖ߂�܂����H")) {
			DxLib::ClearDrawScreen();
			EndFlag = Sp = Cp = 0;
			DrawPointX = DrawPointY = 0;
			BackGroundHandle = CharacterHandle = BackGroundMusicHandle = SoundEffectHandle = 0;
		}
		else
			DrawGameScreenAgain();
	}

	//�N�C�b�N�Z�[�u�֌W
	void QuickSave() {
		SpTemp = Sp;
		QuickSaveDataSave();
		DrawGameScreenAgain();
	}

	//�X�N���[���V���b�g�֘A
	void ScreenShot() {
		ScreenShotGet();
		Cp = DrawPointX = DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//�Q�[�����j���[�֘A
	void GameMenu() {
		SaveDataScreenShotGet();
		DxLib::StopSoundMem(BackGroundMusicHandle);
		DxLib::StopSoundMem(SoundEffectHandle);
		EndFlagTemp = EndFlag;
		SpTemp = Sp;
		EndFlag = 17;
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitTimeCalc(WaitKeyTaskTime) / WaitGameTime));
	}
}

//�e��V���[�g�J�b�g�L�[
void ShortCutKey() noexcept {

	constexpr std::array<int, 12> FuncKey = { KEY_INPUT_F1, KEY_INPUT_F2, KEY_INPUT_F3, KEY_INPUT_F4,
											KEY_INPUT_F5, KEY_INPUT_F6, KEY_INPUT_F7, KEY_INPUT_F8,
											KEY_INPUT_F9, KEY_INPUT_F10, KEY_INPUT_F11, KEY_INPUT_F12 };

	constexpr std::array<int, 4> SkipAndAutoFlag = { 3, 1, 2, 0 };

	const std::array<std::function<void()>, 4> Funcs = { BackLogLoop, ConfigMenuLoop, BackToTitle, QuickSaveDataSave };

	for (std::int32_t i = 0; i < 3; i++) {
		if (DxLib::CheckHitKey(FuncKey[i]) == 1)
			SaveData(i + 1);
	}

	for (std::int32_t i = 0; i < 4; i++) {
		if (DxLib::CheckHitKey(FuncKey[i + 3]) == 1)
			SkipAndAuto(SkipAndAutoFlag[i], EndFlag);
	}

	for (std::int32_t i = 0; i < 4; i++) {
		if (DxLib::CheckHitKey(FuncKey[i + 7]) == 1)
			Funcs[i]();
	}

	if (DxLib::CheckHitKey(FuncKey[11] == 1))
		ScreenShot();

	if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1 || (DxLib::GetMouseInput() == MOUSE_INPUT_RIGHT))
		GameMenu();

	//�Q�[���I���m�F�E�C���h�E
	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		GameEndMessageBox();
}

//�^�C�g�����[�r�[����
void TitleMoviePlay() noexcept {
	PlayMovie("DATA/MOVIE/OP.wmv", 1, DX_MOVIEPLAYTYPE_BCANCEL);
	ScreenClear();
}

//�E�F�C�g����
void WaitTime() noexcept {
	std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}