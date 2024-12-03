

#include <Krampus24/Gameplay/Entities/Hen.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Hen::Hen()
   : Krampus24::Gameplay::Entities::Base()
{
}


Hen::~Hen()
{
}


Krampus24::Gameplay::Entities::Hen* Hen::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Hen::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Hen::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Hen::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Hen::construct]: error: guard \"bitmap_bin\" not met");
   }
   Krampus24::Gameplay::Entities::Hen* result = new Krampus24::Gameplay::Entities::Hen;
   result->model = model_bin->auto_get("hen-01.obj");
   result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   return result;
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


