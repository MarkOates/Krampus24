

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


   result->affected_by_environmental_forces = true;


   //Krampus24::Gameplay::Entities::Base* result = new Krampus24::Gameplay::Entities::Base();

   float x = 0; //entity->location.x;
   float y = 0.5; //entity->location.z; // Swapping z<->y
   float z = 0; //entity->location.y; // Swapping z<->y

   AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

   result->placement.position = position;
   result->placement.size = { 0.5, 0.5, 0.5 };
   result->collides_with_player = true;
   //result->affected_by_environmental_forces = affected_by_environmental_forces;

   //result->name = entity->name;
   //std::string entity_root_name = entity->get_name_unversioned();
   //if (entity_root_name == "elevator")
   //{
      // Do elevator stuff
      //result->placement.size = { 1.0, 2.0, 1.0 };
      //result->box_color = ALLEGRO_COLOR{ 1.0, 1.0, 0.4, 1.0 };
   //}


   return result;
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


