#pragma once


#include <lib/nlohmann/json.hpp>
#include <Krampus24/GameProgressAndStateInfo.hpp>


namespace Krampus24
{
  void to_json(nlohmann::json& j, const GameProgressAndStateInfo& object);
  void from_json(const nlohmann::json& j, GameProgressAndStateInfo& object);
}


