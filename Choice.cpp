// Choice Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <string>
#include <fstream>
#include <thread>
#include <chrono>

extern int EndFlag;

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
	void DrawChoice(unsigned int color, std::int32_t& choice_y) {

		//カーソルの描画
		DxLib::DrawString(choice_pos_x, choice_y, "■", color);

		for (auto&& i : { 0, 1 })
			DxLib::DrawString(choice_pos_x + cursor_move, choice_pos_y[i], Choice[i].c_str(), color);
	}

	//選択肢読込関数
	void ChoiceRead() {
		if (1 <= EndFlag && EndFlag <= 7) {
			for (std::size_t i : {0, 1}) {
				std::ifstream file(ChoiceScript[EndFlag - 1][i], std::ios_base::in);
				std::getline(file, Choice[i]);
			}
		}
	}

	//選択肢キー操作
	void ChoiceKeyMove(std::int32_t& cursor_y) {

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (choice_pos_y[1] == cursor_y) ? choice_pos_y[0] : cursor_y + cursor_move;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (choice_pos_y[0] == cursor_y) ? choice_pos_y[1] : cursor_y - cursor_move;

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	//選択肢(↑)選択時処理
	void ChoiceSelectUp() {
		if (1 <= EndFlag && EndFlag <= 7) {
			EndFlag *= 2;
		}
	}

	//選択肢(↑)選択時処理
	void ChoiceSelectDown() {
		if (1 <= EndFlag && EndFlag <= 7) {
			EndFlag = EndFlag * 2 + 1;
		}
	}

	//選択肢の選択されたかをチェック
	void ChoiceSelectCheck(std::int32_t cursor_y) {

		if (cursor_y == choice_pos_y[0] && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			ChoiceSelectUp();
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}

		if (cursor_y == choice_pos_y[1] && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			ChoiceSelectDown();
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}
	}
}

//選択肢処理関数
void ChoiceSelect(int RoutteNumber) {

	ChoiceRead();

	std::int32_t cursor_y = choice_pos_y[0];
	unsigned int color = DxLib::GetColor(255, 255, 255);

	while (EndFlag == RoutteNumber) {
		DrawChoice(color, cursor_y);
		ChoiceKeyMove(cursor_y);
		ChoiceSelectCheck(cursor_y);
		ScreenClear();
		GameEndMessageBox();
	}

}
