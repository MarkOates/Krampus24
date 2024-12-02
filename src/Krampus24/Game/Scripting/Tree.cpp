

#include <Krampus24/Game/Scripting/Tree.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Game
{
namespace Scripting
{


Tree::Tree()
   : entities(nullptr)
   , collision_observer(nullptr)
   , on_entity_collision_callbacks({})
   , initialized(false)
{
}


Tree::~Tree()
{
}


void Tree::set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_entities]: error: guard \"get_initialized()\" not met.");
   this->entities = entities;
}


void Tree::set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_collision_observer]: error: guard \"get_initialized()\" not met.");
   this->collision_observer = collision_observer;
}


void Tree::set_on_entity_collision_callbacks(std::map<void*, std::function<void()>> on_entity_collision_callbacks)
{
   this->on_entity_collision_callbacks = on_entity_collision_callbacks;
}


std::map<void*, std::function<void()>> Tree::get_on_entity_collision_callbacks() const
{
   return on_entity_collision_callbacks;
}


bool Tree::get_initialized() const
{
   return initialized;
}


bool Tree::a_0th_entity_exists()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::a_0th_entity_exists]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::a_0th_entity_exists]: error: guard \"initialized\" not met");
   }
   return (entities->size() > 0);
}

Krampus24::Gameplay::Entities::Base* Tree::find_0th_entity()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::find_0th_entity]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::find_0th_entity]: error: guard \"initialized\" not met");
   }
   if (!((entities->size() > 0)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::find_0th_entity]: error: guard \"(entities->size() > 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::find_0th_entity]: error: guard \"(entities->size() > 0)\" not met");
   }
   return entities->at(0);
}

void Tree::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"(!initialized)\" not met");
   }
   if (!(entities))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"entities\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"entities\" not met");
   }
   if (!(collision_observer))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"collision_observer\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"collision_observer\" not met");
   }
   initialized = true;
   return;
}

bool Tree::has_on_collision_callback(void* entity)
{
   return on_entity_collision_callbacks.find(entity) != on_entity_collision_callbacks.end();
}

void Tree::call_on_collision_callback(void* entity)
{
   if (!(has_on_collision_callback(entity)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::call_on_collision_callback]: error: guard \"has_on_collision_callback(entity)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::call_on_collision_callback]: error: guard \"has_on_collision_callback(entity)\" not met");
   }
   on_entity_collision_callbacks[entity]();
   return;
}

bool Tree::entity_with_name_exists(std::string name)
{
   if (!(entities))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::entity_with_name_exists]: error: guard \"entities\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::entity_with_name_exists]: error: guard \"entities\" not met");
   }
   for (auto entity : *entities)
   {
      if (entity->name == name) return true;
   }
   return false;
}

Krampus24::Gameplay::Entities::Base* Tree::find_entity_by_name_or_throw(std::string name)
{
   if (!(entity_with_name_exists(name)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::find_entity_by_name_or_throw]: error: guard \"entity_with_name_exists(name)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::find_entity_by_name_or_throw]: error: guard \"entity_with_name_exists(name)\" not met");
   }
   // TODO: Improve error message on entity_with_name_exists(name)
   for (auto entity : *entities)
   {
      if (entity->name == name) return entity;
   }
   // TODO: Improve throw
   throw std::runtime_error("asdfasdfasdfasdfasdfasdf");
   return nullptr;
}

void Tree::link_elevators(std::string elevator_a_name, std::string elevator_b_name)
{
   Krampus24::Gameplay::Entities::Base* elevator_a = find_entity_by_name_or_throw(elevator_a_name);
   Krampus24::Gameplay::Entities::Base* elevator_b = find_entity_by_name_or_throw(elevator_b_name);
   elevator_a->elevator__target = elevator_b;
   elevator_b->elevator__target = elevator_a;
   return;
}

void Tree::travel_player_to_elevators_target(std::string entering_elevator_name)
{
   auto *player_entity = find_0th_entity();
   auto *target_elevator = find_entity_by_name_or_throw(entering_elevator_name)->elevator__target;

   player_entity->get_placement_ref().position =
      target_elevator->get_placement_ref().position + AllegroFlare::Vec3D(0, 0.5, 0);
   collision_observer->passively_add_to_currently_colliding(target_elevator);
   return;
}

void Tree::build_on_collision_callbacks()
{
   link_elevators("elevator1", "elevator2");
   link_elevators("elevator3", "elevator4");
   link_elevators("elevator5", "elevator9");
   link_elevators("elevator7", "elevator8");
   link_elevators("elevator9", "elevator10");

   on_entity_collision_callbacks = {
      { find_entity_by_name_or_throw("elevator1"), [this](){
         travel_player_to_elevators_target("elevator1");
      }},
      { find_entity_by_name_or_throw("elevator2"), [this](){
         travel_player_to_elevators_target("elevator2");
      }},
      { find_entity_by_name_or_throw("elevator3"), [this](){
         travel_player_to_elevators_target("elevator3");
      }},
      { find_entity_by_name_or_throw("elevator4"), [this](){
         travel_player_to_elevators_target("elevator4");
      }},
      { find_entity_by_name_or_throw("elevator5"), [this](){
         travel_player_to_elevators_target("elevator5");
      }},
      { find_entity_by_name_or_throw("elevator6"), [this](){
         travel_player_to_elevators_target("elevator6");
      }},
      { find_entity_by_name_or_throw("elevator7"), [this](){
         travel_player_to_elevators_target("elevator7");
      }},
      { find_entity_by_name_or_throw("elevator8"), [this](){
         travel_player_to_elevators_target("elevator8");
      }},
      { find_entity_by_name_or_throw("elevator9"), [this](){
         travel_player_to_elevators_target("elevator9");
      }},
      { find_entity_by_name_or_throw("elevator10"), [this](){
         travel_player_to_elevators_target("elevator10");
      }},
   };
   return;
}


} // namespace Scripting
} // namespace Game
} // namespace Krampus24


