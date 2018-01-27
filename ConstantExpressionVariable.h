#ifndef CONSTANT_EXPRESSION_VARIABLE_H_
#define CONSTANT_EXPRESSION_VARIABLE_H_

//各種素材最大数
constexpr int MaterialMax = 99;

// 文字のサイズ
constexpr int moji_size = 24;

//タイトルメニュー位置
constexpr int title_menu_y = 300;
constexpr int title_menu_x = 430;
constexpr int title_menu_load_y = 330;
constexpr int title_menu_config_y = 360;
constexpr int title_menu_quick_load_y = 390;
constexpr int title_menu_continue_y = 420;
constexpr int title_menu_exit_y = 450;

//タイトルメニューのカーソル位置
constexpr int title_menu_cursor_x = 400;

//選択肢
constexpr int choice_pos_x = 100;
constexpr int choice_pos_y[] = { 200, 230 };

//キーカーソルの移動量
constexpr int cursor_move = 30;

//キー処理停止時間
constexpr int wait_key_task_time = 70;

//ゲームプレイ時の遅延時間
constexpr int wait_game_time = 3;

//ゲームメニュー位置関係
constexpr int game_menu_base_pos_y = 30;

//セーブ・ロード画面関係
constexpr int save_max_num = 3;
constexpr int save_base_pos_x = 400;
constexpr int save_base_pos_y = 100;
constexpr int save_move_unit = 100;
constexpr int save_buttom_y = save_base_pos_y + save_move_unit * save_max_num;
constexpr int save_snap_draw_pos_x = 150;
constexpr int save_name_pos_x = 250;

//文字描画速度関係
constexpr int string_speed_skip = 10;
constexpr int string_speed_auto = 30;

#endif // CONSTANT_EXPRESSION_VARIABLE_H_