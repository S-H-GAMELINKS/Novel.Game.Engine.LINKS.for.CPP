#ifndef MATERIAL_MANAGER_CPP_
#define MATERIAL_MANAGER_CPP_

#include "ConstantExpressionVariable.hpp"
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

namespace fs = boost::filesystem;

namespace {

	//各素材ファイルパス取得
	const std::vector<std::string> AllFilePath(const std::string& Path) {

		std::vector<std::string> Container;

		const fs::path path(Path);

		for (const auto& p : boost::make_iterator_range(fs::recursive_directory_iterator(path), {})) {
			if (!fs::is_directory(p.path()))
				Container.emplace_back(std::move(Path + "/" + p.path().filename().string()));
		}

		return Container;
	}
}

//各種素材読込テンプレート関数
template <typename T, typename Func>
constexpr std::vector<T> MaterialLoad(std::vector<T> Material, const std::string& Path, Func&& Loader) noexcept {

	//サウンドデータの読み込み形式
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);

	std::vector<std::string> Container = AllFilePath(Path);

	for (auto&& c : Container)
		Material.emplace_back(std::move(Loader(c.c_str())));

	return Material;
}

//スクリプト読込関数
std::vector<std::string> ScriptRead(unsigned int EndFlag) noexcept;

#endif // MATERIAL_MANAGER_CPP_