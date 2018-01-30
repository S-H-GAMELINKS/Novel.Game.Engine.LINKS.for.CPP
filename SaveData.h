#ifndef SAVEDATA_H_
#define SAVEDATA_H_

//セーブデータ(セーブ/ロード/デリート)ループ
void SaveDataLoop(const int& Num);

//セーブデータをセーブ(Quick)
int QuickSaveDataSave();

//セーブデータをロード(Quick)
int QuickSaveDataLoad();

//セーブデータ用スクリーンショット取得
void SaveDataScreenShotGet();

#endif // SAVEDATA_H_