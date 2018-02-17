//Script Tag Task Maneger Source
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "Choice.h"
#include "ConfigMenu.h"
#include "BackLog.h"
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <thread>
#include <chrono>

extern int DrawPointX, DrawPointY;	// 文字列描画の位置
extern int Sp, Cp;	// 参照する文字列番号と文字列中の文字ポインタ

//終了フラグ
extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;
extern std::int32_t ImageEffectHandle;

// 既読スキップ/スキップ/オート変数
extern int SkipAndAutoFlag;

extern struct ConfigData_t ConfigData;

namespace ScriptTask {

	char OneMojiBuf[3];	// １文字分一時記憶配列

	// 改行関数
	void Kaigyou() noexcept {
		int TempGraph;

		// 描画行位置を一つ下げる
		DrawPointY++;

		// 描画列を最初に戻す
		DrawPointX = 0;

		// もし画面からはみ出るなら画面をスクロールさせる
		if (DrawPointY * MojiSize + MojiSize > 480)
		{
			// テンポラリグラフィックの作成
			TempGraph = MakeGraph(640, 480);

			// 画面の内容を丸々コピーする
			GetDrawScreenGraph(0, 0, 640, 480, TempGraph);

			// 一行分上に貼り付ける
			DrawGraph(0, -MojiSize, TempGraph, FALSE);

			// 一番下の行の部分を黒で埋める
			DrawBox(0, 480 - MojiSize, 640, 480, 0, TRUE);

			// 描画行位置を一つあげる
			DrawPointY--;

			// グラフィックを削除する
			DeleteGraph(TempGraph);
		}
	}

	//文字列描画速度
	void DrawScriptSpeed() noexcept {
		switch (SkipAndAutoFlag) {
		case 0:
			std::this_thread::sleep_for(std::chrono::milliseconds(StringSpeedAuto * ConfigData.ScriptDrawSpeedVolume / 100));
			break;

		case 1:
			std::this_thread::sleep_for(std::chrono::milliseconds(StringSpeedSkip * ConfigData.ScriptDrawSpeedVolume / 100));
			break;

		case 2:
			std::this_thread::sleep_for(std::chrono::milliseconds(StringSpeedAuto * ConfigData.ScriptDrawSpeedVolume / 100));
			break;
		}
	}

	//文字列描画関数
	void DrawScript(const std::vector<std::string>& Script) noexcept {
		// １文字分抜き出す
		OneMojiBuf[0] = Script[Sp][Cp];
		OneMojiBuf[1] = Script[Sp][Cp + 1];
		OneMojiBuf[2] = '\0';

		// １文字描画
		DrawString(DrawPointX * MojiSize, DrawPointY * MojiSize, OneMojiBuf, Color);

		// 参照文字位置を２バイト勧める
		Cp += 2;

		// カーソルを一文字文進める
		DrawPointX++;

		// 少し待つ
		DrawScriptSpeed();
	}

	//背景画像描画関数
	void DrawBackGround(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGround) noexcept {
		Cp++;
		BackGroundHandle = BackGround[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1];
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
	}

	//立ち絵削除処理関数
	void RemoveCharacterGraph() noexcept {
		std::unique_ptr<int> CharacterDummy = std::make_unique<int>(DxLib::DerivationGraph(CharacterPosX, CharacterPosY, CharacterGraphSizeWidth, CharacterGraphSizeHeight, BackGroundHandle));
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, *CharacterDummy, true);
	}

	//立ち絵描画関数
	void DrawCharacter(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& Character) noexcept {
		Cp++;

		ScriptTask::RemoveCharacterGraph();

		CharacterHandle = Character[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1];
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, CharacterHandle, TRUE);
	}

	//BGM再生関数
	void PlayBackGroundMusic(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGroundMusic) noexcept {

		DxLib::ChangeVolumeSoundMem(255 * ConfigData.BackGroundMusicVolume / 100, BackGroundMusicHandle);
		
		//BGM再生中の場合は、BGMを停止する
		if (DxLib::CheckSoundMem(BackGroundMusicHandle))
			DxLib::StopSoundMem(BackGroundMusicHandle);

		Cp++;
		BackGroundMusicHandle = BackGroundMusic[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1];
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
	}

	//効果音再生関数
	void PlaySoundEffect(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& SoundEffect) noexcept {

		DxLib::ChangeVolumeSoundMem(255 * ConfigData.SoundEffectVolume / 100, SoundEffectHandle);

		//SE再生中の場合は、SEを停止する
		if (DxLib::CheckSoundMem(SoundEffectHandle))
			DxLib::StopSoundMem(SoundEffectHandle);

		Cp++;
		SoundEffectHandle = SoundEffect[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1];
		DxLib::PlaySoundMem(SoundEffectHandle, DX_PLAYTYPE_BACK);
	}

	//動画再生関数
	template <typename T>
	void PlayMovie(const std::vector<T>& Script, std::array<T, MaterialMax> Movie) noexcept {
		Cp++;
		DxLib::PlayMovie(Movie[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1].c_str(), 1, DX_MOVIEPLAYTYPE_BCANCEL);
	}

	//イメージエフェクト描画関数
	void DrawImageEffect(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& ImageEffect) {
		Cp++;
		ImageEffectHandle = ImageEffect[(static_cast<int>(Script[Sp][Cp]) - 48) * 10 + (static_cast<int>(Script[Sp][Cp + 1]) - 48) - 1];
		DxLib::DrawGraph(0, 0, ImageEffectHandle, TRUE);
	}

	//画面クリア処理関数
	void ClearScreen() noexcept {
		BackLogGet();
		ClearDrawScreen();
		BackGroundHandle = 0;
		CharacterHandle = 0;
		DrawPointY = 0;
		DrawPointX = 0;
	}

	//コメント処理関数
	template <typename T>
	void Comment(const std::vector<T>& Script) noexcept {
		if (Script[Sp][Cp] == '/') {
			Cp = 0;
			Sp++;
		}
	}

	//クリック待ち処理関数
	void ClickWait() noexcept {
		if (SkipAndAutoFlag == 0) {
			DxLib::WaitKey();
			if ((ConfigData.MouseAndKeyFlag == 0 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) || (ConfigData.MouseAndKeyFlag == 1 && (DxLib::GetMouseInput() == MOUSE_INPUT_LEFT)))
				Cp++;
		}

		if (SkipAndAutoFlag == 1)
			Cp++;

		if (SkipAndAutoFlag == 2) {
			std::this_thread::sleep_for(std::chrono::milliseconds((StringSpeedAuto * StringSpeedWait * ConfigData.AutoSpeedVolume / 100)));
			Cp++;
		}

	}

	//ゲームオーバー画面の描画
	void GameOverScreenDraw() {
		std::unique_ptr<std::int32_t> GameOverHandle = std::make_unique<std::int32_t>(DxLib::LoadGraph("DATA/BACKGROUND/GAMEOVER.png"));
		DxLib::DrawGraph(0, 0, *GameOverHandle, TRUE);
	}
}

//スクリプトタグ処理関数
void ScriptTagTaskManager(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGround, const std::array<int, MaterialMax>& Character, const std::array<int, MaterialMax>& BackGroundMusic, const std::array<int, MaterialMax>& SoundEffect, const std::array<std::string, MaterialMax>& Movie, const std::array<int, MaterialMax>& ImageEffect) noexcept {

	switch (Script[Sp][Cp])
	{
	case 'B':	//背景画像描画
		ScriptTask::DrawBackGround(Script, BackGround);
		break;

	case 'C':	//立ち絵画像描画
		ScriptTask::DrawCharacter(Script, Character);
		break;

	case 'M':	//BGM再生
		ScriptTask::PlayBackGroundMusic(Script, BackGroundMusic);
		break;

	case 'S':	//SE再生
		ScriptTask::PlaySoundEffect(Script, SoundEffect);
		break;

	case 'V':	//動画再生
		ScriptTask::PlayMovie(Script, Movie);
		break;

	case 'I':	//イメージエフェクト描画
		ScriptTask::DrawImageEffect(Script, ImageEffect);
		break;

	case 'L':	//改行文字
		ScriptTask::Kaigyou();
		Cp++;
		break;

	case 'P':	//クリック待ち
		ScriptTask::ClickWait();
		break;

	case 'R':	//画面クリア
		ScriptTask::ClearScreen();
		Cp++;
		break;

	case 'W': //遅延処理
		std::this_thread::sleep_for(std::chrono::seconds(WaitGameTime));
		Cp++;
		break;

	case 'G':	//ゲームオーバー画面
		ScriptTask::GameOverScreenDraw();
		Cp++;
		break;

	case 'F':	//エンディング画面
		DxLib::PlayMovie("DATA/MOVIE/ENDING.wmv", 1, DX_MOVIEPLAYTYPE_NORMAL);
		Cp++;
		break;

	case 'O':	//BGM停止
		DxLib::StopSoundMem(BackGroundMusicHandle);
		Cp++;
		break;

	case 'Q':	//SE停止
		Cp++;
		DxLib::StopSoundMem(SoundEffectHandle);
		break;

	case 'D':	//選択肢
		ChoiceSelect(EndFlag);
		break;

	case 'E':	//ゲーム終了
		EndFlag = 99;
		Cp++;
		break;

	case '/':	//コメント
		Cp++;
		ScriptTask::Comment(Script);
		break;

	case '@':	//立ち絵を画面から削除
		ScriptTask::RemoveCharacterGraph();
		Cp++;
		break;

	case '#':	//キャラクター名描画
		Cp++;
		break;

	case ' ':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		Cp++;
		break;

	default:	// その他の文字

		//文字列描画
		ScriptTask::DrawScript(Script);

		// 画面からはみ出たら改行する
		if (DrawPointX * MojiSize + MojiSize > 640) ScriptTask::Kaigyou();

		break;
	}
}