#ifndef SKIP_AUTO_H_
#define SKIP_AUTO_H_

#include <cstdint>

//既読スキップデータ書き込み
void SkipDataWrite(const std::int32_t& RouteNum) noexcept;

//既読判定
void SkipDataCheck(const std::int32_t& RouteNum) noexcept;

//既読スキップデータの読込
int SkipDataLoad() noexcept;

//SKIP_READ SAVE関数
int SkipDataSave() noexcept;

// 既読スキップ/スキップ/オート切り替え関数
void SkipAndAutoTask(const std::int32_t& Num) noexcept;

#endif // !SKIP_AUTO_H_

