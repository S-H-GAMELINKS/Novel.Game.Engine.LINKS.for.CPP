// GameMenu Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "SaveData.h"
#include "ConfigMenu.h"
#include "Skip_Auto.h"
#include "BackLog.h"
#include <thread>
#include <chrono>

//終了フラグ
extern int EndFlag;

//行管理変数
extern int Sp, Cp;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//tempデータ
extern int EndFlagTemp, SpTemp;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

namespace {
	//ゲームメニュー描画関数
	void GameMenuDraw(std::int32_t& GameMenuCursorPosY, unsigned int Color) noexcept {

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

	//ゲームメニューキー操作
	void GameMenuKeyMove(std::int32_t& GameMenuCursorPosY) noexcept {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			GameMenuCursorPosY = (GameMenuBasePosY * 12 == GameMenuCursorPosY) ? GameMenuBasePosY : GameMenuCursorPosY + GameMenuBasePosY;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			GameMenuCursorPosY = (GameMenuBasePosY == GameMenuCursorPosY) ? GameMenuBasePosY * 12 : GameMenuCursorPosY - GameMenuBasePosY;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
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
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//ゲームメニュー項目選択処理
	void GameMenuSelect(std::int32_t& GameMenuCursorPosY) noexcept {

		if (GameMenuCursorPosY == GameMenuBasePosY && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(1);

		if (GameMenuCursorPosY == GameMenuBasePosY * 2 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(2);

		if (GameMenuCursorPosY == GameMenuBasePosY * 3 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SaveDataLoop(3);

		if (GameMenuCursorPosY == GameMenuBasePosY && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SkipAndAutoTask(3);

		if (GameMenuCursorPosY == GameMenuBasePosY * 5 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SkipAndAutoTask(1);

		if (GameMenuCursorPosY == GameMenuBasePosY * 6 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SkipAndAutoTask(2);

		if (GameMenuCursorPosY == GameMenuBasePosY * 7 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			SkipAndAutoTask(0);

		if (GameMenuCursorPosY == GameMenuBasePosY * 8 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			BackLogLoop();

		if (GameMenuCursorPosY == GameMenuBasePosY * 9 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			ConfigMenuLoop();

		if (GameMenuCursorPosY == GameMenuBasePosY * 10 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			GameMenuBackToTitle();

		if (GameMenuCursorPosY == GameMenuBasePosY * 11 && CheckHitKey(KEY_INPUT_RETURN) == 1)
			GameMenuBackToGamePlay();

		if (GameMenuCursorPosY == GameMenuBasePosY * 12 && CheckHitKey(KEY_INPUT_RETURN) == 1) {
			if (IDYES == MessageBoxYesNo("ゲームを終了しますか？"))
				EndFlag = 99;
		}

	}
}

//ゲームメニューループ
void GameMenuLoop() noexcept {

	std::int32_t GameMenuCursorPosY = GameMenuBasePosY;
	unsigned int Color = DxLib::GetColor(255, 255, 255);

	DxLib::ClearDrawScreen();

	while (EndFlag == 17) {
		ScreenClear();
		GameMenuDraw(GameMenuCursorPosY, Color);
		GameMenuKeyMove(GameMenuCursorPosY);
		GameMenuSelect(GameMenuCursorPosY);

		//ゲーム終了確認ウインドウ
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
			GameEndMessageBox();
	}
}