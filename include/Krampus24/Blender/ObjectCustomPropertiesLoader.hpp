#pragma once


#include <Krampus24/Blender/ObjectCustomProperties.hpp>
#include <functional>
#include <map>
#include <string>


namespace Krampus24
{
   namespace Blender
   {
      class ObjectCustomPropertiesLoader
      {
      private:
         std::string filename;
         std::map<std::string, Krampus24::Blender::ObjectCustomProperties> objects;
         bool loaded;

      protected:


      public:
         ObjectCustomPropertiesLoader(std::string filename="tests/fixtures/maps/blender_export_with_entities.blocking2");
         ~ObjectCustomPropertiesLoader();

         void set_filename(std::string filename);
         std::map<std::string, Krampus24::Blender::ObjectCustomProperties> get_objects() const;
         bool get_loaded() const;
         void for_each_object(std::function<void(std::string, Krampus24::Blender::ObjectCustomProperties*)> function={});
         void load();
      };
   }
}



