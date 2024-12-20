

#include <Krampus24/Gameplay/ScriptingInterface.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{


ScriptingInterface::ScriptingInterface()
   : on_entity_collision_callbacks({})
{
}


ScriptingInterface::~ScriptingInterface()
{
}


void ScriptingInterface::game_event_func(AllegroFlare::GameEvent* game_event)
{
   if (!(game_event))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::ScriptingInterface::game_event_func]: error: guard \"game_event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::ScriptingInterface::game_event_func]: error: guard \"game_event\" not met");
   }
   // Override in the derived class
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> ScriptingInterface::build_audio_controller_sound_effect_list()
{
   // Override in the derived class
   // EXAMPLE:
   //std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      //{ "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   //};
   //return sound_effect_elements;
   return {};
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> ScriptingInterface::build_audio_controller_music_track_list()
{
   // Override in the derived class
   // EXAMPLE:
   //std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      //{ "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   //};
   //return sound_effect_elements;
   return {};
}

void ScriptingInterface::render_hud()
{
   return;
}

void ScriptingInterface::draw_inspect_hint(std::string inspect_hint_text)
{
   // TODO: Consider moving all this inspect hint logic into the scripting interface
   return;
}

void ScriptingInterface::update_step(double time_now, double delta_time)
{
   // Override in the derived class
   return;
}

bool ScriptingInterface::end_state_achieved()
{
   return false;
}

AllegroFlare::DialogTree::NodeBank ScriptingInterface::build_dialog_node_bank()
{
   // Override in the derived class
   return {};
}

bool ScriptingInterface::interact_with_focused_object(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with)
{
   // Return "true" if an interaction occurred (otherwise it will indicate to the Gameplay/Screen that nothing
   // happened, it may play a "no interaction" sound, for example)
   return false;
}

bool ScriptingInterface::has_on_collision_callback(void* entity)
{
   return on_entity_collision_callbacks.find(entity) != on_entity_collision_callbacks.end();
}

void ScriptingInterface::call_on_collision_callback(void* entity)
{
   if (!(has_on_collision_callback(entity)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::ScriptingInterface::call_on_collision_callback]: error: guard \"has_on_collision_callback(entity)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::ScriptingInterface::call_on_collision_callback]: error: guard \"has_on_collision_callback(entity)\" not met");
   }
   on_entity_collision_callbacks[entity]();
   return;
}

std::vector<AllegroFlare::Elements::StoryboardPages::Base *> ScriptingInterface::create_arbitrary_storyboard_pages_by_identifier(std::string identifier)
{
   // Override in the derived class
   return {};
}


} // namespace Gameplay
} // namespace Krampus24


