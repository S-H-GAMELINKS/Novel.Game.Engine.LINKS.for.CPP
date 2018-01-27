// BackLog Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>

namespace {
	std::int32_t BackLogCount = 0;
	std::vector<std::int32_t> BackLog;

	//バックログ取得
	void BackLogGet() {
		DxLib::SetDrawScreen(DX_SCREEN_BACK);
		DxLib::SaveDrawScreenToPNG(0, 0, 640, 480, "DATA/BACKLOG/BACKLOG1.png");
		++BackLogCount;
		BackLog.emplace_back(std::move(DxLib::LoadGraph("DATA/BACKLOG/BACKLOG1.png")));
		DxLib::SetDrawScreen(DX_SCREEN_FRONT);
	}

	//バックログのナンバー表示
	void BackLogNumberDraw(const std::int32_t& Num) {
		std::string name = "バックログ";
		name += std::to_string(Num + 1);
		DxLib::DrawString(0, 400, name.c_str(), DxLib::GetColor(255, 255, 255));
	}

	//バックログの描画
	void BackLogDraw(std::int32_t& Num) {

		if (Num < BackLogCount) {
			DxLib::DrawGraph(0, 0, BackLog[Num], TRUE);
			BackLogNumberDraw(Num);
		}
		else {
			MessageBoxOk("これ以上バッグログはありません");
			Num--;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	//バックログ時のキー操作
	void BackLogKeyMove(std::int32_t& Num, std::int32_t& BackLogFlag) {

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			Num = (Num < BackLogCount) ? Num + 1 : Num;

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			Num = (Num > 0) ? Num - 1 : 0;

		if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1) {
			if (IDYES == MessageBoxYesNo("バックログを終了しますか？"))
				BackLogFlag = 0;
		}
	}
}

//バックログループ
void BackLogLoop() {

	if (IDYES == MessageBoxYesNo("バックログを表示しますか？")) {

		if (!BackLog.empty()) {
			std::int32_t Num = 0;
			std::int32_t BackLogFlag = 1;

			while (BackLogFlag == 1) {
				BackLogDraw(Num);
				BackLogKeyMove(Num, BackLogFlag);
				ScreenClear();
			}
		}
		else
			MessageBoxOk("バックログがありません");
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
}
