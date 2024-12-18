
#include <gtest/gtest.h>

#include <Krampus/Gameplay/PlayerLookingAtResolver.hpp>


TEST(Krampus_Gameplay_PlayerLookingAtResolverTest, can_be_created_without_blowing_up)
{
   Krampus::Gameplay::PlayerLookingAtResolver player_looking_at_resolver;
}


TEST(Krampus_Gameplay_PlayerLookingAtResolverTest, run__returns_the_expected_response)
{
   Krampus::Gameplay::PlayerLookingAtResolver player_looking_at_resolver;
   std::string expected_string = "Hello World!";
   EXPECT_EQ(expected_string, player_looking_at_resolver.run());
}


