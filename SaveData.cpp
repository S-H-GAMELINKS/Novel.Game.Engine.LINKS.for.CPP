// SaveData Source

#include "DxLib.h"
#include "ConstantExpressionVariable.hpp"
#include "Utility.hpp"
#include "ConfigMenu.hpp"
#include "MouseAndKeyState.hpp"
#include <array>
#include <string>
#include <thread>
#include <chrono>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

extern int Sp, Cp;	// 参照する文字列番号と文字列中の文字ポインタ

//各種素材ハンドル
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle;

//文字列描画位置
extern int DrawPointX, DrawPointY;

//終了フラグ
extern int EndFlag;

//tempデータ
extern int EndFlagTemp, SpTemp;

extern ConfigData_t ConfigData;

namespace {

	//各種分岐表示配列
	static constexpr const char* SaveTaskItem[] = { "セーブしますか？", "ロードしますか？", "削除しますか？" };
	static constexpr const char* SaveTaskItemParticle[] = { "に", "を", "を" };

	//セーブデータ用スクリーンショット格納変数
	std::int32_t SaveSnap[SaveDataSlotNum];

	//セーブデータ名描画用配列
	static constexpr const char* SaveDataName[] = { "セーブデータ1", "セーブデータ2", "セーブデータ3" };

	//セーブデータPath配列
	static constexpr const char* SaveDataPath[] = { "DATA/SAVE/SAVEDATA1.bat", "DATA/SAVE/SAVEDATA2.bat", "DATA/SAVE/SAVEDATA3.bat" };

	//セーブデータ用スクリーンショットPath
	const std::array<std::string, 3> SaveDataSnapPath = { "DATA/SAVE/SAVESNAP1.png", "DATA/SAVE/SAVESNAP2.png", "DATA/SAVE/SAVESNAP3.png" };

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
	void SaveDataSnapLoad() noexcept {

		for (std::int32_t i = 0; i < SaveDataSlotNum; i++)
			SaveSnap[i] = DxLib::LoadGraph(SaveDataSnapPath[i].c_str());
	}

	//セーブデータ メニュー描画
	void SaveDataMenuDraw(std::int32_t& SaveDataMenuPosY) noexcept {

		//スクリーンショット描画 && セーブデータ名描画
		for (std::int32_t i = 0; i < SaveDataSlotNum; i++) {
			DxLib::DrawRotaGraph(SaveSnapDrawPosX, SaveDataBasePosY * (i + 1), 0.2f, 0, SaveSnap[i], TRUE);
			DxLib::DrawString(SaveDataNamePosX, SaveDataBasePosY * (i + 1), SaveDataName[i], Color);
		}

		//カーソルの描画
		DxLib::DrawString(SaveDataBasePosX, SaveDataMenuPosY, "■", Color);

		DxLib::DrawString(SaveDataNamePosX - CursorMove, SaveDataBasePosY * 4, "戻る", Color);
	}

	//スクリーンショット名前変更
	void SaveDataScreenShotRename(const int& Num) noexcept {

		const fs::path path("DATA/SAVE/SAVESNSAPTEMP.png");
		const fs::path dest(SaveDataSnapPath[Num - 1].c_str());

		fs::copy_file(path, dest, fs::copy_option::overwrite_if_exists);
	}

	//セーブデータ適用処理
	void LoadingSaveData(const SaveData_t& SaveData) {
		EndFlag = SaveData.EndFlag;
		Sp = SaveData.Sp;
		Cp = SaveData.Cp;
		CharacterHandle = SaveData.Character;
		BackGroundHandle = SaveData.BackGround;
		BackGroundMusicHandle = SaveData.BackGroundMusic;
	}

	//セーブデータ読込後処理
	void DrawGameScreen() {
		DrawPointX = 0;
		DrawPointY = 0;
		DxLib::PlaySoundMem(BackGroundMusicHandle, DX_PLAYTYPE_LOOP);
		DxLib::DrawGraph(0, 0, BackGroundHandle, TRUE);
		DxLib::DrawGraph(150, 130, CharacterHandle, TRUE);
	}

	//セーブデータをセーブ
	int SaveDataSave(const char* SaveDataPath, const char* Message, const int& Num) noexcept {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData = { EndFlagTemp, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

			FILE *Fp;

			fopen_s(&Fp, SaveDataPath, "wb");//バイナリファイルを開く
			if (nullptr == Fp) {//エラーが起きたらnullptrを返す
				return 0;
			}

			fwrite(&SaveData, sizeof(SaveData), 1, Fp); // SaveData_t構造体の中身を出力
			fclose(Fp);

			//スクリーンショットの名前変更
			SaveDataScreenShotRename(Num);

			MessageBoxOk("セーブしました！");
			WaitTime();
		}
		return 0;
	}

	//セーブデータをロード
	int SaveDataLoad(const char* SaveDataPath, const char* Message) noexcept {

		if (IDYES == MessageBoxYesNo(Message)) {

			SaveData_t SaveData;

			FILE *Fp;

			fopen_s(&Fp, SaveDataPath, "rb");
			if (Fp == nullptr) {
				MessageBoxOk("セーブデータをロードできません");
				return 0;
			}

			fread(&SaveData, sizeof(SaveData), 1, Fp);
			fclose(Fp);

			LoadingSaveData(SaveData);

			MessageBoxOk("ロードしました！");
			WaitTime();

			DrawGameScreen();
		}
		return 0;
	}

	//セーブデータをデリート
	int SaveDataDelete(const char* SaveDataPath, const char* Message) noexcept {
		if (IDYES == MessageBoxYesNo(Message)) {
			std::remove(SaveDataPath);

			MessageBoxOk("削除しました！");
			WaitTime();
		}
		return 0;
	}

	//セーブ/ロード/デリート切り替え関数
	void SaveDataTask(const int& Num, const char* SaveDataPath, const char* SaveDataName, std::int32_t& SaveFlag, const int& SaveSlot) noexcept {

		std::string Message = SaveDataName;
		Message += SaveTaskItemParticle[Num - 1];
		Message += SaveTaskItem[Num - 1];

		//セーブ
		if (Num == 1)
			SaveFlag = SaveDataSave(SaveDataPath, Message.c_str(), SaveSlot);

		//ロード
		if (Num == 2)
			SaveFlag = SaveDataLoad(SaveDataPath, Message.c_str());

		//デリート
		if (Num == 3)
			SaveFlag = SaveDataDelete(SaveDataPath, Message.c_str());
	}

	//セーブデータ メニュー選択処理
	void SaveDataMenuSelect(std::int32_t& SaveDataMenuPosY, const int& Num, std::int32_t& SaveFlag) noexcept {

		for (std::int32_t i = 0; i < 3; i++) {
			if (SaveDataMenuPosY == SaveDataBasePosY * (i + 1) && MouseAndKey::CheckMouseAndKeyEnter()) {
				SaveDataTask(Num, SaveDataPath[i], SaveDataName[i], SaveFlag, i + 1);
				WaitTime();
			}
		}

		if (SaveDataMenuPosY == SaveDataBasePosY * 4 && MouseAndKey::CheckMouseAndKeyEnter()) {

			if (IDYES == MessageBoxYesNo("戻りますか？")){
				SaveFlag = 0;
				WaitTime();
			}
			WaitTime();
		}
	}
}

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num) noexcept {

	//各種分岐表示
	if (IDYES == MessageBoxYesNo(SaveTaskItem[Num - 1])) {

		//セーブデータループ用フラグ
		std::int32_t SaveFlag = 1;

		DxLib::ClearDrawScreen();
		WaitTime();

		//スクリーンショットの読込
		SaveDataSnapLoad();

		//カーソルの初期位置
		std::int32_t SaveDataMenuPosY = SaveDataBasePosY;

		using namespace KeyState;
		using namespace MouseState;

		//セーブデータループ
		while (SaveFlag == 1) {
			ScreenClear();
			SaveDataMenuDraw(SaveDataMenuPosY);
			SaveDataMenuPosY = KeyMoveTemplate(SaveDataMenuPosY, SaveDataBasePosY, SaveDataPosButtom, SaveDataCursorMove, ConfigData.MouseAndKeyFlag);
			SaveDataMenuPosY = MouseMoveTemplate(SaveDataMenuPosY, SaveDataBasePosY, SaveDataBasePosY, 4, ConfigData.MouseAndKeyFlag);
			SaveDataMenuSelect(SaveDataMenuPosY, Num, SaveFlag);
		}
	}

	WaitTime();
}

//セーブデータをセーブ(Quick)
int QuickSaveDataSave() noexcept {

	if (IDYES == MessageBoxYesNo("クイックセーブを実行しますか？")) {

		SaveData_t SaveData = { EndFlag, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

		FILE *Fp;

		fopen_s(&Fp, "DATA/SAVE/QuickSaveData.dat", "wb");//バイナリファイルを開く
		if (nullptr == Fp) {//エラーが起きたらnullptrを返す
			return 0;
		}

		fwrite(&SaveData, sizeof(SaveData), 1, Fp); // SaveData_t構造体の中身を出力
		fclose(Fp);

		MessageBoxOk("セーブしました！");
		WaitTime();
	}
	return 0;
}

//セーブデータをロード(Quick)
int QuickSaveDataLoad() noexcept {

	if (IDYES == MessageBoxYesNo("クイックロードを実行しますか？")) {

		SaveData_t SaveData;

		FILE *Fp;

		fopen_s(&Fp, "DATA/SAVE/QuickSaveData.dat", "rb");
		if (Fp == nullptr) {
			//MessageBoxOk(ErrorMessage);
			return 0;
		}

		fread(&SaveData, sizeof(SaveData), 1, Fp);
		fclose(Fp);

		LoadingSaveData(SaveData);

		MessageBoxOk("ロードしました！");
		WaitTime();

		DrawGameScreen();
	}
	return 0;
}

//セーブデータをセーブ(Continue)
int ContinueSaveDataSave(const std::int32_t& RouteNum) noexcept {

	SaveData_t SaveData = { RouteNum, SpTemp, 0, CharacterHandle, BackGroundHandle, BackGroundMusicHandle };

	FILE *Fp;

	fopen_s(&Fp, "DATA/SAVE/ContinueSaveData.dat", "wb");//バイナリファイルを開く
	if (nullptr == Fp) {//エラーが起きたらnullptrを返す
		return 0;
	}

	fwrite(&SaveData, sizeof(SaveData), 1, Fp); // SaveData_t構造体の中身を出力
	fclose(Fp);

	return 0;
}

//セーブデータをロード(Continue)
int ContinueSaveDataLoad() noexcept {

	if (IDYES == MessageBoxYesNo("前回プレイした所から再開しますか？")) {

		SaveData_t SaveData;

		FILE *Fp;

		fopen_s(&Fp, "DATA/SAVE/QuickSaveData.dat", "rb");
		if (Fp == nullptr) {
			MessageBoxOk("ロードできません");
			return 0;
		}

		fread(&SaveData, sizeof(SaveData), 1, Fp);
		fclose(Fp);

		LoadingSaveData(SaveData);

		MessageBoxOk("ロードしました！");
		WaitTime();

		DrawGameScreen();
	}
	return 0;
}

//セーブデータ用スクリーンショット取得
void SaveDataScreenShotGet() noexcept {
	DxLib::SaveDrawScreen(0, 0, 640, 480, "DATA/SAVE/SAVESNSAPTEMP.png");
}