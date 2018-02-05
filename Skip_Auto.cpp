// Skip Auto Source

#include "DxLib.h"
#include "Utility.h"
#include "ConstantExpressionVariable.h"
#include <array>
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

// 既読スキップ/スキップ/オート変数
extern int SkipAndAutoFlag;

//既読スキップ判定用変数
std::array<std::int32_t, 15> SkipData;

//既読スキップ
struct alignas(4) SkipData_t {
	std::int32_t LINKS;		//メインルートの既読情報
	std::int32_t A;			//Aルートの既読情報
	std::int32_t B;			//Bルートの既読情報
	std::int32_t C;			//Cルートの既読情報
	std::int32_t D;			//Dルートの既読情報
	std::int32_t E;			//Eルートの既読情報
	std::int32_t F;			//Fルートの既読情報
	std::int32_t G;			//Gルートの既読情報
	std::int32_t H;			//Hルートの既読情報
	std::int32_t I;			//Iルートの既読情報
	std::int32_t J;			//Jルートの既読情報
	std::int32_t K;			//Kルートの既読情報
	std::int32_t L;			//Lルートの既読情報
	std::int32_t M;			//Mルートの既読情報
	std::int32_t N;			//Nルートの既読情報
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
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.dat", "rb");
	if (nullptr == Fp) {
		return 0;
	}
	fread(&Data, sizeof(SkipData_t), 1, Fp);

	SkipData[0] = Data.LINKS;
	SkipData[1] = Data.A;
	SkipData[2] = Data.B;
	SkipData[3] = Data.C;
	SkipData[4] = Data.D;
	SkipData[5] = Data.E;
	SkipData[6] = Data.F;
	SkipData[7] = Data.G;
	SkipData[8] = Data.H;
	SkipData[9] = Data.I;
	SkipData[10] = Data.J;
	SkipData[11] = Data.K;
	SkipData[12] = Data.L;
	SkipData[13] = Data.M;
	SkipData[14] = Data.N;

	fclose(Fp);
	return 0;
}

//SKIP_READ SAVE関数
int SkipDataSave() noexcept {
	SkipData_t Data = {
		SkipData[0], SkipData[1], SkipData[2], SkipData[3], SkipData[4], SkipData[5],
		SkipData[6], SkipData[7], SkipData[8], SkipData[9], SkipData[10], SkipData[11],
		SkipData[12], SkipData[13], SkipData[14]
	};

	FILE *Fp;
	fopen_s(&Fp, "DATA/SAVE/SKIP_READ.dat", "wb");//バイナリファイルを開く
	if (nullptr == Fp) {//エラーが起きたらnullptrを返す
		return 0;
	}
	fwrite(&Data, sizeof(SkipData_t), 1, Fp); // SkipData_t構造体の中身を出力
	fclose(Fp);
	return 0;
}

// 既読スキップ/スキップ/オート切り替え関数
void SkipAndAutoTask(const std::int32_t& Num) noexcept {

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
			SkipDataCheck(EndFlag);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}
}
