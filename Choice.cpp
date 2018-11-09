// Choice Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "BackLog.hpp"
#include "ConfigMenu.hpp"
#include "MouseAndKeyState.hpp"
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

extern int EndFlag;

//�e��f�ރn���h��
extern std::int32_t BackGroundHandle, CharacterHandle;

extern ConfigData_t ConfigData;

//�I�����p�ϐ�
static std::string Choice[2];
const char* ChoiceScript[][2] = {
	{ "DATA/STR/CHOICE/A.txt", "DATA/STR/CHOICE/B.txt" },
	{ "DATA/STR/CHOICE/C.txt", "DATA/STR/CHOICE/D.txt" },
	{ "DATA/STR/CHOICE/E.txt", "DATA/STR/CHOICE/F.txt" },
	{ "DATA/STR/CHOICE/G.txt", "DATA/STR/CHOICE/H.txt" },
	{ "DATA/STR/CHOICE/I.txt", "DATA/STR/CHOICE/J.txt" },
	{ "DATA/STR/CHOICE/K.txt", "DATA/STR/CHOICE/L.txt" },
	{ "DATA/STR/CHOICE/M.txt", "DATA/STR/CHOICE/N.txt" }
};

namespace {

	//�I�����`��֐�
	void DrawChoice(const std::int32_t& ChoiceCursorPosY) noexcept {

		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);

		//�J�[�\���̕`��
		DxLib::DrawString(ChoicePosX, ChoiceCursorPosY, "��", Color);

		for (auto&& i : { 0, 1 })
			DxLib::DrawString(ChoicePosX + CursorMove, ChoicePosY[i], Choice[i].c_str(), Color);
	}

	//�I�����Ǎ��֐�
	void ChoiceRead() noexcept {
		if (1 <= EndFlag && EndFlag <= 7) {
			for (std::size_t i : {0, 1}) {
				std::ifstream File(ChoiceScript[EndFlag - 1][i], std::ios_base::in);
				std::getline(File, Choice[i]);
			}
		}
	}

	//�I�������򏈗�
	void ChoiceSelect(const int& Num) noexcept {
		if (1 <= EndFlag && EndFlag <= 7) {
			BackLogGet();
			EndFlag = Num;
		}

		WaitTime();
	}

	//�I�����̑I�����ꂽ�����`�F�b�N
	constexpr void ChoiceSelectCheck(std::int32_t &ChoiceCursorPosY) noexcept {

		for (std::int32_t i = 0; i < 2; i++) {
			if (ChoiceCursorPosY == ChoicePosY[i] && MouseAndKey::CheckMouseAndKeyEnter())
				ChoiceSelect(EndFlag * 2 + i);
		}
	}
}

//�I���������֐�
void ChoiceSelect(const int RoutteNumber) noexcept {

	ChoiceRead();

	std::int32_t ChoiceCursorPosY = ChoicePosY[0];

	using namespace MouseState;
	using namespace KeyState;

	while (EndFlag == RoutteNumber) {
		DrawChoice(ChoiceCursorPosY);
		ChoiceCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseMoveTemplate(ChoiceCursorPosY, (ChoicePosY[1] - CursorMove), CursorMove, 2, ConfigData.MouseAndKeyFlag)
															: KeyMoveTemplate(ChoiceCursorPosY, ChoicePosY[0], ChoicePosY[1], CursorMove, ConfigData.MouseAndKeyFlag);
		ChoiceSelectCheck(ChoiceCursorPosY);
		ShortCutKey();
		ScreenClear();

		//�Q�[���I���m�F�E�C���h�E
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}

}
