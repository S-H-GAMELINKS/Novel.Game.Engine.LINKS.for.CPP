#ifndef CONFIG_MENU_H_
#define CONFIG_MENU_H_

#include <cstdint>

//コンフィグ画面ループ
void ConfigMenuLoop() noexcept;

//コンフィグ読込関数
int ConfigLoad() noexcept;

//コンフィグ保存関数
int ConfigSave() noexcept;

//設定データ
struct alignas(4) ConfigData_t {
	std::int32_t BackGroundMusicVolume;				//BGM音量情報
	std::int32_t BackGroundMusicVolumeCount;			//BGM音量メーター情報
	std::int32_t SoundEffectVolume;					//SE音量情報
	std::int32_t SoundEffectVolumeCount;			//SE音量メーター情報
	std::int32_t SkipSpeedVolume;				//スキップ速度情報
	std::int32_t SkipSpeedVolumeCount;		//スキップ速度メーター情報
	std::int32_t AutoSpeedVolume;				//オート速度情報
	std::int32_t AutoSpeedVolumeCount;		//オート速度メーター情報
	std::int32_t ScriptDrawSpeedVolume;			//文字列描画速度
	std::int32_t ScriptDrawSpeedVolumeCount;		//文字列描画速度メーター情報
	std::int32_t MouseAndKeyFlag;			//マウス操作とキー操作の情報 true/false
};

#endif // CONFIG_MENU_H_
