//Script Tag Task Maneger Source
#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "Choice.h"
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <thread>
#include <chrono>

extern int DrawPointX, DrawPointY;	// 文字列描画の位置
extern int SP, CP;	// 参照する文字列番号と文字列中の文字ポインタ

//終了フラグ
extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

namespace ScriptTask {

	char OneMojiBuf[3];	// １文字分一時記憶配列

	// 改行関数
	void Kaigyou()
	{
		int TempGraph;

		// 描画行位置を一つ下げる
		DrawPointY++;

		// 描画列を最初に戻す
		DrawPointX = 0;

		// もし画面からはみ出るなら画面をスクロールさせる
		if (DrawPointY * moji_size + moji_size > 480)
		{
			// テンポラリグラフィックの作成
			TempGraph = MakeGraph(640, 480);

			// 画面の内容を丸々コピーする
			GetDrawScreenGraph(0, 0, 640, 480, TempGraph);

			// 一行分上に貼り付ける
			DrawGraph(0, -moji_size, TempGraph, FALSE);

			// 一番下の行の部分を黒で埋める
			DrawBox(0, 480 - moji_size, 640, 480, 0, TRUE);

			// 描画行位置を一つあげる
			DrawPointY--;

			// グラフィックを削除する
			DeleteGraph(TempGraph);
		}
	}

	//文字列描画関数
	void DrawScript(const std::vector<std::string>& Script) {
		// １文字分抜き出す
		OneMojiBuf[0] = Script[SP][CP];
		OneMojiBuf[1] = Script[SP][CP + 1];
		OneMojiBuf[2] = '\0';

		// １文字描画
		DrawString(DrawPointX * moji_size, DrawPointY * moji_size, OneMojiBuf, GetColor(255, 255, 255));

		// 参照文字位置を２バイト勧める
		CP += 2;

		// カーソルを一文字文進める
		DrawPointX++;

		// 少し待つ
		WaitTimer(10);
	}

	//背景画像描画関数
	void DrawBackGround(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGround) {
		CP++;
		BackGroundHandle = BackGround[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1];
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
	}

	//立ち絵描画関数
	void DrawCharacter(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& Character) {
		CP++;
		CharacterHandle = Character[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1];
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//BGM再生関数
	void PlayBackGroundMusic(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGroundMusic) {
		
		//BGM再生中の場合は、BGMを停止する
		if (DxLib::CheckSoundMem(BackGroundMusicHandle))
			DxLib::StopSoundMem(BackGroundMusicHandle);

		CP++;
		BackGroundMusicHandle = BackGroundMusic[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1];
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
	}

	//効果音再生関数
	void PlaySoundEffect(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& SoundEffect) {

		//SE再生中の場合は、SEを停止する
		if (DxLib::CheckSoundMem(SoundEffectHandle))
			DxLib::StopSoundMem(SoundEffectHandle);

		CP++;
		SoundEffectHandle = SoundEffect[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1];
		DxLib::PlaySoundMem(SoundEffectHandle, DX_PLAYTYPE_BACK);
	}

	//動画再生関数
	template <typename T>
	void PlayMovie(const std::vector<T>& Script, std::array<T, MaterialMax> Movie) {
		CP++;
		DxLib::PlayMovie(Movie[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48)].c_str(), 1, DX_MOVIEPLAYTYPE_BCANCEL);
	}

	//画面クリア処理関数
	void ClearScreen() {
		ClearDrawScreen();
		DrawPointY = 0;
		DrawPointX = 0;
	}

	//コメント処理関数
	template <typename T>
	void Comment(const std::vector<T>& Script) {
		if (Script[SP][CP] == '/') {
			CP = 0;
			SP++;
		}
	}
}

//スクリプトタグ処理関数
void ScriptTagTaskManager(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGround, const std::array<int, MaterialMax>& Character, const std::array<int, MaterialMax>& BackGroundMusic, const std::array<int, MaterialMax>& SoundEffect, const std::array<std::string, MaterialMax>& Movie) {

	switch (Script[SP][CP])
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

	case 'L':	//改行文字
		ScriptTask::Kaigyou();
		CP++;
		break;

	case 'P':	//クリック待ち
		DxLib::WaitKey();
		CP++;
		break;

	case 'R':	//画面クリア
		ScriptTask::ClearScreen();
		CP++;
		break;

	case 'W': //遅延処理
		std::this_thread::sleep_for(std::chrono::seconds(wait_game_time));
		CP++;
		break;

	case 'G':	//ゲームオーバー画面
		CP++;
		break;

	case 'F':	//エンディング画面
		CP++;
		break;

	case 'O':	//BGM停止
		DxLib::StopSoundMem(BackGroundMusicHandle);
		CP++;
		break;

	case 'Q':	//SE停止
		CP++;
		DxLib::StopSoundMem(SoundEffectHandle);
		break;

	case 'D':	//選択肢
		ChoiceSelect(EndFlag);
		break;

	case 'E':	//ゲーム終了
		EndFlag = 99;
		CP++;
		break;

	case '/':	//コメント
		CP++;
		ScriptTask::Comment(Script);
		break;

	case '@':	//立ち絵を画面から削除
		CP++;
		break;

	case '#':	//キャラクター名描画
		CP++;
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
		CP++;
		break;

	default:	// その他の文字

		//文字列描画
		ScriptTask::DrawScript(Script);

		// 画面からはみ出たら改行する
		if (DrawPointX * moji_size + moji_size > 640) ScriptTask::Kaigyou();

		break;
	}
}