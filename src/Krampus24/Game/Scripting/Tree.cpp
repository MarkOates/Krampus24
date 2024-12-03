

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
   : Krampus24::Gameplay::ScriptingInterface()
   , font_bin(nullptr)
   , entities(nullptr)
   , primary_power_coil_collected(false)
   , primary_power_coil_returned_to_ship(false)
   , collision_observer(nullptr)
   , initialized(false)
{
}


Tree::~Tree()
{
}


void Tree::set_font_bin(AllegroFlare::FontBin* font_bin)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_font_bin]: error: guard \"get_initialized()\" not met.");
   this->font_bin = font_bin;
}


void Tree::set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_entities]: error: guard \"get_initialized()\" not met.");
   this->entities = entities;
}


void Tree::set_primary_power_coil_collected(bool primary_power_coil_collected)
{
   this->primary_power_coil_collected = primary_power_coil_collected;
}


void Tree::set_primary_power_coil_returned_to_ship(bool primary_power_coil_returned_to_ship)
{
   this->primary_power_coil_returned_to_ship = primary_power_coil_returned_to_ship;
}


void Tree::set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_collision_observer]: error: guard \"get_initialized()\" not met.");
   this->collision_observer = collision_observer;
}


bool Tree::get_primary_power_coil_collected() const
{
   return primary_power_coil_collected;
}


bool Tree::get_primary_power_coil_returned_to_ship() const
{
   return primary_power_coil_returned_to_ship;
}


bool Tree::get_initialized() const
{
   return initialized;
}


void Tree::render_hud()
{
   if (primary_power_coil_collected)
   {
      al_draw_textf(
         obtain_hud_font(),
         ALLEGRO_COLOR{1, 0.65, 0, 1.0},
         40,
         30,
         ALLEGRO_ALIGN_LEFT,
         "PRIMARY POWER COIL COLLECTED"
      );
   }
   return;
}

bool Tree::end_state_achieved()
{
   return primary_power_coil_returned_to_ship;
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
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"font_bin\" not met");
   }
   initialized = true;
   build_on_collision_callbacks();
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

   player_entity->placement.position =
      target_elevator->placement.position + AllegroFlare::Vec3D(0, 0.5, 0);
   collision_observer->passively_add_to_currently_colliding(target_elevator);
   return;
}

void Tree::build_on_collision_callbacks()
{
   link_elevators("elevator-01", "elevator-02");
   link_elevators("elevator-03", "elevator-04");
   link_elevators("elevator-05", "elevator-06");
   link_elevators("elevator-07", "elevator-08");
   link_elevators("elevator-09", "elevator-10");

   on_entity_collision_callbacks = {

      //{ find_entity_by_name_or_throw("hen-01"), [this](){
         //travel_player_to_elevators_target("elevator-01");
      //}},

      { find_entity_by_name_or_throw("elevator-01"), [this](){
         travel_player_to_elevators_target("elevator-01");
      }},
      { find_entity_by_name_or_throw("elevator-02"), [this](){
         travel_player_to_elevators_target("elevator-02");
      }},
      { find_entity_by_name_or_throw("elevator-03"), [this](){
         travel_player_to_elevators_target("elevator-03");
      }},
      { find_entity_by_name_or_throw("elevator-04"), [this](){
         travel_player_to_elevators_target("elevator-04");
      }},
      { find_entity_by_name_or_throw("elevator-05"), [this](){
         travel_player_to_elevators_target("elevator-05");
      }},
      { find_entity_by_name_or_throw("elevator-06"), [this](){
         travel_player_to_elevators_target("elevator-06");
      }},
      { find_entity_by_name_or_throw("elevator-07"), [this](){
         travel_player_to_elevators_target("elevator-07");
      }},
      { find_entity_by_name_or_throw("elevator-08"), [this](){
         travel_player_to_elevators_target("elevator-08");
      }},
      { find_entity_by_name_or_throw("elevator-09"), [this](){
         travel_player_to_elevators_target("elevator-09");
      }},
      { find_entity_by_name_or_throw("elevator-10"), [this](){
         travel_player_to_elevators_target("elevator-10");
      }},

      { find_entity_by_name_or_throw("player_ship"), [this](){
         //travel_player_to_elevators_target("elevator-10");
         if (primary_power_coil_collected)
         {
            primary_power_coil_returned_to_ship = true;
         }
      }},
      { find_entity_by_name_or_throw("primary_power_coil"), [this](){
         find_entity_by_name_or_throw("primary_power_coil")->active = false;
         find_entity_by_name_or_throw("primary_power_coil")->visible = false;
         primary_power_coil_collected = true;
      }},
   };
   return;
}

ALLEGRO_FONT* Tree::obtain_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -52");
}


} // namespace Scripting
} // namespace Game
} // namespace Krampus24


