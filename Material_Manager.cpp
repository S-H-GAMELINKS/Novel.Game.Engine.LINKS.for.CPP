//Material Manage Source
#include "DxLib.h"
#include <vector>
#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace {

	//Šeí‘fŞƒtƒ@ƒCƒ‹Šm”FŠÖ”
	bool CheckMaterialExistence(const std::string& FilePath) {
		std::ifstream Material(FilePath, std::ios_base::in);
		return Material.is_open();
	}

	//”wŒi‰æ‘œ“ÇŠÖ”
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

	//—§‚¿ŠG‘fŞ“ÇŠÖ”
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

	//BGM“ÇŠÖ”
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

	//SE“ÇŠÖ”
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

	//“®‰æ“ÇŠÖ”
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

//Šeí‘fŞ“ÇŠÖ”
void MaterialLoad(std::array<int, 99>& BackGround, std::array<int, 99>& Character, std::array<int, 99>& BackGroundMusic, std::array<int, 99>& SoundEffect, std::array<std::string, 99>& Movie) {

	//”wŒi‰æ‘œ“ÇŠÖ”
	MaterialLoadBackGround(BackGround);

	//—§‚¿ŠG‰æ‘œ“ÇŠÖ”
	MaterialLoadCharacter(Character);

	//BGM“ÇŠÖ”
	MaterialLoadBackGroundMusic(BackGroundMusic);

	//SE“ÇŠÖ”
	MaterialLoadSoundEffect(SoundEffect);

	//“®‰æ“ÇŠÖ”
	MaterialLoadMovie(Movie);
}

//ƒXƒNƒŠƒvƒg“ÇŠÖ”
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
