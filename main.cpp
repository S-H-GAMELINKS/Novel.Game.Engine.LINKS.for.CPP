// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include "Material_Manager.h"
#include "ScriptTask_Manager.h"
#include "GameTitleMenu.h"
#include "GameMenu.h"
#include "ConfigMenu.h"
#include "Skip_Auto.h"
#include "SaveData.h"
#include "Utility.h"
#include "Variable.h"
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <chrono>


//DxLib初期化前処理
void DxLibInitPreProccessing() noexcept {
	//ログの抽出
	DxLib::SetOutApplicationLogValidFlag(FALSE);

	//アイコンの指定
	DxLib::SetWindowIconID(101);

	//ウインドウのサイズを任意に変更可能
	DxLib::SetWindowSizeChangeEnableFlag(TRUE, TRUE);

	//ウインドウサイズ指定
	DxLib::SetGraphMode(640, 480, 16);

	//ウインドウで描画
	DxLib::ChangeWindowMode(TRUE);

	//ゲームタイトルの指定
	DxLib::SetMainWindowText(GameTitle);

	//ウインドウスタイルの指定
	DxLib::SetWindowStyleMode(7);
}

//DxLib初期化後処理
void DxLibInitPostProccessing() noexcept {
	// フォントのサイズセット
	DxLib::SetFontSize(MojiSize);

	//フォントの設定
	DxLib::ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE_8X8);

	//コンフィグ読込関数
	ConfigLoad();

	//背景画像読込関数
	BackGround = MaterialLoad(BackGround, "DATA/BACKGROUND", [](const std::string& Path) {return DxLib::LoadGraph(Path.c_str()); });

	//立ち絵画像読込関数
	Character = MaterialLoad(Character, "DATA/CHARACTER", [](const std::string& Path) {return DxLib::LoadGraph(Path.c_str()); });

	//BGM読込関数
	BackGroundMusic = MaterialLoad(BackGroundMusic, "DATA/BACKGROUNDMUSIC", [](const std::string& Path) {return DxLib::LoadSoundMem(Path.c_str()); });

	//SE読込関数
	SoundEffect = MaterialLoad(SoundEffect, "DATA/SOUNDEFFECT", [](const std::string& Path) {return DxLib::LoadSoundMem(Path.c_str()); });

	//動画読込関数
	Movie = MaterialLoad(Movie, "DATA/MOVIE", [](const std::string& Path) {return std::move(Path); });

	//イメージエフェクト読込関数
	ImageEffect = MaterialLoad(ImageEffect, "DATA/IMAGEEFFECT", [](const std::string& Path) {return DxLib::LoadGraph(Path.c_str()); });

	//タイトル画面読込
	GameTitleGraph = DxLib::LoadGraph("DATA/BACKGROUND/TITLE.png");

	//タイトルムービー処理
	TitleMoviePlay();

	//既読スキップデータの読込
	SkipDataLoad();
}

//ゲーム中のループ
void GamePlayLoop(const int RouteNumber) noexcept {

	Script.clear();

	Script = ScriptRead(EndFlag);

	if (SkipAndAutoFlag == 1)
		SkipDataCheck(RouteNumber);

	while (DxLib::ProcessMessage() == 0) {
		
		//スクリプトタグ処理管理関数
		ScriptTagTaskManager(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie, ImageEffect);

		//ショートカットキー
		ShortCutKey();

		// 終了フラグが異なっていたら終了する
		if (EndFlag != RouteNumber) {
			ContinueSaveDataSave(RouteNumber);
			SkipDataWrite(RouteNumber);
			Sp = 0;
			Cp = 0;
			break;
		}

		//参照文字列の終端まで行っていたら参照文字列を進める
		if (0 < Cp && (Script[Sp].size() == std::size_t(Cp)))
		{
			Sp++;
			Cp = 0;
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//初期化前処理
	DxLibInitPreProccessing();

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//初期化後処理
	DxLibInitPostProccessing();

	std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime * WaitGameTime));

	//カーソルの位置
	std::int32_t CursorPosY = 300;

	while (EndFlag != 99 && DxLib::ProcessMessage() == 0) {

		//タイトル画面
		GameTitleMenuLoop(CursorPosY);

		//ゲームループ
		if (1 <= EndFlag && EndFlag <= 16)
			GamePlayLoop(EndFlag);

		//ゲームメニューループ
		if (EndFlag == 17)
			GameMenuLoop();
	}

	ConfigSave();	// 設定データの保存
	SkipDataSave(); // 既読スキップデータの保存

	std::remove("DATA/SAVE/SAVESNSAPTEMP.png");
	std::remove("DATA/BACKLOG/BACKLOG1.png");

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
