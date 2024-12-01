

#include <Krampus24/BlenderBlockingLoaderEntity.hpp>

#include <algorithm>
#include <cctype>


namespace Krampus24
{


BlenderBlockingLoaderEntity::BlenderBlockingLoaderEntity()
   : name("[unset-name]")
   , location({})
   , rotation({})
   , scale({})
{
}


BlenderBlockingLoaderEntity::~BlenderBlockingLoaderEntity()
{
}


std::string BlenderBlockingLoaderEntity::get_name_unversioned()
{
   std::string result = remove_dot_instance(name);

   size_t pos = result.find_last_of('-');
   if (pos != std::string::npos && pos < result.length() - 1)
   {
      std::string version = result.substr(pos + 1);
      bool isNumber = !version.empty() && std::all_of(version.begin(), version.end(), std::isdigit);
      if (isNumber)
      {
         return result.substr(0, pos);
      }
   }
   return result;
}

std::string BlenderBlockingLoaderEntity::remove_dot_instance(std::string string)
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


