#ifndef SCRIPT_TASK_MANAGER_CPP_
#define SCRIPT_TASK_MANAGER_CPP_

#include "ConstantExpressionVariable.hpp"
#include <vector>
#include <string>
#include <array>

//各種素材格納変数
template <typename T>
using Material = std::vector<T>;

//スクリプトタグ処理関数
void ScriptTagTaskManager(Material<std::string>& Script, Material<int>& BackGround, Material<int>& Character, Material<int>& BackGroundMusic, Material<int>& SoundEffect, Material<std::string>& Movie, Material<int>& ImageEffect) noexcept;

#endif //SCRIPT_TASK_MANAGER_CPP_