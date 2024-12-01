#pragma once


#include <Krampus24/GameProgressAndStateInfo.hpp>
#include <ostream>


namespace Krampus24
{
   bool operator==(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   bool operator!=(const GameProgressAndStateInfo& object, const GameProgressAndStateInfo& other_object);
   void PrintTo(const GameProgressAndStateInfo& object, ::std::ostream* os);
}


