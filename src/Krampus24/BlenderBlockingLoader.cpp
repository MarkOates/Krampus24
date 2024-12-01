

#include <Krampus24/BlenderBlockingLoader.hpp>

#include <AllegroFlare/Int2D.hpp>
#include <AllegroFlare/Logger.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{


BlenderBlockingLoader::BlenderBlockingLoader(std::string filename)
   : filename(filename)
   , blocks({})
   , entities({})
   , loaded(false)
{
}


BlenderBlockingLoader::~BlenderBlockingLoader()
{
}


void BlenderBlockingLoader::set_filename(std::string filename)
{
   this->filename = filename;
}


std::vector<Krampus24::BlenderBlockingLoaderBlock> BlenderBlockingLoader::get_blocks() const
{
   return blocks;
}


std::vector<Krampus24::BlenderBlockingLoaderEntity> BlenderBlockingLoader::get_entities() const
{
   return entities;
}


bool BlenderBlockingLoader::get_loaded() const
{
   return loaded;
}


AllegroFlare::TileMaps::TileMap<float> BlenderBlockingLoader::assemble_height_map(float floor_height)
{
   if (!(loaded))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::BlenderBlockingLoader::assemble_height_map]: error: guard \"loaded\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::BlenderBlockingLoader::assemble_height_map]: error: guard \"loaded\" not met");
   }
   AllegroFlare::TileMaps::TileMap<float> result;

   // Infer the dimensions of the height map
   AllegroFlare::Int2D min_tile_coordinate = { 9999, 9999 };
   AllegroFlare::Int2D max_tile_coordinate = { -9999, -9999 }; 
   for (auto &block : blocks)
   {
      for (auto &tile_coordinate : block.tile_coordinates)
      {
         if (tile_coordinate.first > max_tile_coordinate.x) max_tile_coordinate.x = tile_coordinate.first;
         if (tile_coordinate.second > max_tile_coordinate.y) max_tile_coordinate.y = tile_coordinate.second;
         if (tile_coordinate.first < min_tile_coordinate.x) min_tile_coordinate.x = tile_coordinate.first;
         if (tile_coordinate.second < min_tile_coordinate.y) min_tile_coordinate.y = tile_coordinate.second;
      }
   }

   // Throw if there are any tile coordinates in negative numbers
   if (min_tile_coordinate.x < 0
      || min_tile_coordinate.y < 0
      || max_tile_coordinate.x < 0
      || min_tile_coordinate.y < 0)
   {
      AllegroFlare::Logger::throw_error(
         "TileFPS::Game::BlenderLevelLoaderV2::assemble_height_map",
         "There are tile coordinates in the blocks that are negative numbers. It's expected all tiles coordinates "
            "to be positive."
      );
   }

   // Resize the map, filling with the floor_height
   result.initialize();
   result.resize_with_fill(max_tile_coordinate.x+1, max_tile_coordinate.y+1, floor_height);

   // Iterate through all the tiles provided by the blocks, and set them
   for (auto &block : blocks)
   {
      for (auto &tile_coordinate : block.tile_coordinates)
      {
         // If blocks overlap, use the highest height
         // TODO: Test this
         float existing_tile_height = result.get_tile(tile_coordinate.first, tile_coordinate.second);
         if (block.height > existing_tile_height)
         {
            result.set_tile(tile_coordinate.first, tile_coordinate.second, block.height);
         }
      }
   }

   return result;
}

void BlenderBlockingLoader::load()
{
   if (!((!loaded)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::BlenderBlockingLoader::load]: error: guard \"(!loaded)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::BlenderBlockingLoader::load]: error: guard \"(!loaded)\" not met");
   }
   std::ifstream file(filename);
   if (!file)
   {
       AllegroFlare::Logger::throw_error(
         "TileFPS::Game::BlenderLevelLoaderV2",
         "Unable to open file \"" + filename + "\"."
       );
   }

   std::string line;

   while (std::getline(file, line))
   {
      std::istringstream iss(line);
      std::string type;
      std::string name;
      float x1, y1, z1, x2, y2, z2, sx, sy, sz;
      char comma;

      // Read and parse the line
      if (iss >> type >> name >> x1 >> y1 >> z1 >> comma >> x2 >> y2 >> z2 >> comma >> sx >> sy >> sz)
      {
         //std::cout << "type: " << type << std::endl;
         type = type.substr(0, std::max((int)type.length()-1, 0));
         if (type == "entity")
         {
            entities.push_back({});
            Krampus24::BlenderBlockingLoaderEntity &entity = entities.back();
            //TileFPS::Game::BlenderLevelLoaderEntity &entity = entities.back();

            // Flip the y coordinate (the coordinates in Blender have -y going into the viewer)
            //y1 = -y1;

            // Load in the min and max values for the coordinates in this block
            entity.name = name.substr(0, std::max((int)name.length()-1, 0));
            entity.location = { x1, y1, z1 };
            entity.rotation = { x2, y2, z2 };
            entity.scale = { sx, sy, sz };
         }
         else if (type == "block")
         {
            blocks.push_back({});
            Krampus24::BlenderBlockingLoaderBlock &block = blocks.back();
            //TileFPS::Game::BlenderLevelLoaderBlock &block = blocks.back();

            // Flip the y coordinate (the coordinates in Blender have -y going into the viewer)
            //y1 = -y1;
            //y2 = -y2;

            // Load in the min and max values for the coordinates in this block
            block.name = name.substr(0, std::max((int)name.length()-1, 0));
            block.min_coordinates = { std::min(x1, x2), std::min(y1, y2), std::min(z1, z2) };
            block.max_coordinates = { std::max(x1, x2), std::max(y1, y2), std::max(z1, z2) };
            block.scale = { sx, sy, sz };
            block.height = block.max_coordinates.z;
         }
      }
      else
      {
         AllegroFlare::Logger::throw_error(
           "TileFPS::Game::BlenderLevelLoaderV2",
           "Error parsing line: \"" + line + "\"."
         );
      }
   }

   file.close();

   build_blocks();

   loaded = true;
}

void BlenderBlockingLoader::build_blocks()
{
   for (auto &block : blocks)
   {
      block.tile_coordinates = tiles_within(
         block.min_coordinates.x,
         block.min_coordinates.y,
         block.max_coordinates.x - block.min_coordinates.x,
         block.max_coordinates.y - block.min_coordinates.y,
         1,
         1
      );
   }
   return;
}

std::vector<std::pair<int, int>> BlenderBlockingLoader::tiles_within(float x, float y, float width, float height, int tile_width, int tile_height)
{
   // TODO: Review the comments below, see if it still applies
   // NOTE: This algo will produce tiles starting at the top left, and stepping in increments of 16 (or whatever
   // tile_width and tile_height is). This will mean that tiles at the right and bottom edges may not be included,
   // depending on the dimensions of the box. Something to keep an eye on, or, make adjustments to this method
   // and provide alternative fill options.

   std::vector<std::pair<int, int>> result;

   // Calculate the tile coordinates within the rectangle
   for (int j = y; j < y + height; j += tile_height)
   {
       for (int i = x; i < x + width; i += tile_width)
       {
           result.push_back(std::make_pair(i / tile_width, j / tile_height));
       }
   }

   return result;
}


} // namespace Krampus24


