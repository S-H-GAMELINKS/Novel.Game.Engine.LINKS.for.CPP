// SaveData Source

#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include <string>
#include <thread>
#include <chrono>

extern int Sp, Cp;	// 参照する文字列番号と文字列中の文字ポインタ

//各種素材ハンドル
extern std::int32_t BackGroundHandle;
extern std::int32_t CharacterHandle;
extern std::int32_t BackGroundMusicHandle;
extern std::int32_t SoundEffectHandle;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//終了フラグ
extern int EndFlag;

//tempデータ
extern int EndFlagTemp, SpTemp;

namespace {

	//各種分岐表示配列
	static constexpr const char* SaveTaskItem[] = { "セーブしますか？", "ロードしますか？", "削除しますか？" };
	static constexpr const char* SaveTaskItemParticle[] = { "に", "を", "を" };

	//セーブデータ用スクリーンショット格納変数
	std::int32_t SaveSnap[SaveDataSlotNum];

	//通常セーブデータ
	struct alignas(4) SaveData_t {
		std::int32_t EndFlag;    //EndFlag
		std::int32_t Sp;			//行数
		std::int32_t Cp;			//文字位置
		std::int32_t Character;		//立ち絵情報
		std::int32_t BackGround;			//背景画像情報
		std::int32_t BackGroundMusic;		//BGM情報
	};

	//セーブデータ用スクリーンショット読込関数
	void SaveDataSnapLoad() {
		std::string FilePath = "DATA/SAVE/SAVESNAP";
		std::string FileFormat = ".png";
		std::string FileName = "";

		for (std::int32_t i = 0; i < SaveDataSlotNum; i++) {

			FileName = (FilePath + std::to_string(i + 1) + FileFormat);

			SaveSnap[i] = DxLib::LoadGraph(FileName.c_str());
		}
	}

	//セーブ/ロード/デリート メニュー描画
	void SaveLoadDeleteMenuDraw(std::int32_t& cursor_y, unsigned int color) {

		//スクリーンショット描画
		for (std::int32_t i = 0; i < SaveDataSlotNum; i++)
			DxLib::DrawRotaGraph(SaveSnapDrawPosX, SaveBasePosY * (i + 1), 0.2f, 0, SaveSnap[i], TRUE);

		//カーソルの描画
		DxLib::DrawString(SaveDataBasePosX, cursor_y, "■", color);

		//セーブデータ名描画
		DxLib::DrawString(SaveDataNamePosX, SaveBasePosY, "セーブデータ1", color);
		DxLib::DrawString(SaveDataNamePosX, SaveBasePosY * 2, "セーブデータ2", color);
		DxLib::DrawString(SaveDataNamePosX, SaveBasePosY * 3, "セーブデータ3", color);

		DxLib::DrawString(SaveDataNamePosX - CursorMove, SaveBasePosY * 4, "戻る", color);
	}

	//セーブ/ロードメニューキー操作
	void SaveLoadMenuKeyMove(std::int32_t& cursor_y) {
		if (DxLib::CheckHitKey(KEY_INPUT_DOWN) == 1)
			cursor_y = (SaveDataPosButtom == cursor_y) ? SaveBasePosY : cursor_y + SaveDataCursorMove;

		if (DxLib::CheckHitKey(KEY_INPUT_UP) == 1)
			cursor_y = (SaveBasePosY == cursor_y) ? SaveDataPosButtom : cursor_y - SaveDataCursorMove;

		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}

	//スクリーンショット名前変更
	void SaveDataScreenShotRename(const int& Num) {
		std::string FilePath = "DATA/SAVE/SAVESNAP";
		std::string FileFormat = ".png";
		std::string FileName = FilePath + std::to_string(Num) + FileFormat;

		std::rename("DATA/SAVE/SAVESNSAP_TEMP.png", FileName.c_str());
	}

	//セーブデータをセーブ
	int SaveDataSave(const char* SaveDataPath, const char* Message, const int& Num) {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData = { EndFlagTemp, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

			FILE *fp;

			fopen_s(&fp, SaveDataPath, "wb");//バイナリファイルを開く
			if (nullptr == fp) {//エラーが起きたらnullptrを返す
				return 0;
			}

			fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t構造体の中身を出力
			fclose(fp);

			//スクリーンショットの名前変更
			SaveDataScreenShotRename(Num);

			MessageBoxOk("セーブしました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
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

			EndFlag = SaveData.EndFlag;
			Sp = SaveData.Sp;
			Cp = SaveData.Cp;
			CharacterHandle = SaveData.Character;
			BackGroundHandle = SaveData.BackGround;
			BackGroundMusicHandle = SaveData.BackGroundMusic;

			MessageBoxOk("ロードしました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

			DrawPointX = 0;
			DrawPointY = 0;
			DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
			DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
			DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
		}
		return 0;
	}

	//セーブデータをデリート
	int SaveDataDelete(const char* SaveDataPath, const char* Message) {
		if (IDYES == MessageBoxYesNo(Message)) {
			std::remove(SaveDataPath);

			MessageBoxOk("削除しました！");
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
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
			SaveFlag = SaveDataSave(SaveDataPath, Message.c_str(), Num);

		//ロード
		if (Num == 2)
			SaveFlag = SaveDataLoad(SaveDataPath, Message.c_str());

		//デリート
		if (Num == 3)
			SaveFlag = SaveDataDelete(SaveDataPath, Message.c_str());
	}

	//セーブ/ロード/デリート メニュー選択処理
	void SaveLoadDeleteMenuSelect(std::int32_t& cursor_y, const int& Num, std::int32_t& SaveFlag) {

		if (cursor_y == SaveBasePosY && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA1.bat", "セーブデータ１", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}


		if (cursor_y == SaveBasePosY * 2 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA2.bat", "セーブデータ２", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}


		if (cursor_y == SaveBasePosY * 3 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {
			SaveDataTask(Num, "DATA/SAVE/SAVEDATA3.bat", "セーブデータ３", SaveFlag);
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}


		if (cursor_y == SaveBasePosY * 4 && DxLib::CheckHitKey(KEY_INPUT_RETURN) == 1) {

			if (IDYES == MessageBoxYesNo("戻りますか？")){
				SaveFlag = 0;
				std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
		}
	}
}

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num) {

	//各種分岐表示
	if (IDYES == MessageBoxYesNo(SaveTaskItem[Num - 1])) {

		//セーブデータループ用フラグ
		std::int32_t SaveFlag = 1;

		//描画される文字列の色
		unsigned int color = DxLib::GetColor(255, 255, 255);

		DxLib::ClearDrawScreen();
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

		//スクリーンショットの読込
		SaveDataSnapLoad();

		//カーソルの初期位置
		std::int32_t save_y = SaveBasePosY;

		//セーブデータループ
		while (SaveFlag == 1) {
			ScreenClear();
			SaveLoadDeleteMenuDraw(save_y, color);
			SaveLoadMenuKeyMove(save_y);
			SaveLoadDeleteMenuSelect(save_y, Num, SaveFlag);
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
}

//セーブデータをセーブ(Quick)
int QuickSaveDataSave() {

	if (IDYES == MessageBoxYesNo("クイックセーブを実行しますか？")) {

		SaveData_t SaveData = { EndFlag, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

		FILE *fp;

		fopen_s(&fp, "DATA/SAVE/QuickSaveData.dat", "wb");//バイナリファイルを開く
		if (nullptr == fp) {//エラーが起きたらnullptrを返す
			return 0;
		}

		fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t構造体の中身を出力
		fclose(fp);

		MessageBoxOk("セーブしました！");
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));
	}
	return 0;
}

//セーブデータをロード(Quick)
int QuickSaveDataLoad() {

	if (IDYES == MessageBoxYesNo("クイックロードを実行しますか？")) {

		SaveData_t SaveData;

		FILE *fp;

		fopen_s(&fp, "DATA/SAVE/QuickSaveData.dat", "rb");
		if (fp == nullptr) {
			//MessageBoxOk(ErrorMessage);
			return 0;
		}

		fread(&SaveData, sizeof(SaveData), 1, fp);
		fclose(fp);

		EndFlag = SaveData.EndFlag;
		Sp = SaveData.Sp;
		Cp = SaveData.Cp;
		CharacterHandle = SaveData.Character;
		BackGroundHandle = SaveData.BackGround;
		BackGroundMusicHandle = SaveData.BackGroundMusic;

		MessageBoxOk("ロードしました！");
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}
	return 0;
}

//セーブデータをセーブ(Continue)
int ContinueSaveDataSave(const std::int32_t& RouteNum) {

	SaveData_t SaveData = { RouteNum, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

	FILE *fp;

	fopen_s(&fp, "DATA/SAVE/ContinueSaveData.dat", "wb");//バイナリファイルを開く
	if (nullptr == fp) {//エラーが起きたらnullptrを返す
		return 0;
	}

	fwrite(&SaveData, sizeof(SaveData), 1, fp); // SaveData_t構造体の中身を出力
	fclose(fp);

	return 0;
}

//セーブデータをロード(Continue)
int ContinueSaveDataLoad() {

	if (IDYES == MessageBoxYesNo("前回プレイした所から再開しますか？")) {

		SaveData_t SaveData;

		FILE *fp;

		fopen_s(&fp, "DATA/SAVE/QuickSaveData.dat", "rb");
		if (fp == nullptr) {
			//MessageBoxOk(ErrorMessage);
			return 0;
		}

		fread(&SaveData, sizeof(SaveData), 1, fp);
		fclose(fp);

		EndFlag = SaveData.EndFlag;
		Sp = SaveData.Sp;
		Cp = SaveData.Cp;
		CharacterHandle = SaveData.Character;
		BackGroundHandle = SaveData.BackGround;
		BackGroundMusicHandle = SaveData.BackGroundMusic;

		MessageBoxOk("ロードしました！");
		std::this_thread::sleep_for(std::chrono::milliseconds(WaitKeyTaskTime));

		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}
	return 0;
}

//セーブデータ用スクリーンショット取得
void SaveDataScreenShotGet() {
	DxLib::SaveDrawScreen(0, 0, 640, 480, "DATA/SAVE/SAVESNSAP_TEMP.png");
}