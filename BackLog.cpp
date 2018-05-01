// BackLog Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <string>
#include <vector>
#include <thread>
#include <chrono>

namespace BackLog {
	std::int32_t BackLogCount = 0;
	std::vector<std::int32_t> BackLog;
	std::string Name = "バックログ";

	//バックログのナンバー表示
	void BackLogNumberDraw(const std::int32_t& Num) noexcept {
		Name += std::to_string(Num + 1);
		DxLib::DrawString(0, 400, Name.c_str(), Color);
	}

	//バックログの描画
	std::int32_t BackLogDraw(std::int32_t Num) noexcept {

		if (Num < BackLogCount) {
			DxLib::DrawGraph(0, 0, BackLog[BackLogCount - Num - 1], TRUE);
			BackLogNumberDraw(Num);
		}
		else {
			MessageBoxOk("これ以上バッグログはありません");
			Num--;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

		return Num;
	}

	//バックログ時のキー操作
	void BackLogKeyMove(std::int32_t& Num, bool& BackLogFlag) noexcept {

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			Num = (Num < BackLogCount) ? Num + 1 : Num;

		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			Num = (Num > 0) ? Num - 1 : 0;

		if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1) {
			if (IDYES == MessageBoxYesNo("バックログを終了しますか？"))
				BackLogFlag = false;
		}
	}
}

//バックログ取得
void BackLogGet() noexcept {
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	DxLib::SaveDrawScreenToPNG(0, 0, 640, 480, "DATA/BACKLOG/BACKLOG1.png");
	++BackLog::BackLogCount;
	BackLog::BackLog.emplace_back(std::move(DxLib::LoadGraph("DATA/BACKLOG/BACKLOG1.png")));
	DxLib::SetDrawScreen(DX_SCREEN_FRONT);
}

//バックログループ
void BackLogLoop() noexcept {

	if (IDYES == MessageBoxYesNo("バックログを表示しますか？")) {

		if (!BackLog::BackLog.empty()) {
			std::int32_t Num = 0;
			bool BackLogFlag = true;

			while (BackLogFlag) {
				Num = BackLog::BackLogDraw(Num);
				BackLog::BackLogKeyMove(Num, BackLogFlag);
				ScreenClear();
			}
		}
		else
			MessageBoxOk("バックログがありません");
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}
