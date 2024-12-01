
#include <gtest/gtest.h>

//#include <TileFPS/Game/BlenderLevelLoaderEntity.hpp>
#include <Krampus24/BlenderBlockingLoaderEntity.hpp>


TEST(Krampus24_BlenderBlockingLoaderEntity, can_be_created_without_blowing_up)
{
   Krampus24::BlenderBlockingLoaderEntity blender_level_loader_entity;
}


TEST(Krampus24_BlenderBlockingLoaderEntity, get_name_unversioned__will_remove_the_trailing_dash_and_version_number)
{
   std::vector<std::pair<std::string, std::string>> test_data =
   {
      { "sun_dial-01", "sun_dial" },
      { "fishing_pond-04", "fishing_pond" },
      { "vertical_slate-06", "vertical_slate" },
      { "vertical_slate-06.001", "vertical_slate" },
      { "vertical_slate-06.002", "vertical_slate" },
   };

   for (auto& test_datum : test_data)
   {
      Krampus24::BlenderBlockingLoaderEntity entity;
      entity.name = test_datum.first;

      std::string expected_name_unversioned = test_datum.second;
      std::string actual_name_unversioned = entity.get_name_unversioned();

      EXPECT_EQ(expected_name_unversioned, actual_name_unversioned);
   }
}


