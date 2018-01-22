// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

// 文字のサイズ
const std::int32_t moji_size = 24;

int DrawPointX, DrawPointY;	// 文字列描画の位置
int SP, CP;	// 参照する文字列番号と文字列中の文字ポインタ

//スクリプト格納変数
static std::vector<std::string> Script;

//背景画像格納変数
static std::vector<int> BackGround;

//立ち絵格納変数
static std::vector<int> Character;

//BGM格納変数
static std::vector<int> BackGroundMusic;

//スクリプト読込関数
void ScriptRead(std::vector<std::string>& Script, unsigned int EndFlag) {

	const char* ScriptFilePath[] = { "DATA/STR/LINKS.txt" };

	std::fstream file(ScriptFilePath[EndFlag], std::ios_base::in);

	for (std::string line; std::getline(file, line); ) if (!line.empty()) Script.emplace_back(std::move(line));
}

// 改行関数
int Kaigyou(void)
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

	// 終了
	return 0;
}

//背景画像読込関数
void MaterialLoadBackGround() {

	std::string FilePath = "DATA/BACKGROUND/BG";
	std::string FileFormat = ".png";
	std::string FileName = "";

	for (std::int32_t i = 0; i < 99; i++) {

		std::ostringstream Num;

		Num << std::setfill('0') << std::setw(2) << i + 1;

		FileName = (FilePath + Num.str() + FileFormat);

		BackGround.emplace_back(std::move(DxLib::LoadGraph(FileName.c_str())));
	}
}

//立ち絵素材読込関数
void MaterialLoadCharacter() {

	std::string FilePath = "DATA/CHARACTER/CHAR";
	std::string FileFormat = ".png";
	std::string FileName = "";

	for (std::int32_t i = 0; i < 99; i++) {

		std::ostringstream Num;

		Num << std::setfill('0') << std::setw(2) << i + 1;

		FileName = (FilePath + Num.str() + FileFormat);

		Character.emplace_back(std::move(DxLib::LoadGraph(FileName.c_str())));
	}
}

//BGM読込関数
void MaterialLoadBackGroundMusic() {

	std::string FilePath = "DATA/BACKGROUNDMUSIC/BGM";
	std::string FileFormat = ".ogg";
	std::string FileName = "";

	for (std::int32_t i = 0; i < 99; i++) {

		std::ostringstream Num;

		Num << std::setfill('0') << std::setw(2) << i + 1;

		FileName = (FilePath + Num.str() + FileFormat);

		BackGroundMusic.emplace_back(std::move(DxLib::LoadSoundMem(FileName.c_str())));
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int EndFlag;	// 終了フラグ
	char OneMojiBuf[3];	// １文字分一時記憶配列

	
	DxLib::SetGraphMode(640, 480, 16);

	//ウインドウで描画
	DxLib::ChangeWindowMode(TRUE);

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
	SetFontSize(moji_size);

	// 終了フラグを倒す
	EndFlag = 0;

	//スクリプト読込関数
	ScriptRead(Script, EndFlag);

	//背景画像読込関数
	MaterialLoadBackGround();

	MaterialLoadCharacter();

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 文字の描画
		switch (Script[SP][CP])
		{
		case 'B':
			CP++;
			DxLib::DrawGraph(0, 0, BackGround[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) -48) - 1], TRUE);
			CP++;
			break;

		case 'C':
			CP++;
			DxLib::DrawGraph(150, 130, Character[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], TRUE);
			CP++;
			break;

		case 'M':
			CP++;
			DxLib::PlaySoundMem(BackGroundMusic[(static_cast<int>(Script[SP][CP]) - 48) * 10 + (static_cast<int>(Script[SP][CP + 1]) - 48) - 1], DX_PLAYTYPE_LOOP);
			CP++;
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

		case ' ':
			CP++;
			break;

		default:	// その他の文字

					// １文字分抜き出す
			OneMojiBuf[0] = Script[SP][CP];
			OneMojiBuf[1] = Script[SP][CP + 1];
			OneMojiBuf[2] = '\0';

			// １文字描画
			DrawString(DrawPointX * moji_size, DrawPointY * moji_size,
				OneMojiBuf, GetColor(255, 255, 255));

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

		// 終了フラグが１だったら終了する
		if (EndFlag == 1) break;

		//参照文字列の終端まで行っていたら参照文字列を進める
		if (Script[SP][CP] == '\0')
		{
			SP++;
			CP = 0;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
