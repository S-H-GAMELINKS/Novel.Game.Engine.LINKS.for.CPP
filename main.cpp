// Novel Game Engine LINKS for C++
// Coding by S.H.(GAMELINKS)

#include "DxLib.h"
#include "Material_Manager.h"
#include "ScriptTask_Manager.h"
#include "GameTitleMenu.h"
#include "Utility.h"
#include "Variable.h"
#include <vector>
#include <string>
#include <fstream>

#include <thread>
#include <chrono>


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
	MaterialLoad(BackGround, Character, BackGroundMusic, SoundEffect, Movie, GameTitleGraph);
}

//ゲーム中のループ
void GamePlayLoop(const int RouteNumber) {

	ScriptRead(Script, EndFlag);

	while (DxLib::ProcessMessage() == 0) {
		
		//スクリプトタグ処理管理関数
		ScriptTagTaskManager(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie);

		//ゲーム終了確認ウインドウ
		GameEndMessageBox();

		// 終了フラグが異なっていたら終了する
		if (EndFlag != RouteNumber) {
			SP = 0;
			CP = 0;
			break;
		}

		//参照文字列の終端まで行っていたら参照文字列を進める
		if (0 < CP && (Script[SP].size() == std::size_t(CP)))
		{
			SP++;
			CP = 0;
		}
	}
}

std::int32_t SaveSnap[save_max_num];

//セーブデータ用スクリーンショット読込関数
void SaveDataSnapLoad() {
	std::string FilePath = "DATA/SAVE/SAVESNAP";
	std::string FileFormat = ".png";
	std::string FileName = "";

	for (std::int32_t i = 0; i < save_max_num; i++) {

		FileName = (FilePath + std::to_string(i) + FileFormat);

		SaveSnap[i] = DxLib::LoadGraph(FileName.c_str());
	}
}

//セーブ/ロードメニュー描画
void SaveLoadMenuDraw(std::int32_t& cursor_y) {

	//スクリーンショット描画
	for (std::int32_t i = 0; i < save_max_num; i++)
	DxLib::DrawRotaGraph(save_snap_draw_pos_x, save_base_pos_y * (i + 1), 0.2f, 0, SaveSnap[i - 1], TRUE);

	//カーソルの描画
	DxLib::DrawString(save_base_pos_x, cursor_y, "■", 255);

	//セーブデータ名描画
	DxLib::DrawString(save_name_pos_x, save_base_pos_y, "セーブデータ1", 255);
	DxLib::DrawString(save_name_pos_x, save_base_pos_y * 2, "セーブデータ2", 255);
	DxLib::DrawString(save_name_pos_x, save_base_pos_y * 3, "セーブデータ3", 255);

	DxLib::DrawString(save_name_pos_x - cursor_move, save_base_pos_y * 4, "戻る", 255);
}

//セーブ/ロードメニューキー操作
void SaveLoadMenuKeyMove(std::int32_t& cursor_y) {
	if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
		cursor_y = (save_buttom_y == cursor_y) ? save_base_pos_y : cursor_y + save_move_unit;

	if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
		cursor_y = (save_base_pos_y == cursor_y) ? save_buttom_y : cursor_y - save_move_unit;

	std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
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

	//文字などの色
	std::int32_t color = DxLib::GetColor(255, 255, 255);

	//カーソルの位置
	std::int32_t cursor_y = 300;

	while (EndFlag != 99) {

		//タイトル画面
		GameTitleMenuLoop(color, cursor_y);

		//ゲームループ
		if (1 <= EndFlag && EndFlag <= 16)
			GamePlayLoop(EndFlag);
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
