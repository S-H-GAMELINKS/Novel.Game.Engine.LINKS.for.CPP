// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include <vector>
#include <string>
#include <fstream>

// 文字のサイズ
#define MOJI_SIZE 24

int DrawPointX, DrawPointY;	// 文字列描画の位置
int SP, CP;	// 参照する文字列番号と文字列中の文字ポインタ

char String[][256] =
{
	"　ゲームプログラムとは、いやプログラムとは" ,
	"ある事柄を実現するプログラムの方法を説明されても理解できないことがある。B" ,
	"@　なぜならそのプログラム技法も何かの基本的な技法の組み合わせで出来ているからだ。B",
	"@　これはその他の学問も基本がわからないと応用が利かないということと同じ現象で、",
	"別に特に珍しいことでもない。B" ,
	"C　しかしゲームプログラムとなると覚えなくてはならない基礎が沢山あり、" ,
	"さらにある程度クオリティの高いソフトを作ろうとすると色々なプログラム技法を",
	"習得しなくてはならない。B" ,
	"@　しかもある程度レベルが高くなると自分で技法を編み出すか、技術レベルの高い",
	"プログラマーに聞くなどするしか方法がなく大変厄介である。B"
	"というかそのせいでゲームプログラムの敷居は高くなっているといえる。BE"
};

// 改行関数
int Kaigyou(void)
{
	int TempGraph;

	// 描画行位置を一つ下げる
	DrawPointY++;

	// 描画列を最初に戻す
	DrawPointX = 0;

	// もし画面からはみ出るなら画面をスクロールさせる
	if (DrawPointY * MOJI_SIZE + MOJI_SIZE > 480)
	{
		// テンポラリグラフィックの作成
		TempGraph = MakeGraph(640, 480);

		// 画面の内容を丸々コピーする
		GetDrawScreenGraph(0, 0, 640, 480, TempGraph);

		// 一行分上に貼り付ける
		DrawGraph(0, -MOJI_SIZE, TempGraph, FALSE);

		// 一番下の行の部分を黒で埋める
		DrawBox(0, 480 - MOJI_SIZE, 640, 480, 0, TRUE);

		// 描画行位置を一つあげる
		DrawPointY--;

		// グラフィックを削除する
		DeleteGraph(TempGraph);
	}

	// 終了
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int EndFlag;	// 終了フラグ
	char OneMojiBuf[3];	// １文字分一時記憶配列

	SetGraphMode(640, 480, 16);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画位置の初期位置セット
	DrawPointX = 0;
	DrawPointY = 0;

	// 参照文字位置をセット
	SP = 0;	// １行目の
	CP = 0;	// ０文字

			// フォントのサイズセット
	SetFontSize(MOJI_SIZE);

	// 終了フラグを倒す
	EndFlag = 0;

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		char  Moji;

		// 文字の描画
		Moji = String[SP][CP];
		switch (Moji)
		{
		case '@':	// 改行文字

					// 改行処理および参照文字位置を一つ進める
			Kaigyou();
			CP++;

			break;

		case 'B':	// ボタン押し待ち文字

					// ボタン押し待ちおよび参照文字位置を一つ進める
			WaitKey();
			CP++;

			break;

		case 'E':	// 終了文字

					// 終了フラグを立てるおよび参照文字位置を一つ進める
			EndFlag = 1;
			CP++;

			break;

		case 'C':	// クリア文字

					// 画面を初期化して描画文字位置を初期位置に戻すおよび参照文字位置を一つ進める
			ClearDrawScreen();
			DrawPointY = 0;
			DrawPointX = 0;
			CP++;

			break;

		default:	// その他の文字

					// １文字分抜き出す
			OneMojiBuf[0] = String[SP][CP];
			OneMojiBuf[1] = String[SP][CP + 1];
			OneMojiBuf[2] = '\0';

			// １文字描画
			DrawString(DrawPointX * MOJI_SIZE, DrawPointY * MOJI_SIZE,
				OneMojiBuf, GetColor(255, 255, 255));

			// 参照文字位置を２バイト勧める
			CP += 2;

			// カーソルを一文字文進める
			DrawPointX++;

			// 少し待つ
			WaitTimer(10);

			// 画面からはみ出たら改行する
			if (DrawPointX * MOJI_SIZE + MOJI_SIZE > 640) Kaigyou();

			break;
		}

		// 終了フラグが１だったら終了する
		if (EndFlag == 1) break;

		// 参照文字列の終端まで行っていたら参照文字列を進める
		if (String[SP][CP] == '\0')
		{
			SP++;
			CP = 0;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
