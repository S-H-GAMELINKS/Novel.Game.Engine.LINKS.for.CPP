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
	std::int32_t bgm_vol;				//BGM音量情報
	std::int32_t bgm_vol_count;			//BGM音量メーター情報
	std::int32_t se_vol;					//SE音量情報
	std::int32_t se_vol_count;			//SE音量メーター情報
	std::int32_t skip_speed;				//スキップ速度情報
	std::int32_t skip_speed_count;		//スキップ速度メーター情報
	std::int32_t auto_speed;				//オート速度情報
	std::int32_t auto_speed_count;		//オート速度メーター情報
	std::int32_t string_speed;			//文字列描画速度
	std::int32_t string_speed_count;		//文字列描画速度メーター情報
	std::int32_t mouse_key_move;			//マウス操作とキー操作の情報 true/false
};

#endif // CONFIG_MENU_H_
