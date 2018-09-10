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

	//キー判定ラムダ式
	auto Check = [](const int& key, const int& mouse) {
		return (DxLib::CheckHitKey(key) == 1 || DxLib::GetMouseInput() == mouse);
	};

	//バックログのナンバー表示
	void BackLogNumberDraw(const std::int32_t& Num) noexcept {
		DxLib::DrawString(0, TitleMenuCursorPosX, (Name + std::to_string(Num + 1)).c_str(), Color);
	}

	//バックログの描画
	std::int32_t BackLogDraw(std::int32_t Num) noexcept {

		if (Num < BackLogCount) {
			DxLib::DrawGraph(0, 0, BackLog[BackLogCount - Num - 1], TRUE);
			BackLogNumberDraw(Num);
			WaitTime();
		}
		else {
			MessageBoxOk("これ以上バッグログはありません");
			Num--;
			WaitTime();
		}
		return Num;
	}

	//バックログ時のキー操作
	bool BackLogKeyMove(std::int32_t& Num, bool BackLogFlag) noexcept {

		if (Check(KEY_INPUT_UP, MOUSE_INPUT_RIGHT))
			Num = (Num < BackLogCount) ? Num + 1 : Num;

		if (Check(KEY_INPUT_DOWN, MOUSE_INPUT_LEFT))
			Num = (Num > 0) ? Num - 1 : 0;

		if (Check(KEY_INPUT_BACK, MOUSE_INPUT_MIDDLE)) {
			if (IDYES == MessageBoxYesNo("バックログを終了しますか？"))
				BackLogFlag = false	;
		}
		return BackLogFlag;
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
				BackLogFlag = BackLog::BackLogKeyMove(Num, BackLogFlag);
				ScreenClear();
			}
		}
		else
			MessageBoxOk("バックログがありません");
	}
	WaitTime();
}
