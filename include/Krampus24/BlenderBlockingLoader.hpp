#pragma once


#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <Krampus24/BlenderBlockingLoaderBlock.hpp>
#include <Krampus24/BlenderBlockingLoaderEntity.hpp>
#include <string>
#include <utility>
#include <vector>


namespace Krampus24
{
   class BlenderBlockingLoader
   {
   private:
      std::string filename;
      std::vector<Krampus24::BlenderBlockingLoaderBlock> blocks;
      std::vector<Krampus24::BlenderBlockingLoaderEntity> entities;
      bool loaded;
      void build_blocks();

   protected:


   public:
      BlenderBlockingLoader(std::string filename="tests/fixtures/maps/blender_export_with_entities.blocking2");
      ~BlenderBlockingLoader();

      void set_filename(std::string filename);
      std::vector<Krampus24::BlenderBlockingLoaderBlock> get_blocks() const;
      std::vector<Krampus24::BlenderBlockingLoaderEntity> get_entities() const;
      bool get_loaded() const;
      AllegroFlare::TileMaps::TileMap<float> assemble_height_map(float floor_height=-10.0);
      void load();
      static std::vector<std::pair<int, int>> tiles_within(float x=0.0f, float y=0.0f, float width=1.0f, float height=1.0f, int tile_width=16, int tile_height=16);
   };
}



