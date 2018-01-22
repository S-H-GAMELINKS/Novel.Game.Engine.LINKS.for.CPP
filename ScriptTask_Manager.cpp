//Script Tag Task Maneger Source
#include "DxLib.h"
#include "Variable.h"
#include <vector>
#include <string>

namespace {

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
}

//スクリプトタグ処理関数
void ScriptTagTaskManager(const std::vector<std::string>& Script, std::vector<int>& BackGround, std::vector<int>& Character, std::vector<int>& BackGroundMusic, std::vector<int>& SoundEffect, std::vector<std::string>& Movie) {

	char OneMojiBuf[3];	// １文字分一時記憶配列

	// 文字の描画
	switch (Script[SP][CP])
	{
	case 'B':
		CP++;
		DxLib::DrawGraph(0, 0, BackGround[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], TRUE);
		break;

	case 'C':
		CP++;
		DxLib::DrawGraph(150, 130, Character[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], TRUE);
		break;

	case 'M':
		CP++;
		DxLib::PlaySoundMem(BackGroundMusic[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], DX_PLAYTYPE_LOOP);
		break;

	case 'S':
		CP++;
		DxLib::PlaySoundMem(SoundEffect[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], DX_PLAYTYPE_BACK);
		break;

	case 'V':
		CP++;
		DxLib::PlayMovie(Movie[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48)].c_str(), 1, DX_MOVIEPLAYTYPE_BCANCEL);
		break;

	case 'L':	// 改行文字

				// 改行処理および参照文字位置を一つ進める
		Kaigyou();
		CP++;

		break;

	case 'P':	// ボタン押し待ち文字

				// ボタン押し待ちおよび参照文字位置を一つ進める
		WaitKey();
		CP++;

		break;

	case 'E':	// 終了文字

				// 終了フラグを立てるおよび参照文字位置を一つ進める
		EndFlag = 1;
		CP++;

		break;

	case 'R':	// クリア文字

				// 画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
		ClearDrawScreen();
		DrawPointY = 0;
		DrawPointX = 0;
		CP++;
		break;

	case '/':
		CP++;

		if (Script[SP][CP] == '/') {
			CP = 0;
			SP++;
		}
		break;

	case '@':
		CP++;
		break;

	case '#':
		CP++;
		break;

	case ' ':
		CP++;
		break;

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

		// 画面からはみ出たら改行する
		if (DrawPointX * moji_size + moji_size > 640) Kaigyou();

		break;
	}
}