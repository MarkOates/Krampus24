

#include <Krampus24/Game/Scripting/Tree.hpp>

#include <AllegroFlare/DialogTree/NodeOptions/ExitDialog.hpp>
#include <AllegroFlare/DialogTree/NodeOptions/GoToNode.hpp>
#include <AllegroFlare/DialogTree/Nodes/MultipageWithOptions.hpp>
#include <AllegroFlare/Elements/StoryboardPages/AdvancingText.hpp>
#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Routers/Standard.hpp>
#include <AllegroFlare/StringTransformer.hpp>
#include <AllegroFlare/TimerFormatter.hpp>
#include <AllegroFlare/UsefulPHP.hpp>
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
   , data_folder_path(DEFAULT_DATA_FOLDER_PATH)
   , event_emitter(nullptr)
   , font_bin(nullptr)
   , dialog_system(nullptr)
   , entities(nullptr)
   , arbitrary_storyboard_screen_identifier_to_start("[unset-arbitrary_storyboard_screen_identifier_to_start]")
   , primary_power_coil_collected(false)
   , primary_power_coil_returned_to_ship(false)
   , destruct_countdown_showing(false)
   , destruct_sequence_started(false)
   , destruct_sequence_started_at(0.0)
   , destruct_countdown_started(false)
   , destruct_countdown_duration_msec(120000*2)
   , destruct_countdown_timer({})
   , collision_observer(nullptr)
   , initialized(false)
{
}


Tree::~Tree()
{
}


void Tree::set_data_folder_path(std::string data_folder_path)
{
   this->data_folder_path = data_folder_path;
}


void Tree::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_event_emitter]: error: guard \"get_initialized()\" not met.");
   this->event_emitter = event_emitter;
}


void Tree::set_font_bin(AllegroFlare::FontBin* font_bin)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_font_bin]: error: guard \"get_initialized()\" not met.");
   this->font_bin = font_bin;
}


void Tree::set_dialog_system(AllegroFlare::DialogSystem::DialogSystem* dialog_system)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_dialog_system]: error: guard \"get_initialized()\" not met.");
   this->dialog_system = dialog_system;
}


void Tree::set_entities(std::vector<Krampus24::Gameplay::Entities::Base*>* entities)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_entities]: error: guard \"get_initialized()\" not met.");
   this->entities = entities;
}


void Tree::set_arbitrary_storyboard_screen_identifier_to_start(std::string arbitrary_storyboard_screen_identifier_to_start)
{
   this->arbitrary_storyboard_screen_identifier_to_start = arbitrary_storyboard_screen_identifier_to_start;
}


void Tree::set_primary_power_coil_collected(bool primary_power_coil_collected)
{
   this->primary_power_coil_collected = primary_power_coil_collected;
}


void Tree::set_primary_power_coil_returned_to_ship(bool primary_power_coil_returned_to_ship)
{
   this->primary_power_coil_returned_to_ship = primary_power_coil_returned_to_ship;
}


void Tree::set_destruct_countdown_showing(bool destruct_countdown_showing)
{
   this->destruct_countdown_showing = destruct_countdown_showing;
}


void Tree::set_destruct_sequence_started(bool destruct_sequence_started)
{
   this->destruct_sequence_started = destruct_sequence_started;
}


void Tree::set_destruct_sequence_started_at(float destruct_sequence_started_at)
{
   this->destruct_sequence_started_at = destruct_sequence_started_at;
}


void Tree::set_destruct_countdown_started(bool destruct_countdown_started)
{
   this->destruct_countdown_started = destruct_countdown_started;
}


void Tree::set_destruct_countdown_duration_msec(int destruct_countdown_duration_msec)
{
   this->destruct_countdown_duration_msec = destruct_countdown_duration_msec;
}


void Tree::set_destruct_countdown_timer(AllegroFlare::Timer destruct_countdown_timer)
{
   this->destruct_countdown_timer = destruct_countdown_timer;
}


void Tree::set_collision_observer(AllegroFlare::CollisionObservers::Simple* collision_observer)
{
   if (get_initialized()) throw std::runtime_error("[Tree::set_collision_observer]: error: guard \"get_initialized()\" not met.");
   this->collision_observer = collision_observer;
}


std::string Tree::get_data_folder_path() const
{
   return data_folder_path;
}


std::string Tree::get_arbitrary_storyboard_screen_identifier_to_start() const
{
   return arbitrary_storyboard_screen_identifier_to_start;
}


bool Tree::get_primary_power_coil_collected() const
{
   return primary_power_coil_collected;
}


bool Tree::get_primary_power_coil_returned_to_ship() const
{
   return primary_power_coil_returned_to_ship;
}


bool Tree::get_destruct_countdown_showing() const
{
   return destruct_countdown_showing;
}


bool Tree::get_destruct_sequence_started() const
{
   return destruct_sequence_started;
}


float Tree::get_destruct_sequence_started_at() const
{
   return destruct_sequence_started_at;
}


bool Tree::get_destruct_countdown_started() const
{
   return destruct_countdown_started;
}


int Tree::get_destruct_countdown_duration_msec() const
{
   return destruct_countdown_duration_msec;
}


AllegroFlare::Timer Tree::get_destruct_countdown_timer() const
{
   return destruct_countdown_timer;
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

   //audio_controller->set_and_load_sound_effect_elements(
      //Krampus24::Gameplay::Entities::Door::build_audio_controller_sound_effect_list()
   //);

   //audio_controller->set_and_load_music_track_elements(
      //Krampus24::Game::Scripting::Tree::build_audio_controller_music_track_list()
   //);
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> Tree::build_audio_controller_sound_effect_list()
{
   return Krampus24::Gameplay::Entities::Door::build_audio_controller_sound_effect_list();

   //std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      //{ "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   //};
   //return sound_effect_elements;
   return {};
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> Tree::build_audio_controller_music_track_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> elements = {
      { "opening", { "opening-b-04.ogg", false, "true" } },
      { "escape", { "escape-01.ogg", true, "none" } },
   };
   return elements;
}

void Tree::update_step(double time_now, double delta_time)
{
   if (destruct_sequence_started && !destruct_countdown_started)
   {
      float time_since_destruct_sequence_activated = al_get_time() - destruct_sequence_started_at;
      if (time_since_destruct_sequence_activated > 11.0) // Per the music, the trombones start at 11 seconds in
      {
         start_destruct_timer();
      }
   }
   return;
}

void Tree::render_hud()
{
   //if (!destruct_countdown_started) start_countdown_timer(); // DEVELOPMENT


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
   //destruct_countdown_showing = true;
   if (destruct_countdown_showing)
   {
      //destruct_countdown_timer
      ALLEGRO_FONT *font = obtain_countdown_font();

      std::string timer_str = AllegroFlare::TimerFormatter(get_countdown_time_now_msec()).format();

      al_draw_multiline_textf(
         font,
         ALLEGRO_COLOR{0.8, 0.08, 0.02, 1.0},
         1920/2,
         1080/6*4,
         1920,
         al_get_font_line_height(font),
         ALLEGRO_ALIGN_CENTER,
         "STATION SELF DESTRUCT\n%s",
            timer_str.c_str()
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

void Tree::start_destruct_sequence()
{
   destruct_countdown_showing = true;
   //destruct_countdown_timer.reset();
   //destruct_countdown_timer.start();
   destruct_sequence_started = true;
   destruct_sequence_started_at = al_get_time();
   //destruct_countdown_started = true;
   event_emitter->emit_play_music_track_event("escape"); // TODO: Uncomment this
   return;
}

void Tree::start_destruct_timer()
{
   //destruct_countdown_showing = true;
   destruct_countdown_timer.reset();
   destruct_countdown_timer.start();
   //destruct_sequence_started = true;
   //destruct_sequence_started_at = al_get_time();
   destruct_countdown_started = true;
   return;
}

int Tree::get_countdown_time_now_msec()
{
   if (!destruct_countdown_started) return destruct_countdown_duration_msec;
   return std::max(0, destruct_countdown_duration_msec - destruct_countdown_timer.get_elapsed_time_milliseconds());
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
   if (!((data_folder_path != DEFAULT_DATA_FOLDER_PATH)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"(data_folder_path != DEFAULT_DATA_FOLDER_PATH)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"(data_folder_path != DEFAULT_DATA_FOLDER_PATH)\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"event_emitter\" not met");
   }
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::initialize]: error: guard \"font_bin\" not met");
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

void Tree::spawn_arbitrary_storyboard_screen(std::string storyboard_identifier)
{
   arbitrary_storyboard_screen_identifier_to_start = storyboard_identifier;
   event_emitter->emit_router_event(
      AllegroFlare::Routers::Standard::EVENT_ACTIVATE_ARBITRARY_STORYBOARD_SCREEN,
      nullptr,
      al_get_time()
   );
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
   else if (name == "tablet.001")
   {
      spawn_arbitrary_storyboard_screen("tablet_in_zoo");
   }
   else if (name == "tablet.002")
   {
      spawn_arbitrary_storyboard_screen("tablet_in_docking_bay");
   }
   else if (name == "tablet.003")
   {
      spawn_arbitrary_storyboard_screen("tablet_in_mess_hall");
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
   else if (name == "power_coil")
   {
      if (!primary_power_coil_collected)
      {
         retrieve_primary_power_coil();
         start_destruct_sequence();
         //retrieve_coil

         //primary_power_coil_collected = true;
         // Consider emitting some kind of event, like telling the user to return to the ship or something
      }
   }
   else if (name == "player_ship")
   {
      if (primary_power_coil_collected)
      {
         primary_power_coil_returned_to_ship = true; // NOTE This signals the end of the game for now
      }
      else
      {
         event_emitter->emit_activate_dialog_node_by_name_event("player_ship-dialog");
      }
   }
      //{ find_entity_by_name_or_throw("console-01"), [this](){
         //travel_player_to_elevators_target("elevator-01");
      //}},


   // Return "true" if an interaction occurred (otherwise it will indicate to the Gameplay/Screen that nothing
   // happened, it may play a "no interaction" sound, for example)
   return false;
}

Krampus24::Gameplay::Entities::PowerCoil* Tree::find_power_coil()
{
   return static_cast<Krampus24::Gameplay::Entities::PowerCoil*>(find_entity_by_name_or_throw("power_coil"));
}

void Tree::retrieve_primary_power_coil()
{
   auto power_coil = find_power_coil();
   if (power_coil->coil_is_present())
   {
      power_coil->retrieve_coil();
   }
   primary_power_coil_collected = true;
   return;
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

void Tree::snap_elevator_shaft_to_bottom(std::string elevator_shaft_name)
{
   Krampus24::Gameplay::Entities::Base* e = find_entity_by_name_or_throw(elevator_shaft_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::ElevatorShaft*>(e);
   as->snap_to_bottom();
   //as->set_num_tiers(num_tiers);
   return;
}

void Tree::snap_elevator_shaft_to_top(std::string elevator_shaft_name)
{
   Krampus24::Gameplay::Entities::Base* e = find_entity_by_name_or_throw(elevator_shaft_name);

   // NOTE: Warning: assuming this is an Entities::Door!
   // TODO: Validate this is a door!
   auto as = static_cast<Krampus24::Gameplay::Entities::ElevatorShaft*>(e);
   as->snap_to_top();
   //as->set_num_tiers(num_tiers);
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

   lock_door("door.003"); // Elevator on 1st floor leading up to 2nd
   lock_door("door.005"); // Elevator on 2nd floor leading up to 3rd floor
   lock_door("door.006"); // Elevator on 3rd floor (armory) leading up to 4rd floor
   lock_sliding_door("sliding_door.001"); // Door to VR room (1st floor)
   lock_sliding_door("sliding_door.002"); // Elevator on 4th floor leading to final room (with power coil)
   lock_mega_door("mega_door.001"); // Major door on the first floor


   set_elevator_shaft_num_tiers("elevator_shaft.001", 6);

   // Link the elevators
   //link_elevators("elevator-01", "elevator-02");
   //link_elevators("elevator-03", "elevator-04");
   //link_elevators("elevator-05", "elevator-06");
   //link_elevators("elevator-07", "elevator-08");
   //link_elevators("elevator-09", "elevator-10");

   on_entity_collision_callbacks = {
      { find_entity_by_name_or_throw("door.001"), [this](){
         snap_elevator_shaft_to_bottom("elevator_shaft.001");
      }},
      { find_entity_by_name_or_throw("door.002"), [this](){
         snap_elevator_shaft_to_top("elevator_shaft.001");
      }},

      { find_entity_by_name_or_throw("door.003"), [this](){
         snap_elevator_shaft_to_bottom("elevator_shaft.005");
      }},
      { find_entity_by_name_or_throw("door.004"), [this](){
         snap_elevator_shaft_to_top("elevator_shaft.005");
      }},

      { find_entity_by_name_or_throw("door.005"), [this](){
         snap_elevator_shaft_to_bottom("elevator_shaft.002");
      }},
      { find_entity_by_name_or_throw("door.007"), [this](){
         snap_elevator_shaft_to_top("elevator_shaft.002");
      }},

      { find_entity_by_name_or_throw("door.006"), [this](){
         snap_elevator_shaft_to_bottom("elevator_shaft.003");
      }},
      { find_entity_by_name_or_throw("door.009"), [this](){
         snap_elevator_shaft_to_top("elevator_shaft.003");
      }},

      { find_entity_by_name_or_throw("sliding_door.002"), [this](){
         snap_elevator_shaft_to_bottom("elevator_shaft.004");
      }},
      { find_entity_by_name_or_throw("sliding_door.003"), [this](){
         snap_elevator_shaft_to_top("elevator_shaft.004");
      }},

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

      //{ find_entity_by_name_or_throw("player_ship"), [this](){
         ////travel_player_to_elevators_target("elevator-10");
         //if (primary_power_coil_collected)
         //{
            //primary_power_coil_returned_to_ship = true;
         //}
      //}},
      //{ find_entity_by_name_or_throw("power_coil"), [this](){
         //find_entity_by_name_or_throw("power_coil")->active = false;
         //find_entity_by_name_or_throw("power_coil")->visible = false;
         //if (!primary_power_coil_collected) primary_power_coil_collected = true;
      //}},
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
      { "player_ship-dialog", new AllegroFlare::DialogTree::Nodes::MultipageWithOptions(
            "",
            {
               u("This is my ship. It's a bit worn."),
               u("Hopefully I can retrieve the power coil on this station and trade it in for some fresh new parts.")
            },
            {
               //{ "Unlock Last Elevator", new AllegroFlare::DialogTree::NodeOptions::GoToNode("unlock_elevator_4"), 0 },
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

AllegroFlare::Elements::StoryboardPages::Base* Tree::create_storyboard_page__text(std::string page_text)
{
   if (!((!page_text.empty())))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::create_storyboard_page__text]: error: guard \"(!page_text.empty())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::create_storyboard_page__text]: error: guard \"(!page_text.empty())\" not met");
   }
   auto page = new AllegroFlare::Elements::StoryboardPages::AdvancingText(font_bin, page_text);
   return page;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base*> Tree::build_storyboard_text_from_file(std::string filename)
{
   std::vector<AllegroFlare::Elements::StoryboardPages::Base*> pages_result;

   std::string full_file_location = data_folder_path + "stories/" + filename;
   if (!std::filesystem::exists(full_file_location))
   {
      AllegroFlare::Logger::throw_error(
         "Krampus24::Game::Scripting::Tree::build_storyboard_text_from_file",
         "Could not find file \"" + full_file_location + "\"."
      );
   }

   std::string file_content = AllegroFlare::php::file_get_contents(full_file_location);
   std::string delim = "---PAGE---";
   std::vector<std::string> pages = AllegroFlare::StringTransformer::split_multichar_delim(file_content, delim);

   for (auto &page : pages)
   {
      std::string trimmed = AllegroFlare::StringTransformer::trim(page);
      if (trimmed.empty()) continue;
      pages_result.push_back(create_storyboard_page__text(trimmed));
   }

   if (pages_result.empty())
   {
      AllegroFlare::Logger::throw_error(
         "Krampus24::Game::Scripting::Tree::build_storyboard_text_from_file",
         "When loading \"" + full_file_location + "\", the number of pages that assembled was unexpectedly 0."
      );
   }

   return pages_result;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> Tree::create_arbitrary_storyboard_pages_by_identifier(std::string identifier)
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Game::Scripting::Tree::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Game::Scripting::Tree::create_arbitrary_storyboard_pages_by_identifier]: error: guard \"font_bin\" not met");
   }
   //AllegroFlare::FontBin* font_bin = primary_gameplay_screen->get_font_bin();

   //identifier = primary_gameplay_screen->get_arbitrary_storyboard_screen_identifier_to_start();
   //spawn_arbitrary_storyboard_screen
   //std::string identifier = arbitrary_storyboard_screen_identifier_to_start;
   identifier = arbitrary_storyboard_screen_identifier_to_start;

   std::vector<AllegroFlare::Elements::StoryboardPages::Base *> result = {};

   static std::map<std::string, std::function<void()>> items_map = {
      { "tablet_in_zoo", [this, &result]() {
         result = build_storyboard_text_from_file("tablet_in_zoo.txt");
      }},
      { "tablet_in_mess_hall", [this, &result]() {
         result = build_storyboard_text_from_file("tablet_in_mess_hall.txt");
      }},
      { "tablet_in_docking_bay", [this, &result]() {
         result = build_storyboard_text_from_file("tablet_in_docking_bay.txt");
      }},
   };

   // locate and call the function to handle the item
   if (items_map.count(identifier) == 0)
   {
      bool item_handled = false;

      if (!item_handled)
      {
         // item not found
         AllegroFlare::Logger::throw_error(
            "Krampus24::Game::Scripting::Tree::create_arbitrary_storyboard_pages_by_identifier",
            "Could not find listing for storyboard identifier \"" + identifier + "\"."
         );
      }
   }
   else
   {
      // call the item
      items_map[identifier]();
   }

   return result;



   /*
   if (identifier == "pig_storyboard")
   {
      result =
      {
         create_storyboard_page__text(//font_bin,
           "This is text from an arbitrary \"pig_storyboard\" storyboard screen."
         ),
      };
   }
   else
   {
      AllegroFlare::Logger::throw_error(
         "Krampus24::Game::Scripting::Tree::create_arbitrary_storyboard_pages_by_identifier"
         "Foobar boobaz"
      );
   }

   return result;
   */
}

ALLEGRO_FONT* Tree::obtain_hud_font()
{
   return font_bin->auto_get("Oswald-Medium.ttf -52");
}

ALLEGRO_FONT* Tree::obtain_countdown_font()
{
   return font_bin->auto_get("Michroma-Regular.ttf -82");
   //return font_bin->auto_get("Orbitron-Medium.ttf -82");
}


} // namespace Scripting
} // namespace Game
} // namespace Krampus24


