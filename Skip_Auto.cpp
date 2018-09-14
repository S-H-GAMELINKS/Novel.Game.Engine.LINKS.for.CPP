// Skip Auto Source

#include "DxLib.h"
#include "Utility.hpp"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
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
constexpr void SkipDataCheck(const std::int32_t& RouteNum) noexcept {
	if (SkipData[RouteNum - 1] == 1)
		SkipAndAutoFlag = 1;
	else
		SkipAndAutoFlag = 0;
}

//既読スキップデータ読み込み＆書き込み
template<typename T, std::size_t N>
constexpr std::array<T, N> SkipDataGet(std::array<T, N> ReadData, std::array<T, N> WriteData) {

	auto var = std::begin(WriteData);

	for (auto it = std::begin(ReadData); it != std::end(ReadData); it++) {
		*var = *it;
		var++;
	}

	return WriteData;
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

	SkipData = SkipDataGet(Data.SkipFlag, SkipData);

	fclose(Fp);
	return 0;
}

//SKIP_READ SAVE関数
int SkipDataSave() noexcept {

	SkipData_t Data;

	for (auto&& d : Data.SkipFlag)
		d = 0;

	Data.SkipFlag = SkipDataGet(SkipData, Data.SkipFlag);

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

	std::array<std::string, 4> Message = { "オート/スキップを停止しますか？" , "スキップを実行しますか？",
											"オートを実行しますか？", "既読スキップを実行しますか？" };

	for (std::int32_t i = 0; i < 4; i++) {
		if (Num == i && Num == 3) {
			if (IDYES == MessageBoxYesNo(Message[i].c_str())) {
				SkipDataCheck(Flag);
			}
			WaitTime();
		} else if (Num == i) {
			if (IDYES == MessageBoxYesNo(Message[i].c_str())) {
				SkipAndAutoFlag = i;
			}
			WaitTime();
		}
	}

	EndFlag = Flag;
	DrawPointX = 0;
	DrawPointY = 0;
}
