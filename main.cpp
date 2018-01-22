// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include "Material_Manager.h"
#include "ScriptTask_Manager.h"
#include <vector>
#include <string>
#include <fstream>

// 文字のサイズ
std::int32_t moji_size = 24;

int DrawPointX = 0, DrawPointY = 0;	// 文字列描画の位置
int SP = 0, CP = 0;	// 参照する文字列番号と文字列中の文字ポインタ

int EndFlag = 0;	// 終了フラグ

//スクリプト格納変数
static std::vector<std::string> Script;

//背景画像格納変数
static std::vector<int> BackGround;

//立ち絵格納変数
static std::vector<int> Character;

//BGM格納変数
static std::vector<int> BackGroundMusic;

static std::vector<int> SoundEffect;

//動画格納変数
static std::vector<std::string> Movie;

//スクリプト読込関数
void ScriptRead(std::vector<std::string>& Script, unsigned int EndFlag) {

	const char* ScriptFilePath[] = {
		"DATA/STR/LINKS.txt",
		"DATA/STR/A.txt",
		"DATA/STR/B.txt",
		"DATA/STR/C.txt",
		"DATA/STR/D.txt",
		"DATA/STR/E.txt",
		"DATA/STR/F.txt",
		"DATA/STR/G.txt",
		"DATA/STR/H.txt",
		"DATA/STR/I.txt",
		"DATA/STR/J.txt",
		"DATA/STR/K.txt",
		"DATA/STR/L.txt",
		"DATA/STR/M.txt",
		"DATA/STR/N.txt"
	};

	std::fstream file(ScriptFilePath[EndFlag], std::ios_base::in);

	for (std::string line; std::getline(file, line); ) if (!line.empty()) Script.emplace_back(std::move(line));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	DxLib::SetGraphMode(640, 480, 16);

	//ウインドウで描画
	DxLib::ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// フォントのサイズセット
	SetFontSize(moji_size);

	//スクリプト読込関数
	ScriptRead(Script, EndFlag);

	MaterialLoad(BackGround, Character, BackGroundMusic, SoundEffect, Movie);

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		//スクリプトタグ処理管理関数
		ScriptTagTaskManager(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie);

		// 終了フラグが１だったら終了する
		if (EndFlag == 1) break;

		//参照文字列の終端まで行っていたら参照文字列を進める
		if (0 < CP && (Script[SP].size() == std::size_t(CP)))
		{
			SP++;
			CP = 0;
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
