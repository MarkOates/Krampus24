
#include <gtest/gtest.h>

#include <Krampus24/Testing/Comparison/Krampus24/GameProgressAndStateInfo.hpp>


TEST(AllegroFlare_Testing_Comparison_Krampus24_GameProgressAndStateInfoTest,
   PrintTo__with_an_AllegroFlare_GameProgressAndStateInfo__will_output_as_expected)
{
   std::stringstream ss;
   Krampus24::GameProgressAndStateInfo object;
   object.set_player_inventory_items({ "fruit", "veggie", "fork" });

   PrintTo(object, &ss);

   std::string expected_output =
      "GameProgressAndStateInfo(player_inventory_items: {\"fruit\", \"veggie\", \"fork\", }, )";
   std::string actual_output = ss.str();
   EXPECT_EQ(expected_output, actual_output);
}


TEST(AllegroFlare_Testing_Comparison_Krampus24_GameProgressAndStateInfoTest,
   equality_operator__works_with_google_test_EXPECT_statement)
{
   Krampus24::GameProgressAndStateInfo object;
   Krampus24::GameProgressAndStateInfo other_object;

   EXPECT_EQ(object, other_object);
}


