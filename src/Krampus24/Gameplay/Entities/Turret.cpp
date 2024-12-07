

#include <Krampus24/Gameplay/Entities/Turret.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Turret::Turret()
   : Krampus24::Gameplay::Entities::Base()
   , initialized(false)
{
}


Turret::~Turret()
{
}


bool Turret::get_initialized() const
{
   return initialized;
}


void Turret::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::initialize]: error: guard \"(!initialized)\" not met");
   }
   initialized = true;
   return;
}

Krampus24::Gameplay::Entities::Turret* Turret::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::Vec3D position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"bitmap_bin\" not met");
   }
   // TODO: Sort out the alignment, size, of objects, their collision boxes, their repositioning and the relationship
   // to the bounding box, as well as the collision padding of the player entity
   //float width = 2;
   //float height = 2;
   AllegroFlare::Vec3D box_start_position = position; // + AllegroFlare::Vec3D(1, 0, 0);
   AllegroFlare::Vec3D box_end_position = position; // + AllegroFlare::Vec3D(5, 0, 0);

   // Make the manager
   auto manager = new Krampus24::Gameplay::Entities::Turret;
   manager->model = model_bin->auto_get("turret-03.obj");
   manager->texture = bitmap_bin->auto_get("turret-03.png");
   manager->placement.position = position;
   manager->placement.rotation.y = rotation;
   manager->placement.size = { 0.0, 0.0, 0.0 };
   manager->placement.align = { 0.5, 0.0, 0.5 };
   manager->placement.scale = { 1.0, 1.0, 1.0 };

   // Make inspectable
   manager->player_can_inspect_or_use = true;
   manager->aabb3d.set_max({ 5.0, 5.0, 5.0 });
   manager->aabb3d_alignment = { 0.5, 0.0, 0.5 };

   manager->initialize();

   // DEVELOPMENT: For now, just going to make an interactable zone to trigger the action on this entity
   //manager->set_hit_box_2d(AllegroFlare::Physics::AABB2D(0, 0, 20, 20));
   //manager->set(AllegroFlare::Prototypes::TileFPS::EntityFlags::PLAYER_CAN_INSPECT_OR_USE);
   //manager->set(AllegroFlare::Prototypes::TileFPS::EntityFlags::COLLIDES_WITH_PLAYER);

   return manager;
}

bool Turret::on_player_inspect_or_use()
{
   // TODO: Consider some interaction here
   return true;
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


