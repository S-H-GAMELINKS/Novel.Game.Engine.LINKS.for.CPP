//Script Tag Task Maneger Source
#define _SCL_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 


#include "DxLib.h"
#include "ConstantExpressionVariable.h"
#include "Utility.h"
#include "Choice.h"
#include "ConfigMenu.h"
#include "BackLog.h"
#include "MouseAndKeyState.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <array>
#include <thread>
#include <chrono>
#include <boost/xpressive/xpressive.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace boost::xpressive;

extern int DrawPointX, DrawPointY;	// 文字列描画の位置
extern int Sp, Cp;	// 参照する文字列番号と文字列中の文字ポインタ

//終了フラグ
extern int EndFlag;

//各種素材ハンドル
extern std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle, ImageEffectHandle;

// 既読スキップ/スキップ/オート変数
extern int SkipAndAutoFlag;

extern struct ConfigData_t ConfigData;

//素材用エイリアス
template <class T>
using Material = std::vector<T>;

//スクリプト用エイリアス
using Script = std::vector<std::string>;

//立ち絵削除＆ゲームオーバー用エイリアス
using unique = std::unique_ptr<int>;

//タグ正規表現用エイリアス
using ScriptTag = const std::pair<std::string, std::string>;

namespace ScriptTask {

	char OneMojiBuf[3];	// １文字分一時記憶配列

	//スクリプトタグ処理
	std::vector<std::string> LoadingScriptTag() {

		std::vector<std::string> Str, Container, Temp;
		
		std::fstream file("DATA/STR/ScriptTag.txt", std::ios_base::in);
		
		for (std::string line; std::getline(file, line); ) if (!line.empty()) Str.emplace_back(std::move(line));
		
			for (auto&& s : Str) {
				boost::algorithm::split(Temp, s, boost::is_any_of(",")); // カンマで分割
			
				for (auto&& t : Temp)
				Container.emplace_back(std::move(t));
			}

		file.close();

		return Container;
	}

	//スクリプトタグ反映処理
	std::vector<std::pair<std::string, std::string>> SettingScriptTag(const std::vector<std::string> Container) {

		std::vector<std::pair<std::string, std::string>> ScriptTag;
		
		for (unsigned int i = 0, j = 0; i < Container.size(); i += 2, j++)
			ScriptTag.emplace_back(std::move(std::make_pair(Container[i], Container[i + 1])));

		return ScriptTag;
	}

	std::vector<std::string> ScriptTagContainer = LoadingScriptTag();

	//タグ正規表現
	std::vector<std::pair<std::string, std::string>> Tag = SettingScriptTag(ScriptTagContainer);

	// 改行関数
	void Kaigyou() noexcept {
		int TempGraph;

		// 描画行位置を一つ下げる
		DrawPointY++;

		// 描画列を最初に戻す
		DrawPointX = 0;

		// もし画面からはみ出るなら画面をスクロールさせる
		if (DrawPointY * MojiSize + MojiSize > 480)
		{
			// テンポラリグラフィックの作成
			TempGraph = MakeGraph(640, 480);

			// 画面の内容を丸々コピーする
			GetDrawScreenGraph(0, 0, 640, 480, TempGraph);

			// 一行分上に貼り付ける
			DrawGraph(0, -MojiSize, TempGraph, FALSE);

			// 一番下の行の部分を黒で埋める
			DrawBox(0, 480 - MojiSize, 640, 480, 0, TRUE);

			// 描画行位置を一つあげる
			DrawPointY--;

			// グラフィックを削除する
			DeleteGraph(TempGraph);
		}
	}

	//文字速度関数
	void DrawSpeed(const int& Speed, const int& SpeedVolume) {
		std::this_thread::sleep_for(std::chrono::milliseconds(SpeedVolume * Speed / 100));
	}

	//文字列描画速度
	void DrawScriptSpeed() noexcept {
		switch (SkipAndAutoFlag) {
		case 0:
			DrawSpeed(StringSpeedAuto, ConfigData.ScriptDrawSpeedVolume);
			break;

		case 1:
			DrawSpeed(StringSpeedSkip, ConfigData.SkipSpeedVolumeCount);
			break;

		case 2:
			DrawSpeed(StringSpeedAuto, ConfigData.AutoSpeedVolume);
			break;
		}
	}

	//文字列描画関数
	void DrawScript(Script& Script) noexcept {

		std::string str = "";

		if (Script[Sp] == str)
			Sp++;
		else {
			// １文字分抜き出す
			OneMojiBuf[0] = Script[Sp][Cp];
			OneMojiBuf[1] = Script[Sp][Cp + 1];
			OneMojiBuf[2] = '\0';

			// １文字描画
			DrawString(DrawPointX * MojiSize, DrawPointY * MojiSize, OneMojiBuf, Color);

			// 参照文字位置を２バイト勧める
			Cp += 2;

			// カーソルを一文字文進める
			DrawPointX++;

			// 少し待つ
			DrawScriptSpeed();
		}
	}

	//素材番号処理
	int MaterialNumCheck(Script& Script, const std::pair<std::string, std::string>& Tag) {

		std::string str = Script[Sp];

		smatch what;

		std::array<std::string, 2> ScriptTagContainer = { Tag.first, Tag.second };

		for (std::int32_t i = 0; i < 2; i++) {

			sregex rex = sregex::compile(ScriptTagContainer[i]);

			if (regex_search(str, what, rex)) {

				std::string text(what[1]);
				int n = std::stoi(text);

				Script[Sp] = regex_replace(str, rex, "");
				Cp = 0;

				return n - 1;
			}
		}

		return 0;
	}

	//背景画像＆イメージエフェクト描画関数
	template <typename T, typename Func>
	void DrawImages(Script& Script, Material<T>& Material, Func&& DrawFunc, T& Handle, const std::pair<std::string, std::string> Tag) noexcept {
		Handle = Material[MaterialNumCheck(Script, Tag)];
		DrawFunc(Handle);
	}

	//立ち絵削除処理関数
	void RemoveCharacterGraph() noexcept {
		unique CharacterDummy = std::make_unique<int>(DxLib::DerivationGraph(CharacterPosX, CharacterPosY, CharacterGraphSizeWidth, CharacterGraphSizeHeight, BackGroundHandle));
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, *CharacterDummy, true);
	}

	//立ち絵描画関数
	void DrawCharacter(Script& Script, Material<int>& Character) noexcept {
		Cp++;

		ScriptTask::RemoveCharacterGraph();

		CharacterHandle = Character[MaterialNumCheck(Script, Tag[1])];
		DxLib::DrawGraph(CharacterPosX, CharacterPosY, CharacterHandle, TRUE);
	}

	//音量セット関数
	void ChangeSoundVolumne() {
			DxLib::ChangeVolumeSoundMem(255 * ConfigData.BackGroundMusicVolume / 100, BackGroundMusicHandle);
			DxLib::ChangeVolumeSoundMem(255 * ConfigData.SoundEffectVolume / 100, SoundEffectHandle);
	}

	//音源再生確認関数
	void CheckSoundPlay(const int& Handle) {
		//BGM再生中の場合は、BGMを停止する
		if (DxLib::CheckSoundMem(Handle))
			DxLib::StopSoundMem(Handle);
	}

	//音源再生関数
	template <typename T>
	void PlaySounds(Script& Script, Material<int>& Material, T& Handle, const T& PlayType, const std::pair<std::string, std::string> Tag) noexcept {

		CheckSoundPlay(Handle);

		Cp++;
		Handle = Material[MaterialNumCheck(Script, Tag)];

		ChangeSoundVolumne();

		DxLib::PlaySoundMem(Handle, PlayType);
	}

	//動画再生関数
	void PlayMovie(Script& Script, Material<std::string>& Movie) noexcept {
		Cp++;
		DxLib::PlayMovie(Movie[MaterialNumCheck(Script, Tag[4])].c_str(), 1, DX_MOVIEPLAYTYPE_BCANCEL);
	}

	//画面クリア処理関数
	void ClearScreen(Script& Script) noexcept {
		BackLogGet();
		ClearDrawScreen();
		BackGroundHandle = 0;
		CharacterHandle = 0;
		DrawPointY = 0;
		DrawPointX = 0;

		std::string str = Script[Sp];

		sregex rex = sregex::compile("R");
		smatch what;

		regex_search(str, what, rex);

		Script[Sp] = regex_replace(str, rex, "");
		Cp = 0;
	}

	//コメント処理関数
	void Comment(Script& Script) noexcept {
		if (Script[Sp][Cp] == '/') {
			Cp = 0;
			Sp++;
		}
	}

	//クリック待ち処理関数
	void ClickWait() noexcept {
		if (SkipAndAutoFlag == 0) {
			DxLib::WaitKey();
			if (MouseAndKey::CheckMouseAndKeyEnter())
				Cp = 0;//Cp++;
		}

		if (SkipAndAutoFlag == 1)
			Cp++;

		if (SkipAndAutoFlag == 2) {
			std::this_thread::sleep_for(std::chrono::milliseconds((StringSpeedAuto * StringSpeedWait * ConfigData.AutoSpeedVolume / 100)));
			Cp++;
		}
	}

	//ゲームオーバー画面の描画
	void GameOverScreenDraw() {
		unique GameOverHandle = std::make_unique<std::int32_t>(DxLib::LoadGraph("DATA/BACKGROUND/GAMEOVER.png"));
		DxLib::DrawGraph(0, 0, *GameOverHandle, TRUE);
	}

	//タグチェック関数
	bool ScriptTagCheck(const Script& Script, ScriptTag& Tag) {

		sregex rex = sregex::compile(Tag.first);
		smatch what;

		if (regex_search(Script[Sp], what, rex))
			return true;

		rex = sregex::compile(Tag.second);

		if (regex_search(Script[Sp], what, rex))
			return true;

		return false;
	}

	//各種素材描画
	bool DrawMaterial(Material<std::string>& Script, Material<int>& BackGround, Material<int>& Character, Material<int>& BackGroundMusic, Material<int>& SoundEffect, Material<std::string>& Movie, Material<int>& ImageEffect) {
		if (ScriptTask::ScriptTagCheck(Script, Tag[0])) {	//背景画像描画
			ScriptTask::DrawImages(Script, BackGround, [](int Handle) {DxLib::DrawGraph(0, 0, Handle, TRUE); }, BackGroundHandle, Tag[0]);
			Cp = 0;
			return true;
		}

		if (ScriptTask::ScriptTagCheck(Script, Tag[1])) {	//立ち絵画像描画
			ScriptTask::DrawCharacter(Script, Character);
			return true;
		}

		if (ScriptTask::ScriptTagCheck(Script, Tag[2])) {	//BGM再生
			ScriptTask::PlaySounds(Script, BackGroundMusic, BackGroundMusicHandle, DX_PLAYTYPE_LOOP, Tag[2]);
			return true;
		}

		if (ScriptTask::ScriptTagCheck(Script, Tag[3])) {	//SE再生
			ScriptTask::PlaySounds(Script, SoundEffect, SoundEffectHandle, DX_PLAYTYPE_BACK, Tag[3]);
			return true;
		}

		if (ScriptTask::ScriptTagCheck(Script, Tag[4])) {	//動画再生
			ScriptTask::PlayMovie(Script, Movie);
			return true;
		}

		if (ScriptTask::ScriptTagCheck(Script, Tag[5])) {	//イメージエフェクト描画
			ScriptTask::DrawImages(Script, ImageEffect, [](int Handle) { DxLib::DrawGraph(0, 0, Handle, TRUE); }, ImageEffectHandle, Tag[5]);
			return true;
		}

		return false;
	}

	//システムタグチェック関数
	bool SystemTag(Material <std::string>& Script, ScriptTag& Tag) {

		std::string Str = Script[Sp];

		smatch what;

		std::array<std::string, 2> ScriptTagContainer = { Tag.first, Tag.second };

		for (int i = Cp; i < Script[Sp].length(); i++) {
			if (Script[Sp][i] == Tag.first[0] || Script[Sp][i] == Tag.second[0]) {

				for (std::int32_t i = 0; i < 2; i++) {

					sregex rex = sregex::compile(ScriptTagContainer[i]);

					if (regex_match(Script[Sp], what, rex)) {

						Script[Sp] = regex_replace(Str, rex, "");

						Cp = 0;

						return true;
					}
				}
			}
		}


		return false;
	}

	//各種システム処理
	bool SystemTagTask(Material<std::string>& Script) {

		if (SystemTag(Script, Tag[6])) {		//改行
			ScriptTask::Kaigyou();
			return true;
		}

		if (SystemTag(Script, Tag[7])) {		//クリック待ち
			ScriptTask::ClickWait();
			return true;
		}

		if (SystemTag(Script, Tag[8])) {		//画面クリア処理
			ScriptTask::ClearScreen(Script);
			return true;
		}

		if (SystemTag(Script, Tag[9])) {		//遅延処理
			std::this_thread::sleep_for(std::chrono::seconds(WaitGameTime));
			return true;
		}

		if (SystemTag(Script, Tag[10])) {		//ゲームオーバー画面描画
			ScriptTask::GameOverScreenDraw();
			return true;
		}

		if (SystemTag(Script, Tag[11])) {		//エンディング画面描画
			DxLib::PlayMovie("DATA/MOVIE/ENDING.wmv", 1, DX_MOVIEPLAYTYPE_NORMAL);
			return true;
		}

		if (SystemTag(Script, Tag[12])) {		//BGM停止
			DxLib::StopSoundMem(BackGroundMusicHandle);
			return true;
		}

		if (SystemTag(Script, Tag[13])) {		//SE停止
			DxLib::StopSoundMem(SoundEffectHandle);
			return true;
		}

		if (SystemTag(Script, Tag[14])) {		//選択肢
			ChoiceSelect(EndFlag);
			return true;
		}

		if (SystemTag(Script, Tag[15])) {		//ゲーム終了
			EndFlag = 99;
			return true;
		}

		return false;
	}
}

//スクリプトタグ処理関数
void ScriptTagTaskManager(Material<std::string>& Script, Material<int>& BackGround, Material<int>& Character, Material<int>& BackGroundMusic, Material<int>& SoundEffect, Material<std::string>& Movie, Material<int>& ImageEffect) noexcept {

	switch (Script[Sp][Cp])
	{

	case '/':	//コメント
		Cp++;
		ScriptTask::Comment(Script);
		break;

	case '@':	//立ち絵を画面から削除
		ScriptTask::RemoveCharacterGraph();
		Cp++;
		break;

	case '#':	//キャラクター名描画
		Cp++;
		break;

	case ' ':
		Cp++;
		break;

	default:	// その他の文字

				//システム用タグ処理
		if (ScriptTask::SystemTagTask(Script))
			break;

		//各種素材描画
		if (ScriptTask::DrawMaterial(Script, BackGround, Character, BackGroundMusic, SoundEffect, Movie, ImageEffect))
			break;


		//文字列描画
		ScriptTask::DrawScript(Script);

		// 画面からはみ出たら改行する
		if (DrawPointX * MojiSize + MojiSize > 640) ScriptTask::Kaigyou();

		break;
	}
}