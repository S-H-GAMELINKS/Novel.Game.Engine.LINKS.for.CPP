//Script Tag Task Maneger Source
#define _SCL_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 


#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "Choice.hpp"
#include "ConfigMenu.hpp"
#include "BackLog.hpp"
#include "MouseAndKeyState.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <thread>
#include <chrono>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace boost::xpressive;

extern int DrawPointX, DrawPointY;	// ������`��̈ʒu
extern int Sp, Cp;	// �Q�Ƃ��镶����ԍ��ƕ����񒆂̕����|�C���^

//�I���t���O
extern int EndFlag;

//�e��f�ރn���h��
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle, ImageEffectHandle;

// ���ǃX�L�b�v/�X�L�b�v/�I�[�g�ϐ�
extern int SkipAndAutoFlag;

extern struct ConfigData_t ConfigData;

//�f�ޗp�G�C���A�X
template <class T>
using Material = std::vector<T>;

//�X�N���v�g�p�G�C���A�X
using Script = std::vector<std::string>;

//�����G�폜���Q�[���I�[�o�[�p�G�C���A�X
using unique = std::unique_ptr<int>;

//�^�O���K�\���p�G�C���A�X
using ScriptTag = const std::pair<std::string, std::string>;

namespace ScriptTask {

	char OneMojiBuf[3];	// �P�������ꎞ�L���z��

	//�X�N���v�g�^�O����
	std::vector<std::string> LoadingScriptTag() {

		std::vector<std::string> Str, Container, Temp;
		
		std::fstream file("DATA/STR/ScriptTag.txt", std::ios_base::in);
		
		for (std::string line; std::getline(file, line); ) if (!line.empty()) Str.emplace_back(std::move(line));
		
			for (auto&& s : Str) {
				boost::algorithm::split(Temp, s, boost::is_any_of(",")); // �J���}�ŕ���
			
				for (auto&& t : Temp)
				Container.emplace_back(std::move(t));
			}

		file.close();

		return Container;
	}

	//�X�N���v�g�^�O���f����
	std::vector<std::pair<std::string, std::string>> SettingScriptTag(const std::vector<std::string> Container) {

		std::vector<std::pair<std::string, std::string>> ScriptTag;
		
		for (unsigned int i = 0, j = 0; i < Container.size(); i += 2, j++)
			ScriptTag.emplace_back(std::move(std::make_pair(Container[i], Container[i + 1])));

		return ScriptTag;
	}

	//�^�O���K�\���ꎞ�i�[
	std::vector<std::string> ScriptTagContainer = LoadingScriptTag();

	//�^�O���K�\��
	std::vector<std::pair<std::string, std::string>> Tag = SettingScriptTag(ScriptTagContainer);

	// ���s�֐�
	bool Kaigyou() noexcept {
		int TempGraph;

		// �`��s�ʒu���������
		DrawPointY++;

		// �`�����ŏ��ɖ߂�
		DrawPointX = 0;

		// ������ʂ���͂ݏo��Ȃ��ʂ��X�N���[��������
		if (DrawPointY * MojiSize + MojiSize > 480)
		{
			// �e���|�����O���t�B�b�N�̍쐬
			TempGraph = MakeGraph(640, 480);

			// ��ʂ̓��e���ہX�R�s�[����
			GetDrawScreenGraph(0, 0, 640, 480, TempGraph);

			// ��s����ɓ\��t����
			DrawGraph(0, -MojiSize, TempGraph, FALSE);

			// ��ԉ��̍s�̕��������Ŗ��߂�
			DrawBox(0, 480 - MojiSize, 640, 480, 0, TRUE);

			// �`��s�ʒu���������
			DrawPointY--;

			// �O���t�B�b�N���폜����
			DeleteGraph(TempGraph);
		}

		return true;
	}

	//�������x�֐�
	void DrawSpeed(const int& Speed, const int& SpeedVolume) {
		std::this_thread::sleep_for(std::chrono::milliseconds(SpeedVolume * Speed / 100));
	}

	//������`�摬�x
	constexpr void DrawScriptSpeed() noexcept {
		switch (SkipAndAutoFlag) {
		case 0:
			DrawSpeed(StringSpeedAuto, ConfigData.ScriptDrawSpeedVolume);
			break;

		case 1:
			DrawSpeed(StringSpeedSkip, ConfigData.SkipSpeedVolumeCount);
			break;

		case 2:
			DrawSpeed(StringSpeedAuto, ConfigData.AutoSpeedVolume);
			break;
		}
	}

	//������`��֐�
	void DrawScript(Script& Script) noexcept {

		std::string str = "";

		if (Script[Sp] == str)
			Sp++;
		else {
			// �P�����������o��
			OneMojiBuf[0] = Script[Sp][Cp];
			OneMojiBuf[1] = Script[Sp][Cp + 1];
			OneMojiBuf[2] = '\0';

			// �P�����`��
			DrawString(DrawPointX * MojiSize, DrawPointY * MojiSize, OneMojiBuf, Color);

			// �Q�ƕ����ʒu���Q�o�C�g���߂�
			Cp += 2;

			// �J�[�\�����ꕶ�����i�߂�
			DrawPointX++;

			// �����҂�
			DrawScriptSpeed();
		}
	}

	//�f�ޔԍ�����
	int MaterialNumCheck(Script& Script, const std::pair<std::string, std::string>& Tag) {

		std::string str = Script[Sp];

		smatch what;

		std::array<std::string, 2> ScriptTagContainer = { Tag.first, Tag.second };

		for (std::int32_t i = 0; i < 2; i++) {

			sregex rex = sregex::compile(ScriptTagContainer[i]);

			if (regex_search(str, what, rex)) {

				std::string text(what[1]);
				int n = std::stoi(text);

				Script[Sp] = regex_replace(str, rex, "");
				Cp = 0;

				return n - 1;
			}
		}

		return 0;
	}

	//�w�i�摜���C���[�W�G�t�F�N�g�`��֐�
	template <typename T, typename Func>
	constexpr bool DrawImages(Script& Script, Material<T>& Material, Func&& DrawFunc, T& Handle, const std::pair<std::string, std::string> Tag) noexcept {
		Handle = Material[MaterialNumCheck(Script, Tag)];
		DrawFunc(Handle);
		return true;
	}

	//�����G�폜�����֐�
	void RemoveCharacterGraph() noexcept {
		unique CharacterDummy = std::make_unique<int>(DxLib::DerivationGraph(CharacterPosX, CharacterPosY, CharacterGraphSizeWidth, CharacterGraphSizeHeight, BackGroundHandle));
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, *CharacterDummy, true);
	}

	//�����G�`��֐�
	bool DrawCharacter(Script& Script, Material<int>& Character) noexcept {
		Cp++;

		ScriptTask::RemoveCharacterGraph();

		CharacterHandle = Character[MaterialNumCheck(Script, Tag[1])];
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, CharacterHandle, TRUE);

		return true;
	}

	//���ʃZ�b�g�֐�
	void ChangeSoundVolumne() {
			DxLib::ChangeVolumeSoundMem(255 * ConfigData.BackGroundMusicVolume / 100, BackGroundMusicHandle);
			DxLib::ChangeVolumeSoundMem(255 * ConfigData.SoundEffectVolume / 100, SoundEffectHandle);
	}

	//�����Đ��m�F�֐�
	void CheckSoundPlay(const int& Handle) {
		//BGM�Đ����̏ꍇ�́ABGM���~����
		if (DxLib::CheckSoundMem(Handle))
			DxLib::StopSoundMem(Handle);
	}

	//�����Đ��֐�
	template <typename T>
	bool PlaySounds(Script& Script, Material<int>& Material, T& Handle, const T& PlayType, const std::pair<std::string, std::string> Tag) noexcept {

		CheckSoundPlay(Handle);

		Cp++;
		Handle = Material[MaterialNumCheck(Script, Tag)];

		ChangeSoundVolumne();

		DxLib::PlaySoundMem(Handle, PlayType);

		return true;
	}

	//����Đ��֐�
	bool PlayMovie(Script& Script, Material<std::string>& Movie) noexcept {
		Cp++;
		DxLib::PlayMovie(Movie[MaterialNumCheck(Script, Tag[4])].c_str(), 1, DX_MOVIEPLAYTYPE_BCANCEL);

		return true;
	}

	//��ʃN���A�����֐�
	bool ClearScreen(Script& Script) noexcept {
		BackLogGet();
		ClearDrawScreen();
		BackGroundHandle = 0;
		CharacterHandle = 0;
		DrawPointY = 0;
		DrawPointX = 0;

		std::string str = Script[Sp];

		sregex rex = sregex::compile("R");
		smatch what;

		regex_search(str, what, rex);

		Script[Sp] = regex_replace(str, rex, "");
		Cp = 0;

		return true;
	}

	//�R�����g�����֐�
	void Comment(Script& Script) noexcept {
		if (Script[Sp][Cp] == '/') {
			Cp = 0;
			Sp++;
		}
	}

	//�N���b�N�҂������֐�
	constexpr bool ClickWait() noexcept {
		if (SkipAndAutoFlag == 0) {
			DxLib::WaitKey();
			if (MouseAndKey::CheckMouseAndKeyEnter())
				Cp = 0;//Cp++;
		}

		if (SkipAndAutoFlag == 1)
			Cp++;

		if (SkipAndAutoFlag == 2) {
			std::this_thread::sleep_for(std::chrono::milliseconds((StringSpeedAuto * StringSpeedWait * ConfigData.AutoSpeedVolume / 100)));
			Cp++;
		}

		return true;
	}

	//�x������
	bool WaitTime() {

		std::this_thread::sleep_for(std::chrono::seconds(WaitGameTime));

		return true;
	}

	//�Q�[���I�[�o�[��ʂ̕`��
	bool GameOverScreenDraw() {
		unique GameOverHandle = std::make_unique<std::int32_t>(DxLib::LoadGraph("DATA/BACKGROUND/GAMEOVER.png"));
		DxLib::DrawGraph(0, 0, *GameOverHandle, TRUE);

		return true;
	}

	//�G���f�B���O����̍Đ�
	bool DrawEndingMovie() {

		DxLib::PlayMovie("DATA/MOVIE/ENDING.wmv", 1, DX_MOVIEPLAYTYPE_NORMAL);

		return true;
	}

	//�����֌W�̏�����~
	bool StopSounds(const std::int32_t& SoundHandle) {

		DxLib::StopSoundMem(SoundHandle);

		return true;
	}

	//�^�O�`�F�b�N�֐�
	bool ScriptTagCheck(const Script& Script, ScriptTag& Tag) {

		std::array<std::string, 2> Container = { Tag.first, Tag.second };
		
		smatch what;

		for (std::int32_t i = 0; i < 2; i++) {

			sregex rex = sregex::compile(Container[i]);

			if (regex_search(Script[Sp], what, rex))
				return true;
		}

		return false;
	}

	//�e��f�ޕ`��
	bool DrawMaterial(Material<std::string>& Script, Material<int>& BackGround, Material<int>& Character, Material<int>& BackGroundMusic, Material<int>& SoundEffect, Material<std::string>& Movie, Material<int>& ImageEffect) {
		if (ScriptTask::ScriptTagCheck(Script, Tag[0]))		//�w�i�摜�`��
			return ScriptTask::DrawImages(Script, BackGround, [](int Handle) {DxLib::DrawGraph(0, 0, Handle, TRUE); }, BackGroundHandle, Tag[0]);

		if (ScriptTask::ScriptTagCheck(Script, Tag[1])) 	//�����G�摜�`��
			return ScriptTask::DrawCharacter(Script, Character);

		if (ScriptTask::ScriptTagCheck(Script, Tag[2])) 	//BGM�Đ�
			return ScriptTask::PlaySounds(Script, BackGroundMusic, BackGroundMusicHandle, DX_PLAYTYPE_LOOP, Tag[2]);

		if (ScriptTask::ScriptTagCheck(Script, Tag[3])) 	//SE�Đ�
			return ScriptTask::PlaySounds(Script, SoundEffect, SoundEffectHandle, DX_PLAYTYPE_BACK, Tag[3]);

		if (ScriptTask::ScriptTagCheck(Script, Tag[4])) 	//����Đ�
			return ScriptTask::PlayMovie(Script, Movie);

		if (ScriptTask::ScriptTagCheck(Script, Tag[5])) 	//�C���[�W�G�t�F�N�g�`��
			return ScriptTask::DrawImages(Script, ImageEffect, [](int Handle) { DxLib::DrawGraph(0, 0, Handle, TRUE); }, ImageEffectHandle, Tag[5]);

		return false;
	}

	//�V�X�e���^�O�`�F�b�N�֐�
	bool SystemTag(Material <std::string>& Script, ScriptTag& Tag) {

		std::string Str = Script[Sp];

		smatch what;

		std::array<std::string, 2> ScriptTagContainer = { Tag.first, Tag.second };

		for (int i = Cp; i < Script[Sp].length(); i++) {
			if (Script[Sp][i] == Tag.first[0] || Script[Sp][i] == Tag.second[0]) {

				for (std::int32_t i = 0; i < 2; i++) {

					sregex rex = sregex::compile(ScriptTagContainer[i]);

					if (regex_match(Script[Sp], what, rex)) {

						Script[Sp] = regex_replace(Str, rex, "");

						Cp = 0;

						return true;
					}
				}
			}
		}


		return false;
	}

	//�e��V�X�e������
	bool SystemTagTask(Material<std::string>& Script) {

		if (SystemTag(Script, Tag[6]))		//���s
			return ScriptTask::Kaigyou();

		if (SystemTag(Script, Tag[7]))		//�N���b�N�҂�
			return ScriptTask::ClickWait();

		if (SystemTag(Script, Tag[8])) 		//��ʃN���A����
			return ScriptTask::ClearScreen(Script);

		if (SystemTag(Script, Tag[9]))		//�x������
			return ScriptTask::WaitTime();

		if (SystemTag(Script, Tag[10]))		//�Q�[���I�[�o�[��ʕ`��
			return ScriptTask::GameOverScreenDraw();

		if (SystemTag(Script, Tag[11]))		//�G���f�B���O��ʕ`��
			return ScriptTask::DrawEndingMovie();

		if (SystemTag(Script, Tag[12]))		//BGM��~
			return ScriptTask::StopSounds(BackGroundMusicHandle);

		if (SystemTag(Script, Tag[13]))		//SE��~
			return ScriptTask::StopSounds(SoundEffectHandle);

		if (SystemTag(Script, Tag[14])) {		//�I����
			ChoiceSelect(EndFlag);
			return true;
		}

		if (SystemTag(Script, Tag[15])) {		//�Q�[���I��
			EndFlag = 99;
			return true;
		}

		return false;
	}
}

//�X�N���v�g�^�O�����֐�
void ScriptTagTaskManager(Material<std::string>& Script, Material<int>& BackGround, Material<int>& Character, Material<int>& BackGroundMusic, Material<int>& SoundEffect, Material<std::string>& Movie, Material<int>& ImageEffect) noexcept {

	switch (Script[Sp][Cp])
	{

	case '/':	//�R�����g
		Cp++;
		ScriptTask::Comment(Script);
		break;

	case '@':	//�����G����ʂ���폜
		ScriptTask::RemoveCharacterGraph();
		Cp++;
		break;

	case '#':	//�L�����N�^�[���`��
	case ' ':
		Cp++;
		break;

	default:	// ���̑��̕���

				//�V�X�e���p�^�O����
		if (ScriptTask::SystemTagTask(Script))
			break;

		//�e��f�ޕ`��
		if (ScriptTask::DrawMaterial(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie, ImageEffect))
			break;


		//������`��
		ScriptTask::DrawScript(Script);

		// ��ʂ���͂ݏo������s����
		if (DrawPointX * MojiSize + MojiSize > 640) ScriptTask::Kaigyou();

		break;
	}
}