
#include <gtest/gtest.h>

#include <Krampus24/BlenderCustomPropertiesLoader.hpp>


TEST(Krampus24_Blender_CustomPropertiesLoaderTest, can_be_created_without_blowing_up)
{
   Krampus24::BlenderCustomPropertiesLoader loader;
}


TEST(Krampus24_Blender_CustomPropertiesLoaderTest, load__will_mark_loaded_as_true)
{
   std::string filename = "tests/fixtures/maps/output_file-02.custom_properties";
   Krampus24::BlenderCustomPropertiesLoader loader(filename);
   loader.load();
   EXPECT_EQ(true, loader.get_loaded());
}


TEST(Krampus24_Blender_CustomPropertiesLoaderTest, load__will_load_the_expected_custom_properties)
{
   std::string filename = "tests/fixtures/maps/output_file-02.custom_properties";
   Krampus24::BlenderCustomPropertiesLoader loader(filename);
   loader.load();

   auto objects = loader.get_objects();
   std::string expected_object_name = "door.008";
   ASSERT_EQ(1, objects.size());
   ASSERT_NE(objects.end(), objects.find(expected_object_name));

   std::string expected_keys = "[\"door_texture_style\" (int), ]";
   std::string actual_keys = objects[expected_object_name].get_keys_in_list();
   EXPECT_EQ(expected_keys, actual_keys);
}


