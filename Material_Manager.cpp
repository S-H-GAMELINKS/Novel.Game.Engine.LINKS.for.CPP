//Material Manage Source
#include "DxLib.h"
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

	//各種素材読込テンプレート関数
	template <typename T, typename Func>
	void MaterialLoadTemplate(std::array<T, MaterialMax>& Material, const std::string& FilePath, const std::string& FileFormat, Func&& loader) noexcept {
		for (std::int32_t i = 0; i < MaterialMax; i++) {
			if (CheckMaterialExistence(MaterialPathCalc(i, FilePath, FileFormat)))
				Material[i] = loader(MaterialPathCalc(i, FilePath, FileFormat));
		}
	}
}

//各種素材読込関数
void MaterialLoad(std::array<int, MaterialMax>& BackGround, std::array<int, MaterialMax>& Character, std::array<int, MaterialMax>& BackGroundMusic, std::array<int, MaterialMax>& SoundEffect, std::array<std::string, MaterialMax>& Movie, std::int32_t& GameTitleGraph) noexcept {

	//サウンドデータの読み込み形式
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);

	//背景画像読込関数
	MaterialLoadTemplate(BackGround, "DATA/BACKGROUND/BG", ".png", [](const std::string& path) {return DxLib::LoadGraph(path.c_str()); });

	//立ち絵画像読込関数
	MaterialLoadTemplate(Character, "DATA/CHARACTER/CHAR", ".png", [](const std::string& path) {return DxLib::LoadGraph(path.c_str()); });

	//BGM読込関数
	MaterialLoadTemplate(BackGroundMusic, "DATA/BACKGROUNDMUSIC/BGM", ".ogg", [](const std::string& path) {return DxLib::LoadSoundMem(path.c_str());});

	//SE読込関数
	MaterialLoadTemplate(SoundEffect, "DATA/SOUNDEFFECT/SE", ".ogg", [](const std::string& path) {return DxLib::LoadSoundMem(path.c_str()); });

	//動画読込関数
	MaterialLoadTemplate(Movie, "DATA/MOVIE/MOVIE", ".wmv", [](const std::string& path) {return std::move(path); });

	//タイトル画面読込
	GameTitleGraph = DxLib::LoadGraph("DATA/BACKGROUND/TITLE.png");
}

//スクリプト読込関数
void ScriptRead(std::vector<std::string>& Script, unsigned int EndFlag) noexcept {

	Script.clear();

	const char* ScriptFilePath[] = {
		"DATA/STR/LINKS.txt",
		"DATA/STR/A.txt",
		"DATA/STR/B.txt",
		"DATA/STR/C.txt",
		"DATA/STR/D.txt",
		"DATA/STR/E.txt",
		"DATA/STR/F.txt",
		"DATA/STR/G.txt",
		"DATA/STR/H.txt",
		"DATA/STR/I.txt",
		"DATA/STR/J.txt",
		"DATA/STR/K.txt",
		"DATA/STR/L.txt",
		"DATA/STR/M.txt",
		"DATA/STR/N.txt"
	};

	if (1 <= EndFlag && EndFlag < 16) {
		std::fstream file(ScriptFilePath[EndFlag - 1], std::ios_base::in);

		for (std::string line; std::getline(file, line); ) if (!line.empty()) Script.emplace_back(std::move(line));
	}
}
