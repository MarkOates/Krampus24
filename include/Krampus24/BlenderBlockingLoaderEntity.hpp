#pragma once


#include <AllegroFlare/Vec3D.hpp>
#include <string>


namespace Krampus24
{
   class BlenderBlockingLoaderEntity
   {
   private:

   protected:


   public:
      std::string name;
      AllegroFlare::Vec3D location;
      AllegroFlare::Vec3D rotation;
      AllegroFlare::Vec3D scale;
      BlenderBlockingLoaderEntity();
      ~BlenderBlockingLoaderEntity();

      std::string get_name_unversioned();
      static std::string remove_dot_instance(std::string string="[unset-string]");
   };
}



