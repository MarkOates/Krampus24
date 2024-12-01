#pragma once


#include <AllegroFlare/Vec3D.hpp>
#include <string>
#include <utility>
#include <vector>


namespace Krampus24
{
   class BlenderBlockingLoaderBlock
   {
   private:

   protected:


   public:
      std::string name;
      AllegroFlare::Vec3D min_coordinates;
      AllegroFlare::Vec3D max_coordinates;
      AllegroFlare::Vec3D scale;
      std::vector<std::pair<int, int>> tile_coordinates;
      float height;
      BlenderBlockingLoaderBlock();
      ~BlenderBlockingLoaderBlock();

   };
}



