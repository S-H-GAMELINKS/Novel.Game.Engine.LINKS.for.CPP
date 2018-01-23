#include "DxLib.h"

//‰æ–ÊƒNƒŠƒAˆ—ŠÖ”
void ScreenClear() noexcept {
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	SetDrawScreen(DX_SCREEN_FRONT);
}
