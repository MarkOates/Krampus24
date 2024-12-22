

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
      bool isNumber = !version.empty()
                    // NOTE: This noisy C++ing due to inconsistencies between Mac and Win compiles
                    // See: https://chatgpt.com/c/67684279-c730-800f-be4e-0c2d369b2483
                    && std::all_of(version.begin(), version.end(), static_cast<int(*)(int)>(std::isdigit));
                    //std::all_of(version.begin(), version.end(), std::isdigit);
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
      bool isNumber = !version.empty()
                    // NOTE: This noisy C++ing due to inconsistencies between Mac and Win compiles
                    // See: https://chatgpt.com/c/67684279-c730-800f-be4e-0c2d369b2483
                    && std::all_of(version.begin(), version.end(), static_cast<int(*)(int)>(std::isdigit));
                    // std::all_of(version.begin(), version.end(), std::isdigit);
      if (isNumber)
      {
         return string.substr(0, pos);
      }
   }
   return string;
}


} // namespace Krampus24


