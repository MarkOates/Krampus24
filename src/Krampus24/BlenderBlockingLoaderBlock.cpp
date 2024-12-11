

#include <Krampus24/BlenderBlockingLoaderBlock.hpp>

#include <algorithm>
#include <cctype>


namespace Krampus24
{


BlenderBlockingLoaderBlock::BlenderBlockingLoaderBlock()
   : name("[unset-name]")
   , min_coordinates({})
   , max_coordinates({})
   , scale({})
   , tile_coordinates({})
   , height(0.0f)
{
}


BlenderBlockingLoaderBlock::~BlenderBlockingLoaderBlock()
{
}


AllegroFlare::Vec3D BlenderBlockingLoaderBlock::calculate_size()
{
   return max_coordinates - min_coordinates;
}

std::string BlenderBlockingLoaderBlock::get_name_unversioned()
{
   std::string result = remove_dot_instance(name);

   size_t pos = result.find_last_of('-');
   if (pos != std::string::npos && pos < result.length() - 1)
   {
      std::string version = result.substr(pos + 1);
      bool is_number = !version.empty() &&
                       std::all_of(version.begin(), version.end(), [](char c)
                       {
                          return std::isdigit(static_cast<unsigned char>(c));
                       });
      if (is_number)
      {
         return result.substr(0, pos);
      }
   }
   return result;
}

std::string BlenderBlockingLoaderBlock::remove_dot_instance(std::string string)
{
   size_t pos = string.find_last_of('.');
   if (pos != std::string::npos && pos < string.length() - 1)
   {
      std::string version = string.substr(pos + 1);
      bool isNumber = !version.empty() && std::all_of(version.begin(), version.end(), ::isdigit);
      if (isNumber)
      {
         return string.substr(0, pos);
      }
   }
   return string;
}


} // namespace Krampus24


