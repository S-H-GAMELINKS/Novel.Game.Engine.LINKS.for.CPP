//Material Manage Source
#include "DxLib.h"
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace {

	//各種素材ファイル確認関数
	bool CheckMaterialExistence(const std::string& FilePath) {
		std::ifstream Material(FilePath, std::ios_base::in);
		return Material.is_open();
	}

	//背景画像読込関数
	void MaterialLoadBackGround(std::array<int, 99>& BackGround) {

		std::string FilePath = "DATA/BACKGROUND/BG";
		std::string FileFormat = ".png";
		std::string FileName = "";

		for (std::int32_t i = 0; i < 99; i++) {

			std::ostringstream Num;

			Num << std::setfill('0') << std::setw(2) << i + 1;

			FileName = (FilePath + Num.str() + FileFormat);

			if (CheckMaterialExistence(FileName))
				BackGround[i] = DxLib::LoadGraph(FileName.c_str());
		}
	}

	//立ち絵素材読込関数
	void MaterialLoadCharacter(std::array<int, 99>& Character) {

		std::string FilePath = "DATA/CHARACTER/CHAR";
		std::string FileFormat = ".png";
		std::string FileName = "";

		for (std::int32_t i = 0; i < 99; i++) {

			std::ostringstream Num;

			Num << std::setfill('0') << std::setw(2) << i + 1;

			FileName = (FilePath + Num.str() + FileFormat);

			if (CheckMaterialExistence(FileName))
				Character[i] = DxLib::LoadGraph(FileName.c_str());
		}
	}

	//BGM読込関数
	void MaterialLoadBackGroundMusic(std::array<int, 99>& BackGroundMusic) {

		std::string FilePath = "DATA/BACKGROUNDMUSIC/BGM";
		std::string FileFormat = ".ogg";
		std::string FileName = "";

		for (std::int32_t i = 0; i < 99; i++) {

			std::ostringstream Num;

			Num << std::setfill('0') << std::setw(2) << i + 1;

			FileName = (FilePath + Num.str() + FileFormat);

			if (CheckMaterialExistence(FileName))
				BackGroundMusic[i] = DxLib::LoadSoundMem(FileName.c_str());
		}
	}

	//SE読込関数
	void MaterialLoadSoundEffect(std::array<int, 99>& SoundEffect) {

		std::string FilePath = "DATA/SOUNDEFFECT/SE";
		std::string FileFormat = ".ogg";
		std::string FileName = "";

		for (std::int32_t i = 0; i < 99; i++) {

			std::ostringstream Num;

			Num << std::setfill('0') << std::setw(2) << i + 1;

			FileName = (FilePath + Num.str() + FileFormat);

			if (CheckMaterialExistence(FileName))
				SoundEffect[i] = DxLib::LoadSoundMem(FileName.c_str());
		}
	}

	//動画読込関数
	template <typename T>
	void MaterialLoadMovie(std::array<T, 99>& Movie) {

		std::string FilePath = "DATA/MOVIE/MOVIE";
		std::string FileFormat = ".wmv";
		std::string FileName = "";

		for (std::int32_t i = 0; i < 99; i++) {

			std::ostringstream Num;

			Num << std::setfill('0') << std::setw(2) << i + 1;

			FileName = (FilePath + Num.str() + FileFormat);

			if (CheckMaterialExistence(FileName))
				Movie[i] = FileName;
		}
	}
}

//各種素材読込関数
void MaterialLoad(std::array<int, 99>& BackGround, std::array<int, 99>& Character, std::array<int, 99>& BackGroundMusic, std::array<int, 99>& SoundEffect, std::array<std::string, 99>& Movie, std::int32_t& GameTitleGraph) {

	//サウンドデータの読み込み形式
	DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);

	//背景画像読込関数
	MaterialLoadBackGround(BackGround);

	//立ち絵画像読込関数
	MaterialLoadCharacter(Character);

	//BGM読込関数
	MaterialLoadBackGroundMusic(BackGroundMusic);

	//SE読込関数
	MaterialLoadSoundEffect(SoundEffect);

	//動画読込関数
	MaterialLoadMovie(Movie);

	//タイトル画面読込
	GameTitleGraph = DxLib::LoadGraph("DATA/BACKGROUND/TITLE.png");
}

//スクリプト読込関数
void ScriptRead(std::vector<std::string>& Script, unsigned int EndFlag) {

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

	if (0 <= EndFlag && EndFlag < 15) {
		std::fstream file(ScriptFilePath[EndFlag], std::ios_base::in);

		for (std::string line; std::getline(file, line); ) if (!line.empty()) Script.emplace_back(std::move(line));
	}
}
