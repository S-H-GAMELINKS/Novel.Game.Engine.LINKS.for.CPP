// Variable Manage Source
#ifndef VALIABLE_H_
#define VALIABLE_H_

#include <vector>
#include <string>
#include <array>

//ゲームタイトル
extern const char* GameTitle = "ノベルゲームエンジン「LINKS」 for C++";

// 文字のサイズ
extern const std::int32_t moji_size = 24;

int DrawPointX = 0, DrawPointY = 0;	// 文字列描画の位置
int SP = 0, CP = 0;	// 参照する文字列番号と文字列中の文字ポインタ

int EndFlag = 0;	// 終了フラグ

//スクリプト格納変数
static std::vector<std::string> Script;

//背景画像格納変数
std::array<int, 99> BackGround;

//立ち絵格納変数
static std::array<int, 99> Character;

//BGM格納変数
static std::array<int, 99> BackGroundMusic;

static std::array<int, 99> SoundEffect;

//動画格納変数
static std::array<std::string, 99> Movie;

#endif //VALIABLE_H_