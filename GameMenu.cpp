// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "SaveData.hpp"
#include "ConfigMenu.hpp"
#include "Skip_Auto.hpp"
#include "BackLog.hpp"
#include "MouseAndKeyState.hpp"
#include <thread>
#include <chrono>
#include <array>
#include <functional>

//終了フラグ
extern int EndFlag;

//行管理変数
extern int Sp, Cp;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SpTemp;

//各種素材ハンドル
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle;

extern ConfigData_t ConfigData;

namespace {
	//ゲームメニュー描画関数
	void GameMenuDraw(const std::int32_t& GameMenuCursorPosY) noexcept {

		static constexpr const char* GameMenuItem[] = {
			"セーブ", "ロード", "セーブデータ削除", "既読スキップ", "スキップ", "オート",
			"オート/スキップ停止", "バックログ参照", "設定", "タイトルに戻る", "ゲームに戻る", "ゲーム終了"
		};

		//各メニュー項目の描画
		for (std::int32_t i = 0; i < 12; i++)
			DxLib::DrawString(SaveDataNamePosX, GameMenuBasePosY * (i + 1), GameMenuItem[i], Color);

		//カーソルの描画
		DxLib::DrawString(SaveDataBasePosX - (CursorMove * 6), GameMenuCursorPosY, "■", Color);
	}

	//ゲームへ戻る
	void GameMenuBackToGamePlay() noexcept {
		EndFlag = EndFlagTemp;
		Sp = SpTemp;
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//タイトルへ戻る
	void GameMenuBackToTitle() noexcept {

		if (IDYES == MessageBoxYesNo("タイトルに戻りますか？")) {
			EndFlag = 0;
			Sp = 0;
			Cp = 0;
			DrawPointX = 0;
			DrawPointY = 0;
			BackGroundHandle = 0;
			CharacterHandle = 0;
			BackGroundMusicHandle = 0;
			SoundEffectHandle = 0;
		}
		WaitTime();
	}

	constexpr std::array<int, 4> SkipAutoTaskFlag = { 3, 1, 2, 0 };

	const std::array<std::function<void()>, 4> Funcs = { BackLogLoop, ConfigMenuLoop, GameMenuBackToTitle, GameMenuBackToGamePlay };

	//ゲームメニュー項目選択処理
	constexpr void GameMenuSelect(const std::int32_t& GameMenuCursorPosY) noexcept {

		for (std::int32_t i = 0; i < 3; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 1) && MouseAndKey::CheckMouseAndKeyEnter())
				SaveDataLoop(i + 1);
		}

		for (std::int32_t i = 0; i < 3; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 4) && MouseAndKey::CheckMouseAndKeyEnter())
				SkipAndAutoTask(SkipAutoTaskFlag[i], EndFlagTemp);
		}

		for (std::int32_t i = 0; i < 4; i++) {
			if (GameMenuCursorPosY == GameMenuBasePosY * (i + 8) && MouseAndKey::CheckMouseAndKeyEnter())
				Funcs[i]();
		}

		if (GameMenuCursorPosY == GameMenuBasePosY * 12 && MouseAndKey::CheckMouseAndKeyEnter()) {
			if (IDYES == MessageBoxYesNo("ゲームを終了しますか？"))
				EndFlag = 99;
		}
	}
}

//ゲームメニューループ
void GameMenuLoop() noexcept {

	std::int32_t GameMenuCursorPosY = GameMenuBasePosY;

	//ネームスペースの省略
	using namespace MouseState;
	using namespace KeyState;

	while (EndFlag == 17) {
		ScreenClear();
		GameMenuDraw(GameMenuCursorPosY);
		GameMenuCursorPosY = (ConfigData.MouseAndKeyFlag == 1) ? MouseMoveTemplate(GameMenuCursorPosY, GameMenuBasePosY, CursorMove, 12, ConfigData.MouseAndKeyFlag)
																: KeyMoveTemplate(GameMenuCursorPosY, GameMenuBasePosY, GameMenuBasePosY * 12, CursorMove, ConfigData.MouseAndKeyFlag);
		GameMenuSelect(GameMenuCursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}