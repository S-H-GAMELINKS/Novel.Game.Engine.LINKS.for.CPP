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

	//�e�f�ރt�@�C���p�X�擾
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

//�e��f�ޓǍ��e���v���[�g�֐�
template <typename T, typename Func>
constexpr std::vector<T> MaterialLoad(std::vector<T> Material, const std::string& Path, Func&& Loader) noexcept {

	//�T�E���h�f�[�^�̓ǂݍ��݌`��
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);

	std::vector<std::string> Container = AllFilePath(Path);

	for (auto&& c : Container)
		Material.emplace_back(std::move(Loader(c.c_str())));

	return Material;
}

//�X�N���v�g�Ǎ��֐�
std::vector<std::string> ScriptRead(unsigned int EndFlag) noexcept;

#endif // MATERIAL_MANAGER_CPP_