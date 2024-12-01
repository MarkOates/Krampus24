
#include <gtest/gtest.h>

//#include <TileFPS/Game/BlenderLevelLoaderV2.hpp>
#include <Krampus24/BlenderBlockingLoader.hpp>


TEST(Krampus24_BlenderBlockingLoaderTest, can_be_created_without_blowing_up)
{
   Krampus24::BlenderBlockingLoader blender_level_loader;
}


TEST(Krampus24_BlenderBlockingLoaderTest, load__will_mark_loaded_as_true)
{
   std::string filename = "tests/fixtures/maps/blender_export_with_entities.blocking2";
   Krampus24::BlenderBlockingLoader blender_level_loader(filename);
   blender_level_loader.load();
   EXPECT_EQ(true, blender_level_loader.get_loaded());
}


TEST(Krampus24_BlenderBlockingLoaderTest, load__will_populate_the_blocks_with_the_expected_data)
{
   std::string filename = "tests/fixtures/maps/blender_export_with_entities.blocking2";
   Krampus24::BlenderBlockingLoader blender_level_loader(filename);
   blender_level_loader.load();
   std::vector<Krampus24::BlenderBlockingLoaderBlock> blocks = blender_level_loader.get_blocks();
   std::vector<Krampus24::BlenderBlockingLoaderEntity> entities = blender_level_loader.get_entities();
   ASSERT_EQ(4, blocks.size());
   ASSERT_EQ(1, entities.size());

   {
      Krampus24::BlenderBlockingLoaderBlock &block = blocks[0];
      std::vector<std::pair<int, int>> expected_tile_coordinates = { { 0, 5 } };
      EXPECT_EQ(expected_tile_coordinates, block.tile_coordinates);
      EXPECT_EQ("Cube", block.name);
      EXPECT_EQ(1.0, block.height);
   }
   {
      Krampus24::BlenderBlockingLoaderBlock &block = blocks[1];
      std::vector<std::pair<int, int>> expected_tile_coordinates = {
         { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 },
      };
      EXPECT_EQ(expected_tile_coordinates, block.tile_coordinates);
      EXPECT_EQ("Cube.001", block.name);
      EXPECT_EQ(2.0f, block.height);
   }
   {
      Krampus24::BlenderBlockingLoaderBlock &block = blocks[2];
      //std::vector<std::pair<int, int>> expected_tile_coordinates = {
         //{ 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 },
      //};
      EXPECT_EQ(30, block.tile_coordinates.size());
      EXPECT_EQ("Cube.002", block.name);
      EXPECT_EQ(0.25f, block.height);
   }
   {
      Krampus24::BlenderBlockingLoaderBlock &block = blocks[3];
      std::vector<std::pair<int, int>> expected_tile_coordinates = {
         { 10, 0 }, { 11, 0 }, { 12, 0 }, { 13, 0 }, { 14, 0 }, { 15, 0 },
      };
      EXPECT_EQ(expected_tile_coordinates, block.tile_coordinates);
      EXPECT_EQ("Cube.003", block.name);
      EXPECT_EQ(0.825f, block.height);
   }
   {
      //entity, sun_plate-03, 11.000000 -6.000000 0.000000, 1.570796 -0.000000 0.000000, 1.000000 1.000000 1.000000
      Krampus24::BlenderBlockingLoaderEntity &entity = entities[0];
      //std::vector<std::pair<int, int>> expected_tile_coordinates = {
         //{ 10, 0 }, { 11, 0 }, { 12, 0 }, { 13, 0 }, { 14, 0 }, { 15, 0 },
      //};
      EXPECT_EQ("sun_plate-03", entity.name);
      EXPECT_EQ(AllegroFlare::Vec3D(11.0f, 6.0f, 0.0f), entity.location);
      EXPECT_EQ(AllegroFlare::Vec3D(1.570796f, 0.0f, 0.0f), entity.rotation); // NOTE: These values are temporary until
                                                                              // rotation is figured out.
      EXPECT_EQ(AllegroFlare::Vec3D(1.0f, 1.0f, 1.0f), entity.scale);
   }
}


/*
TEST(Krampus24_BlenderBlockingLoaderTest, assemble_height_map__will_generate_a_height_map_with_the_expected_data)
{
   std::string filename = "tests/fixtures/maps/blender_export_with_entities.blocking2";
   TileFPS::Game::BlenderLevelLoaderV2 blender_level_loadertype(filename);
   blender_level_loader.load();
   AllegroFlare::TileMaps::TileMap<float> height_map = blender_level_loader.assemble_height_map();

   float floor_height = -10.0f;
   EXPECT_EQ(16, height_map.get_num_columns());
   EXPECT_EQ(10, height_map.get_num_rows());

   std::vector<std::pair<std::pair<int, int>, float>> some_expected_tiles = {
      // First long bar across the top
      { { 0, 0 }, 2.0 },
      { { 5, 0 }, 2.0 },
      { { 6, 0 }, floor_height },

      // Little block on the bottom left
      { { 0, 5 }, 1.0 },

      // Second long bar along the top
      { { 10, 0 }, 0.825f },
      { { 15, 0 }, 0.825f },
      { { 15, 1 }, floor_height },

      // The big flat block in the middle
      { { 4, 4 }, floor_height },
      { { 4, 5 }, 0.25f },
      { { 9, 5 }, 0.25f },
      { { 4, 9 }, 0.25f },
      { { 9, 9 }, 0.25f },
      { { 10, 9 }, floor_height },

      // Edge tiles
      { { 15, 9 }, floor_height },
   };
   for (auto &tile : some_expected_tiles)
   {
      EXPECT_EQ(tile.second, height_map.get_tile(tile.first.first, tile.first.second))
                                                        << "Tile at (" << tile.first.first
                                                        << ", " << tile.first.second
                                                        << ")";
   }
}
*/


