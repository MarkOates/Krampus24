

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

void ScriptingInterface::render_hud()
{
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


} // namespace Gameplay
} // namespace Krampus24


