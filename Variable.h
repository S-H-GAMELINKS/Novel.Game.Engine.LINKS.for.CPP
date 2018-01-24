// Variable Manage Source
#ifndef VALIABLE_H_
#define VALIABLE_H_

#include "ConstantExpressionVariable.h"
#include <vector>
#include <string>
#include <array>

//Q[^Cg
extern const char* GameTitle = "mxQ[GWuLINKSv for C++";

int DrawPointX = 0, DrawPointY = 0;	// ¶ñ`æÌÊu
int SP = 0, CP = 0;	// QÆ·é¶ñÔÆ¶ñÌ¶|C^

int EndFlag = -1;	// I¹tO

//XNvgi[Ï
static std::vector<std::string> Script;

//wiæi[Ï
std::array<int, MaterialMax> BackGround;

//§¿Gi[Ï
static std::array<int, MaterialMax> Character;

//BGMi[Ï
static std::array<int, MaterialMax> BackGroundMusic;

static std::array<int, MaterialMax> SoundEffect;

//®æi[Ï
static std::array<std::string, MaterialMax> Movie;

//eífÞnh
std::int32_t BackGroundHandle;
std::int32_t CharacterHandle;
std::int32_t BackGroundMusicHandle;
std::int32_t SoundEffectHandle;

std::int32_t GameTitleGraph;

#endif //VALIABLE_H_