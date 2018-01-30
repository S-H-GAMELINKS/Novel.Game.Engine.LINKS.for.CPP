#ifndef SAVEDATA_H_
#define SAVEDATA_H_

#include <cstdint>

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num);

//セーブデータをセーブ(Quick)
int QuickSaveDataSave();

//セーブデータをロード(Quick)
int QuickSaveDataLoad();

//セーブデータをセーブ(Continue)
int ContinueSaveDataSave(const std::int32_t& RouteNum);

//セーブデータをロード(Continue)
int ContinueSaveDataLoad();

//セーブデータ用スクリーンショット取得
void SaveDataScreenShotGet();

#endif // SAVEDATA_H_