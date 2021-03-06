#ifndef UTILITY_H_
#define UTILITY_H_

//メッセージボックス(Yes or No)
int MessageBoxYesNo(const LPCTSTR lpText) noexcept;

//メッセージボックス(OK)
int MessageBoxOk(const LPCTSTR lpText) noexcept;

//画面クリア処理関数c
void ScreenClear() noexcept;

//ゲーム終了確認ウインドウ
void GameEndMessageBox() noexcept;

//各種ショートカットキー
void ShortCutKey() noexcept;

//タイトルムービー処理
void TitleMoviePlay() noexcept;

//ウェイト処理
void WaitTime() noexcept;

#endif // !UTILITY_H_
