

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


Tree::Tree(std::vector<Krampus24::Gameplay::Entities::Base*>* entities)
   : entities(entities)
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

void* Tree::find_entity_by_name_or_throw(std::string name)
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

void Tree::build_on_collision_callbacks()
{
   on_entity_collision_callbacks = {
      { find_entity_by_name_or_throw("elevator1"), [](){
         // TODO: Do thing
         std::cout << "Entered elevator1" << std::endl;
      }},
      { find_entity_by_name_or_throw("elevator2"), [](){
         // TODO: Do thing
         std::cout << "Entered elevator2" << std::endl;
      }},
   };
   return;
}


} // namespace Scripting
} // namespace Game
} // namespace Krampus24


