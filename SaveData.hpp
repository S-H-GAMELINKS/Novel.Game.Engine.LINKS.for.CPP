#ifndef SAVEDATA_H_
#define SAVEDATA_H_

#include <cstdint>

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num) noexcept;

//セーブデータをセーブ(Quick)
int QuickSaveDataSave() noexcept;

//セーブデータをロード(Quick)
int QuickSaveDataLoad() noexcept;

//セーブデータをセーブ(Continue)
int ContinueSaveDataSave(const std::int32_t& RouteNum) noexcept;

//セーブデータをロード(Continue)
int ContinueSaveDataLoad() noexcept;

//セーブデータ用スクリーンショット取得
void SaveDataScreenShotGet() noexcept;

#endif // SAVEDATA_H_