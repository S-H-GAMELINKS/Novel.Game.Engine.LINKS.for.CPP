// Variable Manage Source
#ifndef VALIABLE_H_
#define VALIABLE_H_

#include "ConstantExpressionVariable.h"
#include <vector>
#include <string>
#include <array>

//ゲームタイトル
extern const char* GameTitle = "ノベルゲームエンジン「LINKS」 for C++";

int DrawPointX = 0, DrawPointY = 0;	// 文字列描画の位置
int Sp = 0, Cp = 0;	// 参照する文字列番号と文字列中の文字ポインタ

int EndFlag = 0;	// 終了フラグ

//スクリプト格納変数
static std::vector<std::string> Script;

//各種素材格納変数
static std::vector<int> BackGround, Character, BackGroundMusic, SoundEffect, ImageEffect;
static std::vector<std::string> Movie;

//各種素材ハンドル
std::int32_t BackGroundHandle, CharacterHandle, BackGroundMusicHandle, SoundEffectHandle, ImageEffectHandle, GameTitleGraph;

//画面変遷用Temp変数
int EndFlagTemp, SpTemp;

// 既読スキップ/スキップ/オート変数
int SkipAndAutoFlag = 0;

#endif //VALIABLE_H_