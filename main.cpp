// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include "Material_Manager.h"
#include "ScriptTask_Manager.h"
#include "GameTitleMenu.h"
#include "Variable.h"
#include <vector>
#include <string>
#include <fstream>

//DxLib初期化前処理
void DxLibInitPreProccessing() {
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
void DxLibInitPostProccessing() {
	// フォントのサイズセット
	SetFontSize(moji_size);

	//スクリプト読込関数
	ScriptRead(Script, EndFlag);

	//各種素材の読込関数
	MaterialLoad(BackGround, Character, BackGroundMusic, SoundEffect, Movie);
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

	std::int32_t color = DxLib::GetColor(255, 255, 255);
	std::int32_t cursor_y = 300;

	//タイトル画面
	GameTitleMenuLoop(color, cursor_y);

	ScriptRead(Script, EndFlag);

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
