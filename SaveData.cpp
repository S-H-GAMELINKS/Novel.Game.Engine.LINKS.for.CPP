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

	//各種分岐表示配列
	static constexpr const char* SaveTaskItem[] = { "セーブしますか？", "ロードしますか？", "削除しますか？" };
	static constexpr const char* SaveTaskItemParticle[] = { "に", "を", "を" };

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

			fopen_s(&fp, SaveDataPath, "wb");//バイナリファイルを開く
			if (nullptr == fp) {//エラーが起きたらnullptrを返す
				return 0;
			}

			fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t構造体の中身を出力
			fclose(fp);

			MessageBoxOk("セーブしました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}
		return 0;
	}

	//セーブデータをロード
	int SaveDataLoad(const char* SaveDataPath, const char* Message) {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData;

			FILE *fp;

			fopen_s(&fp, SaveDataPath, "rb");
			if (fp == nullptr) {
				//MessageBoxOk(ErrorMessage);
				return 0;
			}

			fread(&SaveData, sizeof(SaveData), 1, fp);
			fclose(fp);

			EndFlag = SaveData.ENDFLAG;
			SP = SaveData.SP;
			CP = SaveData.CP;
			CharacterHandle = SaveData.CHAR;
			BackGroundHandle = SaveData.BG;
			BackGroundMusicHandle = SaveData.BGM;

			MessageBoxOk("ロードしました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}
		return 0;
	}

	//セーブデータをデリート
	int SaveDataDelete(const char* SaveDataPath, const char* Message) {
		if (IDYES == MessageBoxYesNo(Message)) {
			std::remove(SaveDataPath);

			MessageBoxOk("削除しました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}
		return 0;
	}

	//セーブ/ロード/デリート切り替え関数
	void SaveDataTask(const int& Num, const char* SaveDataPath, const char* SaveDataName, std::int32_t& SaveFlag) {

		std::string Message = SaveDataName;
		Message += SaveTaskItemParticle[Num - 1];
		Message += SaveTaskItem[Num - 1];

		//セーブ
		if (Num == 1)
			SaveFlag = SaveDataSave(SaveDataPath, Message.c_str());

		//ロード
		if (Num == 2)
			SaveFlag = SaveDataLoad(SaveDataPath, Message.c_str());

		//デリート
		if (Num == 3)
			SaveFlag = SaveDataDelete(SaveDataPath, Message.c_str());
	}

	//セーブ/ロード/デリート メニュー選択処理
	void SaveLoadDeleteMenuSelect(std::int32_t& cursor_y, const int& Num, std::int32_t& SaveFlag) {

		if (cursor_y == save_base_pos_y && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA1.bat", "セーブデータ１", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}


		if (cursor_y == save_base_pos_y * 2 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA2.bat", "セーブデータ２", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}


		if (cursor_y == save_base_pos_y * 3 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA3.bat", "セーブデータ３", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}


		if (cursor_y == save_base_pos_y * 4 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {

			if (IDYES == MessageBoxYesNo("戻りますか？")){
				SaveFlag = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
		}
	}
}

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num) {

	//各種分岐表示
	if (IDYES == MessageBoxYesNo(SaveTaskItem[Num - 1])) {

		std::int32_t SaveFlag = 1;

		DxLib::ClearDrawScreen();
		std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));

		//スクリーンショットの読込
		SaveDataSnapLoad();

		std::int32_t save_y = save_base_pos_y;

		while (SaveFlag == 1) {
			SaveLoadDeleteMenuDraw(save_y);
			SaveLoadMenuKeyMove(save_y);
			SaveLoadDeleteMenuSelect(save_y, Num, SaveFlag);
			ScreenClear();
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(wait_key_task_time));
}