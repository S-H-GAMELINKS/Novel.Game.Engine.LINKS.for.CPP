// Variable Manage Source
#ifndef VALIABLE_H_
#define VALIABLE_H_

#include "ConstantExpressionVariable.h"
#include <vector>
#include <string>
#include <array>

//ƒQ[ƒ€ƒ^ƒCƒgƒ‹
extern const char* GameTitle = "ƒmƒxƒ‹ƒQ[ƒ€ƒGƒ“ƒWƒ“uLINKSv for C++";

int DrawPointX = 0, DrawPointY = 0;	// •¶š—ñ•`‰æ‚ÌˆÊ’u
int SP = 0, CP = 0;	// QÆ‚·‚é•¶š—ñ”Ô†‚Æ•¶š—ñ’†‚Ì•¶šƒ|ƒCƒ“ƒ^

int EndFlag = -1;	// I—¹ƒtƒ‰ƒO

//ƒXƒNƒŠƒvƒgŠi”[•Ï”
static std::vector<std::string> Script;

//”wŒi‰æ‘œŠi”[•Ï”
std::array<int, MaterialMax> BackGround;

//—§‚¿ŠGŠi”[•Ï”
static std::array<int, MaterialMax> Character;

//BGMŠi”[•Ï”
static std::array<int, MaterialMax> BackGroundMusic;

static std::array<int, MaterialMax> SoundEffect;

//“®‰æŠi”[•Ï”
static std::array<std::string, MaterialMax> Movie;

std::int32_t GameTitleGraph;

#endif //VALIABLE_H_