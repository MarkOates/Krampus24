#pragma once


#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <functional>
#include <map>


namespace Krampus24
{
   namespace Gameplay
   {
      class ScriptingInterface
      {
      private:

      protected:


      public:
         std::map<void*, std::function<void()>> on_entity_collision_callbacks;
         ScriptingInterface();
         virtual ~ScriptingInterface();

         virtual void game_event_func(AllegroFlare::GameEvent* game_event=nullptr);
         virtual void render_hud();
         virtual bool end_state_achieved();
         virtual AllegroFlare::DialogTree::NodeBank build_dialog_node_bank();
         virtual bool interact_with_focused_object(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with=nullptr);
         bool has_on_collision_callback(void* entity=nullptr);
         void call_on_collision_callback(void* entity=nullptr);
      };
   }
}



