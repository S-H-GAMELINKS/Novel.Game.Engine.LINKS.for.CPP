// SaveData Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <string>
#include <thread>
#include <chrono>

extern int SP, CP;	// 参照する文字列番号と文字列中の文字ポインタ

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;

//終了フラグ
extern int EndFlag;

namespace {

	//セーブデータ用スクリーンショット格納変数
	std::int32_t SaveSnap[save_max_num];

	//通常セーブデータ
	struct alignas(4) SaveData_t {
		std::int32_t ENDFLAG;    //ENDFLAG
		std::int32_t SP;			//行数
		std::int32_t CP;			//文字位置
		std::int32_t CHAR;		//立ち絵情報
		std::int32_t BG;			//背景画像情報
		std::int32_t BGM;		//BGM情報
	};

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

	//セーブ/ロード/デリート メニュー描画
	void SaveLoadDeleteMenuDraw(std::int32_t& cursor_y) {

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

	//セーブデータをセーブ
	int SaveDataSave(const char* SaveDataPath, const char* Message) {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData = { EndFlag, SP, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

			FILE *fp;

#ifdef LINKS_HAS_FOPEN_S
			const errno_t er = fopen_s(&fp, SaveDataPath, "wb");
			if (0 != er || nullptr == fp) {
				return 0;
			}
#else
			fopen_s(&fp, SaveDataPath, "wb");//バイナリファイルを開く
			if (nullptr == fp) {//エラーが起きたらnullptrを返す
				return 0;
			}
#endif

			fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t構造体の中身を出力
			fclose(fp);
		}
	}

	//セーブデータをロード
	int SaveDataLoad(const char* SaveDataPath, const char* Message) {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData;

			FILE *fp;

#ifdef LINKS_HAS_FOPEN_S
			const errno_t er = fopen_s(&fp, SaveDataPath, "rb");
			if (0 != er || nullptr == fp) {
				MessageBoxOk(ErrorMessage);
				return 0;
			}
#else
			fopen_s(&fp, SaveDataPath, "rb");
			if (fp == nullptr) {
				//MessageBoxOk(ErrorMessage);
				return 0;
			}
#endif
			fread(&SaveData, sizeof(SaveData), 1, fp);
			fclose(fp);
			EndFlag = SaveData.ENDFLAG;
			SP = SaveData.SP;
			CP = SaveData.CP;
			CharacterHandle = SaveData.CHAR;
			BackGroundHandle = SaveData.BG;
			BackGroundMusicHandle = SaveData.BGM;
		}
	}

	//セーブデータをデリート
	void SaveDataDelete(const char* SaveDataPath, const char* Message) {
		if (IDYES == MessageBoxYesNo(Message)) {
			std::remove(SaveDataPath);
		}
	}

	//セーブ/ロード/デリート切り替え関数
	void SaveDataTask(std::int32_t Num, const char* SaveDataPath) {

		static constexpr const char* SaveTaskItem[] = { "セーブしますか？", "ロードしますか？", "削除しますか？" };

		//セーブ
		if (Num == 1)
			SaveDataSave(SaveDataPath, SaveTaskItem[Num - 1]);

		//ロード
		if (Num == 2)
			SaveDataLoad(SaveDataPath, SaveTaskItem[Num - 1]);

		//デリート
		if (Num == 3)
			SaveDataDelete(SaveDataPath, SaveTaskItem[Num - 1]);
	}
}

//セーブ/ロード/デリート メニュー選択処理
void SaveLoadDeleteMenuSelect(std::int32_t& cursor_y) {

	if (cursor_y == save_base_pos_y && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		SaveDataTask(1, "DATA/SAVE/SAVEDATA1.bat");
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 2 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		SaveDataTask(1, "DATA/SAVE/SAVEDATA2.bat");
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 3 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		SaveDataTask(1, "DATA/SAVE/SAVEDATA3.bat");
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}


	if (cursor_y == save_base_pos_y * 4 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
	}
}

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop() {

	//スクリーンショットの読込
	SaveDataSnapLoad();

	std::int32_t save_y = save_base_pos_y;

	while (EndFlag == 17) {
		SaveLoadDeleteMenuDraw(save_y);
		SaveLoadMenuKeyMove(save_y);
		SaveLoadDeleteMenuSelect(save_y);
		ScreenClear();
	}
}