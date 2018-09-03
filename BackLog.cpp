// BackLog Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include <string>
#include <vector>
#include <thread>
#include <chrono>

namespace BackLog {
	std::int32_t BackLogCount = 0;
	std::vector<std::int32_t> BackLog;
	const std::string Name = "バックログ";

	//判定用ラムダ式
	auto MouseCheck = [](const int& Args) {return (DxLib::CheckHitKey(Args) == 1); };
	auto KeyCheck = [](const int& Args) {return (DxLib::GetMouseInput() == Args); };

	//バックログのナンバー表示
	void BackLogNumberDraw(const std::int32_t& Num) noexcept {
		DxLib::DrawString(0, 400, (Name + std::to_string(Num + 1)).c_str(), Color);
	}

	//バックログの描画
	std::int32_t BackLogDraw(std::int32_t Num) noexcept {

		if (Num < BackLogCount) {
			DxLib::DrawGraph(0, 0, BackLog[BackLogCount - Num - 1], TRUE);
			BackLogNumberDraw(Num);
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
		else {
			MessageBoxOk("これ以上バッグログはありません");
			Num--;
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
		return Num;
	}

	//バックログ時のキー操作
	void BackLogKeyMove(std::int32_t& Num, bool& BackLogFlag) noexcept {

		if (MouseCheck(KEY_INPUT_UP) || KeyCheck(MOUSE_INPUT_RIGHT))
			Num = (Num < BackLogCount) ? Num + 1 : Num;

		if (MouseCheck(KEY_INPUT_DOWN) || KeyCheck(MOUSE_INPUT_LEFT))
			Num = (Num > 0) ? Num - 1 : 0;

		if (DxLib::CheckHitKey(KEY_INPUT_BACK) == 1) {
			if (IDYES == MessageBoxYesNo("バックログを終了しますか？"))
				BackLogFlag = false	;
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
