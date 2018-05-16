// Skip Auto Source

#include "DxLib.h"
#include "Utility.h"
#include "ConstantExpressionVariable.h"
#include <array>
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//文字列描画位置
extern int DrawPointX, DrawPointY;

// 既読スキップ/スキップ/オート変数
extern int SkipAndAutoFlag;

//既読スキップ判定用変数
std::array<std::int32_t, 15> SkipData;

//既読スキップ
struct alignas(4) SkipData_t {
	//既読スキップ判定用変数
	std::array<std::int32_t, 15> SkipFlag;
};

//既読スキップデータ書き込み
void SkipDataWrite(const std::int32_t& RouteNum) noexcept {
	SkipData[RouteNum - 1] = 1;
}

//既読判定
void SkipDataCheck(const std::int32_t& RouteNum) noexcept {
	if (SkipData[RouteNum - 1] == 1)
		SkipAndAutoFlag = 1;
	else
		SkipAndAutoFlag = 0;
}

//既読スキップデータの読込
int SkipDataLoad() noexcept {
	SkipData_t Data;

	FILE *Fp;
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.bat", "rb");
	if (nullptr == Fp) {
		return 0;
	}
	fread(&Data, sizeof(SkipData_t), 1, Fp);

	std::int32_t i = 0;

	for (auto&& d : Data.SkipFlag) {
		SkipData[i] = d;
		i++;
	}

	fclose(Fp);
	return 0;
}

//SKIP_READ SAVE関数
int SkipDataSave() noexcept {

	SkipData_t Data;

	std::int32_t i = 0;

	for (auto&& d : Data.SkipFlag) {
		d = SkipData[i];
		i++;
	}

	FILE *Fp;
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.bat", "wb");//バイナリファイルを開く
	if (nullptr == Fp) {//エラーが起きたらnullptrを返す
		return 0;
	}
	fwrite(&Data, sizeof(SkipData_t), 1, Fp); // SkipData_t構造体の中身を出力
	fclose(Fp);
	return 0;
}

// 既読スキップ/スキップ/オート切り替え関数
void SkipAndAutoTask(const std::int32_t& Num, const int Flag) noexcept {

	if (Num == 0) {
		if (IDYES == MessageBoxYesNo("オート/スキップを停止しますか？")) {
			SkipAndAutoFlag = 0;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	if (Num == 1) {
		if (IDYES == MessageBoxYesNo("スキップを実行しますか？")) {
			SkipAndAutoFlag = 1;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	if (Num == 2) {
		if (IDYES == MessageBoxYesNo("オートを実行しますか？")) {
			SkipAndAutoFlag = 2;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	if (Num == 3) {
		if (IDYES == MessageBoxYesNo("既読スキップを実行しますか？")) {
			SkipDataCheck(Flag);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}
	EndFlag = Flag;
	DrawPointX = 0;
	DrawPointY = 0;
}
