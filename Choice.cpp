// Choice Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "BackLog.h"
#include "ConfigMenu.h"
#include "MouseAndKeyState.hpp"
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;

extern ConfigData_t ConfigData;

//選択肢用変数
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

	//選択肢描画関数
	void DrawChoice(std::int32_t& ChoiceCursorPosY) noexcept {

		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);

		//カーソルの描画
		DxLib::DrawString(ChoicePosX, ChoiceCursorPosY, "■", Color);

		for (auto&& i : { 0, 1 })
			DxLib::DrawString(ChoicePosX + CursorMove, ChoicePosY[i], Choice[i].c_str(), Color);
	}

	//選択肢読込関数
	void ChoiceRead() noexcept {
		if (1 <= EndFlag && EndFlag <= 7) {
			for (std::size_t i : {0, 1}) {
				std::ifstream File(ChoiceScript[EndFlag - 1][i], std::ios_base::in);
				std::getline(File, Choice[i]);
			}
		}
	}

	//選択肢分岐処理
	void ChoiceSelect(const int& Num) noexcept {
		if (1 <= EndFlag && EndFlag <= 7) {
			BackLogGet();
			EndFlag = Num;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//選択肢の選択されたかをチェック
	void ChoiceSelectCheck(std::int32_t &ChoiceCursorPosY) noexcept {

		if (ChoiceCursorPosY == ChoicePosY[0] && MouseAndKey::CheckMouseAndKeyEnter())
			ChoiceSelect(EndFlag * 2);

		if (ChoiceCursorPosY == ChoicePosY[1] && MouseAndKey::CheckMouseAndKeyEnter())
			ChoiceSelect(EndFlag * 2 + 1);
	}
}

//選択肢処理関数
void ChoiceSelect(const int RoutteNumber) noexcept {

	ChoiceRead();

	std::int32_t ChoiceCursorPosY = ChoicePosY[0];

	while (EndFlag == RoutteNumber) {
		DrawChoice(ChoiceCursorPosY);
		ChoiceCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseState::ChoiceMouseMove(ChoiceCursorPosY) : KeyState::ChoiceKeyMove(ChoiceCursorPosY);
		ChoiceSelectCheck(ChoiceCursorPosY);
		ShortCutKey();
		ScreenClear();

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}

}
