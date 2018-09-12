// Config Menu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "ConfigMenu.hpp"
#include "MouseAndKeyState.hpp"
#include <array>
#include <thread>
#include <chrono>

ConfigData_t ConfigData = {
	/*BackGroundMusicVolume               :*/100,
	/*BackGroundMusicVolumeCount          :*/10,
	/*SoundEffectVolume		              :*/100,
	/*SoundEffectVolumecount              :*/10,
	/*SkipSpeedVolume		              :*/100,
	/*SkipSpeedVolumeCount			      :*/10,
	/*AutoSpeedVolume		              :*/100,
	/*AutoSpeedVolumeCount				  :*/10,
	/*ScriptDrawSpeedVolume		          :*/100,
	/*ScriptDrawSpeedVolumeCount	      :*/10,
	/*MouseAndKeyFlag;			          :*/1
};

namespace {

	static constexpr const char* ConfigMenuItem[] = {
		"�a�f�l����", "�r�d����", "�I�[�g���x", "�X�L�b�v���x", "�����`�摬�x", "�}�E�X/�L�[����", "�߂�"
	};

	//�R���t�B�O��ʕ`��
	void ConfigMenuDraw(std::int32_t& ConfigCursorPosY) noexcept {

		std::array<std::int32_t, 5> Container = { ConfigData.BackGroundMusicVolume, ConfigData.SoundEffectVolume, ConfigData.AutoSpeedVolume,
															ConfigData.SkipSpeedVolume,  ConfigData.ScriptDrawSpeedVolume };

		//�e���ڂ̕`��
		for (std::int32_t i = 0; i < 7; i++)
			DxLib::DrawString(SaveDataNamePosX, GameMenuBasePosY * (i + 1), ConfigMenuItem[i], Color);

		DxLib::DrawString(SaveDataNamePosX - CursorMove, ConfigCursorPosY, "��", Color);

		//�e���ڂ̐��l�`��(�}�E�X����&�L�[���쏜��)
		for (std::int32_t i = 0; i < 5; i++)
			DxLib::DrawFormatString(SaveDataNamePosX + CursorMove * 5, GameMenuBasePosY * (i + 1), Color, "%d", Container[i]);

		DxLib::DrawString(SaveDataNamePosX + CursorMove * 8, GameMenuBasePosY * 6, ((1 == ConfigData.MouseAndKeyFlag) ? "�}�E�X����" : "�L�[����"), Color);
	}

	//�e��p�����[�^���ߋ@�\
	void VolChangeTemplate(std::int32_t& Count, std::int32_t& Param) {

		if (MouseAndKey::CheckMouseAndKeyRight()) {
			Count = (Count >= 10) ? Count = 10 : Count += 1;
			Param = (Count >= 10) ? Param = 100 : Param += 10;

		}

		if (MouseAndKey::CheckMouseAndKeyLeft()) {
			Count = (Count <= 0) ? Count = 0 : Count -= 1;
			Param = (Count <= 0) ? Param = 0 : Param -= 10;
		}

		WaitTime();
	}

	// �}�E�X����/�L�[����؂�ւ�
	void MouseAndKeyMoveChange() noexcept {
		if (MouseAndKey::CheckMouseAndKeyRight())
			ConfigData.MouseAndKeyFlag = 1;

		if (MouseAndKey::CheckMouseAndKeyLeft())
			ConfigData.MouseAndKeyFlag = 0;

		WaitTime();
	}

	//�R���t�B�O��ʑI������
	bool ConfigMenuSelect(std::int32_t& ConfigCursorPosY, bool ConfigFlag) noexcept {

		if (GameMenuBasePosY == ConfigCursorPosY)
			VolChangeTemplate(ConfigData.BackGroundMusicVolumeCount, ConfigData.BackGroundMusicVolume);

		if (GameMenuBasePosY * 2 == ConfigCursorPosY)
			VolChangeTemplate(ConfigData.SoundEffectVolumeCount, ConfigData.SoundEffectVolume);

		if (GameMenuBasePosY * 3 == ConfigCursorPosY)
			VolChangeTemplate(ConfigData.AutoSpeedVolumeCount, ConfigData.AutoSpeedVolume);

		if (GameMenuBasePosY * 4 == ConfigCursorPosY)
			VolChangeTemplate(ConfigData.SkipSpeedVolumeCount, ConfigData.SkipSpeedVolume);

		if (GameMenuBasePosY * 5 == ConfigCursorPosY)
			VolChangeTemplate(ConfigData.ScriptDrawSpeedVolumeCount, ConfigData.ScriptDrawSpeedVolume);

		if (GameMenuBasePosY * 6 == ConfigCursorPosY)
			MouseAndKeyMoveChange();

		if (GameMenuBasePosY * 7 == ConfigCursorPosY && MouseAndKey::CheckMouseAndKeyLeft()) {
			if (IDYES == MessageBoxYesNo("�߂�܂����H")) {
				ConfigFlag = false;
			}
			WaitTime();
		}

		return ConfigFlag;
	}
}

//�R���t�B�O�Ǎ��֐�
int ConfigLoad() noexcept {

	FILE *Fp;

	fopen_s(&Fp, "DATA/SAVE/Config.dat", "rb");
	if (nullptr == Fp) {
		return 0;
	}

	fread(&ConfigData, sizeof(ConfigData_t), 1, Fp);
	fclose(Fp);
	return 0;
}

//�R���t�B�O�ۑ��֐�
int ConfigSave() noexcept {
	FILE* Fp;

	fopen_s(&Fp, "DATA/SAVE/Config.dat", "wb");//�o�C�i���t�@�C�����J��

	if (nullptr == Fp) {//�G���[���N������nullptr��Ԃ�
		return 0;
	}

	fwrite(&ConfigData, sizeof(ConfigData_t), 1, Fp); // ConfigData_t�\���̂̒��g���o��
	fclose(Fp);

	return 0;
}

//�R���t�B�O��ʃ��[�v
void ConfigMenuLoop() noexcept {

	if (IDYES == MessageBoxYesNo("�R���t�B�O��ʂɈڍs���܂����H")) {

		bool ConfigFlag = true;

		std::int32_t ConfigCursorPosY = GameMenuBasePosY;

		//�l�[���X�y�[�X�̏ȗ�
		using namespace MouseState;
		using namespace KeyState;

		while (ConfigFlag) {
			ScreenClear();
			ConfigMenuDraw(ConfigCursorPosY);
			ConfigCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseMoveTemplate(ConfigCursorPosY, GameMenuBasePosY, CursorMove, 7, ConfigData.MouseAndKeyFlag)
																: KeyMoveTemplate(ConfigCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 7, CursorMove, ConfigData.MouseAndKeyFlag);
			ConfigFlag = ConfigMenuSelect(ConfigCursorPosY, ConfigFlag);
		}
	}
}
