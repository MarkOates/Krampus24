

#include <Krampus24/Game/EntityFactory.hpp>

#include <Krampus24/Gameplay/Entities/Console.hpp>
#include <Krampus24/Gameplay/Entities/Cryobed.hpp>
#include <Krampus24/Gameplay/Entities/Door.hpp>
#include <Krampus24/Gameplay/Entities/ElevatorShaft.hpp>
#include <Krampus24/Gameplay/Entities/Hen.hpp>
#include <Krampus24/Gameplay/Entities/Horse.hpp>
#include <Krampus24/Gameplay/Entities/MegaDoor.hpp>
#include <Krampus24/Gameplay/Entities/Pig.hpp>
#include <Krampus24/Gameplay/Entities/PlayerShip.hpp>
#include <Krampus24/Gameplay/Entities/PowerCoil.hpp>
#include <Krampus24/Gameplay/Entities/SlidingDoor.hpp>
#include <Krampus24/Gameplay/Entities/Tablet.hpp>
#include <Krampus24/Gameplay/Entities/Turret.hpp>
#include <Krampus24/Gameplay/Entities/Zone.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Game
{


EntityFactory::EntityFactory()
   : event_emitter(nullptr)
   , model_bin(nullptr)
   , bitmap_bin(nullptr)
   , collision_mesh(nullptr)
   , initialized(false)
{
}


EntityFactory::~EntityFactory()
{
}


void EntityFactory::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (get_initialized()) throw std::runtime_error("[EntityFactory::set_event_emitter]: error: guard \"get_initialized()\" not met.");
   this->event_emitter = event_emitter;
}


void EntityFactory::set_model_bin(AllegroFlare::ModelBin* model_bin)
{
   if (get_initialized()) throw std::runtime_error("[EntityFactory::set_model_bin]: error: guard \"get_initialized()\" not met.");
   this->model_bin = model_bin;
}


void EntityFactory::set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin)
{
   if (get_initialized()) throw std::runtime_error("[EntityFactory::set_bitmap_bin]: error: guard \"get_initialized()\" not met.");
   this->bitmap_bin = bitmap_bin;
}


void EntityFactory::set_collision_mesh(AllegroFlare::Physics::CollisionMesh* collision_mesh)
{
   if (get_initialized()) throw std::runtime_error("[EntityFactory::set_collision_mesh]: error: guard \"get_initialized()\" not met.");
   this->collision_mesh = collision_mesh;
}


bool EntityFactory::get_initialized() const
{
   return initialized;
}


void EntityFactory::initialize()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::initialize]: error: guard \"event_emitter\" not met");
   }
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::initialize]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::initialize]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::initialize]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::initialize]: error: guard \"bitmap_bin\" not met");
   }
   if (!(collision_mesh))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::initialize]: error: guard \"collision_mesh\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::initialize]: error: guard \"collision_mesh\" not met");
   }
   initialized = true;
   return;
}

std::vector<Krampus24::Gameplay::Entities::Base*> EntityFactory::create_entity(Krampus24::BlenderBlockingLoaderEntity* entity)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::create_entity]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::create_entity]: error: guard \"initialized\" not met");
   }
   if (!(entity))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::EntityFactory::create_entity]: error: guard \"entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::EntityFactory::create_entity]: error: guard \"entity\" not met");
   }

   float x = entity->location.x;
   float y = entity->location.z; // Swapping z<->y
   float z = entity->location.y; // Swapping z<->y

   AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

   std::string entity_root_name = entity->get_name_unversioned();


   if (entity_root_name == Krampus24::Gameplay::Entities::Hen::BLENDER_IDENTIFIER)
   {
      //std::cout << "HEN made" << std::endl;
      auto *result = Krampus24::Gameplay::Entities::Hen::construct(model_bin, bitmap_bin, position, 6.0);
      result->name = entity->name;
      return { result };
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Horse::BLENDER_IDENTIFIER)
   {
      //std::cout << "HEN made" << std::endl;
      auto *result = Krampus24::Gameplay::Entities::Horse::construct(model_bin, bitmap_bin, position, 6.0);
      result->name = entity->name;
      return { result };
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Pig::BLENDER_IDENTIFIER)
   {
      //std::cout << "HEN made" << std::endl;
      auto *result = Krampus24::Gameplay::Entities::Pig::construct(model_bin, bitmap_bin, position, 6.0);
      result->name = entity->name;
      return { result };
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Turret::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      auto *result = Krampus24::Gameplay::Entities::Turret::construct(model_bin, bitmap_bin, position, rotation);
      result->name = entity->name;
      return { result };
   }
   // TODO: Move this to ZONES:
   //else if (entity_root_name == Krampus24::Gameplay::Entities::Zone::BLENDER_IDENTIFIER)
   //{
      ////float rotation = entity->rotation.z / 360.0;
      //AllegroFlare::Vec3D size = entity->size;
      //auto *result = Krampus24::Gameplay::Entities::Zone::construct(position, size);
      //result->name = entity->name;
      //return { result };
   //}
   else if (entity_root_name == Krampus24::Gameplay::Entities::Cryobed::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results = Krampus24::Gameplay::Entities::Cryobed::construct(
         model_bin,
         bitmap_bin,
         event_emitter,
         collision_mesh,
         entity->name,
         position,
         rotation
      );
      results[0]->name = entity->name;
      results[0]->set_type("Cryobed");
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Door::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results = Krampus24::Gameplay::Entities::Door::construct(
         model_bin,
         bitmap_bin,
         event_emitter,
         collision_mesh,
         entity->name,
         position,
         rotation
      );
      results[0]->name = entity->name;
      results[0]->set_type("Door");
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::MegaDoor::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results = Krampus24::Gameplay::Entities::MegaDoor::construct(
         model_bin,
         bitmap_bin,
         event_emitter,
         collision_mesh,
         entity->name,
         position,
         rotation
      );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::ElevatorShaft::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::ElevatorShaft::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            collision_mesh,
            entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::SlidingDoor::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::SlidingDoor::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            collision_mesh,
            entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Console::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::Console::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            collision_mesh,
            entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::PlayerShip::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::PlayerShip::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            collision_mesh,
            entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::PowerCoil::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::PowerCoil::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            collision_mesh,
            entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }
   else if (entity_root_name == Krampus24::Gameplay::Entities::Tablet::BLENDER_IDENTIFIER)
   {
      float rotation = entity->rotation.z / 360.0;
      std::vector<Krampus24::Gameplay::Entities::Base*> results =
         Krampus24::Gameplay::Entities::Tablet::construct(
            model_bin,
            bitmap_bin,
            event_emitter,
            //collision_mesh,
            //entity->name,
            position,
            rotation
         );
      results[0]->name = entity->name;
      return results;
   }

   //AllegroFlare::Vec3D position;
   bool affected_by_environmental_forces = true;


   Krampus24::Gameplay::Entities::Base* result = new Krampus24::Gameplay::Entities::Base();

   //float x = entity->location.x;
   //float y = entity->location.z; // Swapping z<->y
   //float z = entity->location.y; // Swapping z<->y

   //AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);

   result->placement.position = position;
   result->placement.size = { 0.5, 0.5, 0.5 };
   result->aabb3d.set_max(result->placement.size);
   result->collides_with_player = true;
   result->affected_by_environmental_forces = affected_by_environmental_forces;

   result->name = entity->name;
   //std::string entity_root_name = entity->get_name_unversioned();
   if (entity_root_name == "elevator")
   {
      // Do elevator stuff
      result->placement.size = { 4.0, 4.0, 4.0 };
      result->aabb3d.set_max(result->placement.size);
      result->aabb3d_alignment = { 0.5, 0.005, 0.5 };
      result->box_color = ALLEGRO_COLOR{ 1.0, 1.0, 0.4, 1.0 };
   }

   return { result };
}


} // namespace Game
} // namespace Krampus24


