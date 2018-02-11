#ifndef SCRIPT_TASK_MANAGER_CPP_
#define SCRIPT_TASK_MANAGER_CPP_

#include "ConstantExpressionVariable.h"
#include <vector>
#include <string>
#include <array>

//スクリプトタグ処理関数
void ScriptTagTaskManager(const std::vector<std::string>& Script, const std::array<int, MaterialMax>& BackGround, const std::array<int, MaterialMax>& Character, const std::array<int, MaterialMax>& BackGroundMusic, const std::array<int, MaterialMax>& SoundEffect, const std::array<std::string, MaterialMax>& Movie, const std::array<int, MaterialMax>& ImageEffect) noexcept;

#endif //SCRIPT_TASK_MANAGER_CPP_