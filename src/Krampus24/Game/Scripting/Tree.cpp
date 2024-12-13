

#include <Krampus24/Game/Scripting/Tree.hpp>

#include <AllegroFlare/DialogTree/NodeOptions/ExitDialog.hpp>
#include <AllegroFlare/DialogTree/NodeOptions/GoToNode.hpp>
#include <AllegroFlare/DialogTree/Nodes/MultipageWithOptions.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/StringTransformer.hpp>
#include <Krampus24/Gameplay/Entities/ElevatorShaft.hpp>
#include <Krampus24/Gameplay/Entities/MegaDoor.hpp>
#include <Krampus24/Gameplay/Entities/SlidingDoor.hpp>
#include <iostream>
#include <set>
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
   , event_emitter(nullptr)
   , dialog_system(nullptr)
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


void Tree::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_event_emitter]: error: guard \"get_initialized()\" not met.");
   this->event_emitter = event_emitter;
}


void Tree::set_dialog_system(AllegroFlare::DialogSystem::DialogSystem* dialog_system)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_dialog_system]: error: guard \"get_initialized()\" not met.");
   this->dialog_system = dialog_system;
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


void Tree::game_event_func(AllegroFlare::GameEvent* game_event)
{
   if (!(game_event))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::game_event_func]: error: guard \"game_event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::game_event_func]: error: guard \"game_event\" not met");
   }
   // TODO: Handle different events
   if (game_event->is_type("unlock_mega_door")) unlock_mega_door("mega_door.001");
   if (game_event->is_type("unlock_elevator_1")) unlock_door("door.003");
   if (game_event->is_type("unlock_elevator_2")) unlock_door("door.005");
   if (game_event->is_type("unlock_vr_room")) unlock_sliding_door("sliding_door.001");
   if (game_event->is_type("unlock_elevator_3")) unlock_door("door.006");
   if (game_event->is_type("unlock_elevator_4")) unlock_sliding_door("sliding_door.002");
   return;
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
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"event_emitter\" not met");
   }
   if (!(dialog_system))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"dialog_system\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"dialog_system\" not met");
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

std::pair<int, std::string> Tree::get_entities_names_in_list()
{
   // TODO: Test this method
   std::set<std::string> result;
   for (auto entity : *entities) result.insert(entity->name);

   std::stringstream result_str;
   result_str << "[";
   for (auto& res : result)
   {
      result_str << res << ", ";
   }
   result_str << "]";

   return { result.size(), result_str.str() };
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
   if (!(entities))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::find_entity_by_name_or_throw]: error: guard \"entities\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::find_entity_by_name_or_throw]: error: guard \"entities\" not met");
   }
   // TODO: Improve error message on entity_with_name_exists(name)
   if (!entity_with_name_exists(name))
   {
      int num_entities = entities->size();
      int num_unique_entity_names = 0;
      std::string unique_entity_names = "";
      std::tie(num_unique_entity_names, unique_entity_names) = get_entities_names_in_list();

      AllegroFlare::Logger::throw_error(
         "Krampus24::Game::Scripting::Tree::find_entity_by_name_or_throw",
         "An entity with the name \"" + name + "\" does not exist. There are " + std::to_string(num_entities)
            + " entities, and the following " + std::to_string(num_unique_entity_names) + " unique entity "
            "names are present: " + unique_entity_names
      );
   }

   for (auto entity : *entities)
   {
      if (entity->name == name) return entity;
   }
   // TODO: Improve throw
   throw std::runtime_error("asdfasdfasdfasdfasdfasdf");
   return nullptr;
}

bool Tree::interact_with_focused_object(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with)
{
   if (!(inspectable_entity_that_player_is_currently_colliding_with))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::interact_with_focused_object]: error: guard \"inspectable_entity_that_player_is_currently_colliding_with\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::interact_with_focused_object]: error: guard \"inspectable_entity_that_player_is_currently_colliding_with\" not met");
   }
   //throw std::runtime_error("-------------------------=-=-=-=--=======================");

   auto &name = inspectable_entity_that_player_is_currently_colliding_with->name;

   if (name == "sliding_door.001")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("locked_door");
   }
   else if (name == "console-01")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-01-dialog");
   }
   else if (name == "console-02")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-02-dialog");
   }
   else if (name == "console-03")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-03-dialog");
   }
   else if (name == "console-04")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-04-dialog");
   }
   else if (name == "console-05")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-05-dialog");
   }
   else if (name == "console-06")
   {
      event_emitter->emit_activate_dialog_node_by_name_event("console-06-dialog");
   }
      //{ find_entity_by_name_or_throw("console-01"), [this](){
         //travel_player_to_elevators_target("elevator-01");
      //}},


   // Return "true" if an interaction occurred (otherwise it will indicate to the Gameplay/Screen that nothing
   // happened, it may play a "no interaction" sound, for example)
   return false;
}

void Tree::link_elevators(std::string elevator_a_name, std::string elevator_b_name)
{
   Krampus24::Gameplay::Entities::Base* elevator_a = find_entity_by_name_or_throw(elevator_a_name);
   Krampus24::Gameplay::Entities::Base* elevator_b = find_entity_by_name_or_throw(elevator_b_name);
   elevator_a->elevator__target = elevator_b;
   elevator_b->elevator__target = elevator_a;
   return;
}

void Tree::set_elevator_shaft_num_tiers(std::string elevator_shaft_name, float num_tiers)
{
   Krampus24::Gameplay::Entities::Base* e = find_entity_by_name_or_throw(elevator_shaft_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::ElevatorShaft*>(e);
   as->set_num_tiers(num_tiers);
   return;
}

void Tree::customize_door_style(std::string door_object_name, Krampus24::Gameplay::Entities::Door::Style door_style)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::Door*>(door);
   as->set_style(door_style);
   return;
}

void Tree::lock_sliding_door(std::string sliding_door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(sliding_door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::SlidingDoor*>(door);
   as->lock();
   return;
}

void Tree::unlock_sliding_door(std::string sliding_door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(sliding_door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::SlidingDoor*>(door);
   as->unlock();
   return;
}

void Tree::lock_mega_door(std::string mega_door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(mega_door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::MegaDoor*>(door);
   as->lock();
   return;
}

void Tree::unlock_mega_door(std::string mega_door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(mega_door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::MegaDoor*>(door);
   as->unlock();
   return;
}

void Tree::lock_door(std::string door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::Door*>(door);
   as->lock();
   return;
}

void Tree::unlock_door(std::string door_object_name)
{
   Krampus24::Gameplay::Entities::Base* door = find_entity_by_name_or_throw(door_object_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::Door*>(door);
   as->unlock();
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
   // Build and load the dialog system
   dialog_system->set_dialog_node_bank(build_dialog_node_bank());


   // Customize some door styles
   customize_door_style("door.008", Krampus24::Gameplay::Entities::Door::Style::STYLE_NORMAL_DISRUPTED);

   //lock_door("door.003"); // Elevator on 1st floor leading up to 2nd
   //lock_door("door.005"); // Elevator on 2nd floor leading up to 3rd floor
   //lock_door("door.006"); // Elevator on 3rd floor (armory) leading up to 4rd floor
   //lock_sliding_door("sliding_door.001"); // Door to VR room (1st floor)
   //lock_sliding_door("sliding_door.002"); // Elevator on 4th floor leading to final room (with power coil)
   //lock_mega_door("mega_door.001"); // Major door on the first floor


   set_elevator_shaft_num_tiers("elevator_shaft-01", 6);

   // Link the elevators
   //link_elevators("elevator-01", "elevator-02");
   //link_elevators("elevator-03", "elevator-04");
   //link_elevators("elevator-05", "elevator-06");
   //link_elevators("elevator-07", "elevator-08");
   //link_elevators("elevator-09", "elevator-10");

   on_entity_collision_callbacks = {
      //{ find_entity_by_name_or_throw("hen-01"), [this](){
         //travel_player_to_elevators_target("elevator-01");
      //}},

      //{ find_entity_by_name_or_throw("elevator-01"), [this](){
         //travel_player_to_elevators_target("elevator-01");
      //}},
      //{ find_entity_by_name_or_throw("elevator-02"), [this](){
         //travel_player_to_elevators_target("elevator-02");
      //}},
      //{ find_entity_by_name_or_throw("elevator-03"), [this](){
         //travel_player_to_elevators_target("elevator-03");
      //}},
      //{ find_entity_by_name_or_throw("elevator-04"), [this](){
         //travel_player_to_elevators_target("elevator-04");
      //}},
      //{ find_entity_by_name_or_throw("elevator-05"), [this](){
         //travel_player_to_elevators_target("elevator-05");
      //}},
      //{ find_entity_by_name_or_throw("elevator-06"), [this](){
         //travel_player_to_elevators_target("elevator-06");
      //}},
      //{ find_entity_by_name_or_throw("elevator-07"), [this](){
         //travel_player_to_elevators_target("elevator-07");
      //}},
      //{ find_entity_by_name_or_throw("elevator-08"), [this](){
         //travel_player_to_elevators_target("elevator-08");
      //}},
      //{ find_entity_by_name_or_throw("elevator-09"), [this](){
         //travel_player_to_elevators_target("elevator-09");
      //}},
      //{ find_entity_by_name_or_throw("elevator-10"), [this](){
         //travel_player_to_elevators_target("elevator-10");
      //}},

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

std::string Tree::u(std::string string)
{
   return AllegroFlare::StringTransformer(string).upcase().get_text();
}

AllegroFlare::DialogTree::NodeBank Tree::build_dialog_node_bank()
{
   AllegroFlare::DialogTree::NodeBank result;
   result.set_nodes({

      //console-01-dialog

      { "console-01-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Disable lock on F1 MegaDoor", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_mega_door"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "console-02-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Unlock Elevator 1", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_elevator_1"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "console-03-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Unlock Elevator 2", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_elevator_2"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "console-04-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Unlock VR Room", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_vr_room"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "console-05-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Unlock Elevator 3", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_elevator_3"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "console-06-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "System",
            { u("What would you like to do?") },
            {
               { "Unlock Last Elevator", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_elevator_4"), 0 },
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "locked_door", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "",
            { u("This door is locked.") },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "unlock_mega_door", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_mega_door", "exit_dialog")
      },
      { "unlock_elevator_1", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_elevator_1", "exit_dialog")
      },
      { "unlock_elevator_2", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_elevator_2", "exit_dialog")
      },
      { "unlock_elevator_3", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_elevator_3", "exit_dialog")
      },
      { "unlock_elevator_4", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_elevator_4", "exit_dialog")
      },
      { "unlock_vr_room", new AllegroFlare::DialogTree::Nodes::EmitGameEvent("unlock_vr_room", "exit_dialog")
      },

      { "exit_dialog", new AllegroFlare::DialogTree::Nodes::ExitDialog()
      },









      { "hydroflora", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Hydroflora",
            { "Alien mushroom with a green cap and vibrant purple stem." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "default_inspect_fallback", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "",
            { "There doesn't seem to be anything of importance here." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "station_alarm_is_activated", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Control System",
            //{ "There doesn't seem to be anything of importance here." },
            {
               "IMPORTANT: An alarm was triggered from the outside deck.",
               "This is an emergency sitation.",
               "A system outside the ship has been damaged. You must be repair it.",
               "The space suit cabinet has been unlocked. Collect the space suit "
                  "and exit the airlock onto the deck to perform repairs.",
            },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },


      { "inspect_cryo_pod", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Cryo Pod",
            { "Used for suspending life forms for extended periods of time. Can also be used for general "
               "surgeries, medical proceedures, or adminstration of chemicals." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "inspect_engine_core", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Engine Core",
            { "The primary power source for the ship. The Entinite core provides power for all "
                 "systems on the ship, including thrust, stabilization, and life support." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
      { "inspect_airlock_door_when_locked", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "Airlock Door",
            { "This is the only exit to the space station's exterior.",
              "You cannot go outside without wearing a space suit." },
            {
               //{ "Advance", nullptr, 0 }, // Should be close dialog
               { "Exit", new AllegroFlare::DialogTree::NodeOptions::ExitDialog(), 0 },
            }
         )
      },
   });
   return result;
}

ALLEGRO_FONT* Tree::obtain_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -52");
}


} // namespace Scripting
} // namespace Game
} // namespace Krampus24


