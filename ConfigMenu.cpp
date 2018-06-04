// Config Menu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "ConfigMenu.h"
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
		"ＢＧＭ音量", "ＳＥ音量", "オート速度", "スキップ速度", "文字描画速度", "マウス/キー操作", "戻る"
	};

	//コンフィグ画面描画
	void ConfigMenuDraw(std::int32_t& ConfigCursorPosY) noexcept {

		std::array<std::int32_t, 5> Container = { ConfigData.BackGroundMusicVolume, ConfigData.SoundEffectVolume, ConfigData.AutoSpeedVolume,
															ConfigData.SkipSpeedVolume,  ConfigData.ScriptDrawSpeedVolume };

		//各項目の描画
		for (std::int32_t i = 0; i < 7; i++)
			DxLib::DrawString(SaveDataNamePosX, GameMenuBasePosY * (i + 1), ConfigMenuItem[i], Color);

		DxLib::DrawString(SaveDataNamePosX - CursorMove, ConfigCursorPosY, "■", Color);

		//各項目の数値描画(マウス操作&キー操作除く)
		for (std::int32_t i = 0; i < 5; i++)
			DxLib::DrawFormatString(SaveDataNamePosX + CursorMove * 5, GameMenuBasePosY * (i + 1), Color, "%d", Container[i]);

		DxLib::DrawString(SaveDataNamePosX + CursorMove * 8, GameMenuBasePosY * 6, ((1 == ConfigData.MouseAndKeyFlag) ? "マウス操作" : "キー操作"), Color);
	}

	//各種パラメータ調節機能
	void VolChangeTemplate(std::int32_t& Count, std::int32_t& Param) {

		if (MouseAndKey::CheckMouseAndKeyRight()) {
			Count = (Count >= 10) ? Count = 10 : Count += 1;
			Param = (Count >= 10) ? Param = 100 : Param += 10;

		}

		if (MouseAndKey::CheckMouseAndKeyLeft()) {
			Count = (Count <= 0) ? Count = 0 : Count -= 1;
			Param = (Count <= 0) ? Param = 0 : Param -= 10;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	// マウス操作/キー操作切り替え
	void MouseAndKeyMoveChange() noexcept {
		if (MouseAndKey::CheckMouseAndKeyRight())
			ConfigData.MouseAndKeyFlag = 1;

		if (MouseAndKey::CheckMouseAndKeyLeft())
			ConfigData.MouseAndKeyFlag = 0;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//コンフィグ画面選択処理
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
			if (IDYES == MessageBoxYesNo("戻りますか？")) {
				ConfigFlag = false;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}

		return ConfigFlag;
	}
}

//コンフィグ読込関数
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

//コンフィグ保存関数
int ConfigSave() noexcept {
	FILE* Fp;

	fopen_s(&Fp, "DATA/SAVE/Config.dat", "wb");//バイナリファイルを開く

	if (nullptr == Fp) {//エラーが起きたらnullptrを返す
		return 0;
	}

	fwrite(&ConfigData, sizeof(ConfigData_t), 1, Fp); // ConfigData_t構造体の中身を出力
	fclose(Fp);

	return 0;
}

//コンフィグ画面ループ
void ConfigMenuLoop() noexcept {

	if (IDYES == MessageBoxYesNo("コンフィグ画面に移行しますか？")) {

		bool ConfigFlag = true;

		std::int32_t ConfigCursorPosY = GameMenuBasePosY;

		while (ConfigFlag) {
			ScreenClear();
			ConfigMenuDraw(ConfigCursorPosY);
			ConfigCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseState::MouseMoveTemplate(ConfigCursorPosY, GameMenuBasePosY, CursorMove, 7, ConfigData.MouseAndKeyFlag)
																: KeyState::KeyMoveTemplate(ConfigCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 7, CursorMove, ConfigData.MouseAndKeyFlag);
			ConfigFlag = ConfigMenuSelect(ConfigCursorPosY, ConfigFlag);
		}
	}
}
