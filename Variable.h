// Variable Manage Source
#ifndef VALIABLE_H_
#define VALIABLE_H_

#include <vector>
#include <string>

// •¶š‚ÌƒTƒCƒY
extern const std::int32_t moji_size = 24;

int DrawPointX = 0, DrawPointY = 0;	// •¶š—ñ•`‰æ‚ÌˆÊ’u
int SP = 0, CP = 0;	// QÆ‚·‚é•¶š—ñ”Ô†‚Æ•¶š—ñ’†‚Ì•¶šƒ|ƒCƒ“ƒ^

int EndFlag = 0;	// I—¹ƒtƒ‰ƒO

					//ƒXƒNƒŠƒvƒgŠi”[•Ï”
static std::vector<std::string> Script;

//”wŒi‰æ‘œŠi”[•Ï”
static std::vector<int> BackGround;

//—§‚¿ŠGŠi”[•Ï”
static std::vector<int> Character;

//BGMŠi”[•Ï”
static std::vector<int> BackGroundMusic;

static std::vector<int> SoundEffect;

//“®‰æŠi”[•Ï”
static std::vector<std::string> Movie;

#endif //VALIABLE_H_