// Skip Auto Source

#include "DxLib.h"
#include "Utility.h"
#include "ConstantExpressionVariable.h"
#include <thread>
#include <chrono>

// 既読スキップ/スキップ/オート変数
extern int SkipAndAutoFlag;

// 既読スキップ/スキップ/オート切り替え関数
void SkipAndAutoTask(const std::int32_t& Num) {

	if (Num == 0) {
		if (IDYES == MessageBoxYesNo("オート/スキップを停止しますか？")) {
			SkipAndAutoFlag = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	if (Num == 1) {
		if (IDYES == MessageBoxYesNo("スキップを実行しますか？")) {
			SkipAndAutoFlag = 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}

	if (Num == 2) {
		if (IDYES == MessageBoxYesNo("オートを実行しますか？")) {
			SkipAndAutoFlag = 2;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}
}
