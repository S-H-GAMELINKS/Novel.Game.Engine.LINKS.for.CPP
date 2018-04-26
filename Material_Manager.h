#ifndef MATERIAL_MANAGER_CPP_
#define MATERIAL_MANAGER_CPP_

#include "ConstantExpressionVariable.h"
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace {

	//各種素材ファイル確認関数
	bool CheckMaterialExistence(const std::string& FilePath) noexcept {
		std::ifstream Material(FilePath, std::ios_base::in);
		return Material.is_open();
	}

	//各種素材ファイルパス処理
	std::string MaterialPathCalc(const std::int32_t& i, const std::string& FilePath, const std::string& FileFormat) noexcept {
		std::ostringstream Num;

		Num << std::setfill('0') << std::setw(2) << i + 1;

		return (FilePath + Num.str() + FileFormat);
	}
}

//各種素材読込テンプレート関数
template <typename T, typename Func>
std::array<T, MaterialMax> MaterialLoad(std::array<T, MaterialMax>& Material, const std::string& FilePath, const std::string& FileFormat, Func&& Loader) noexcept {

	//サウンドデータの読み込み形式
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);

	for (std::int32_t i = 0; i < MaterialMax; i++) {
		if (CheckMaterialExistence(MaterialPathCalc(i, FilePath, FileFormat)))
			Material[i] = Loader(MaterialPathCalc(i, FilePath, FileFormat));
	}

	return Material;
}

//スクリプト読込関数
void ScriptRead(std::vector<std::string>& Script, unsigned int EndFlag) noexcept;

#endif // MATERIAL_MANAGER_CPP_