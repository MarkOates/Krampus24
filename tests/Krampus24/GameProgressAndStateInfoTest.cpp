
#include <gtest/gtest.h>

#include <Krampus24/GameProgressAndStateInfo.hpp>


TEST(Krampus24_GameProgressAndStateInfoTest, can_be_created_without_blowing_up)
{
   Krampus24::GameProgressAndStateInfo game_progress_and_state_info;
}


TEST(Krampus24_GameProgressAndStateInfoTest, TYPE__has_the_expected_value)
{
   EXPECT_STREQ(
     "Krampus24/GameProgressAndStateInfo",
     Krampus24::GameProgressAndStateInfo::TYPE
   );
}


TEST(Krampus24_GameProgressAndStateInfoTest, type__has_the_expected_value_matching_TYPE)
{
   Krampus24::GameProgressAndStateInfo game_progress_and_state_info;
   EXPECT_EQ(Krampus24::GameProgressAndStateInfo::TYPE, game_progress_and_state_info.get_type());
}


