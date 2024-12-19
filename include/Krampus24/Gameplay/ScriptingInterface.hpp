#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <AllegroFlare/DialogTree/NodeBank.hpp>
#include <AllegroFlare/Elements/StoryboardPages/Base.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <functional>
#include <map>
#include <string>
#include <vector>


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
         virtual std::map<std::string, AllegroFlare::AudioRepositoryElement> build_audio_controller_sound_effect_list();
         virtual std::map<std::string, AllegroFlare::AudioRepositoryElement> build_audio_controller_music_track_list();
         virtual void render_hud();
         virtual void update_step(double time_now=0.0f, double delta_time=1.0f);
         virtual bool end_state_achieved();
         virtual AllegroFlare::DialogTree::NodeBank build_dialog_node_bank();
         virtual bool interact_with_focused_object(Krampus24::Gameplay::Entities::Base* inspectable_entity_that_player_is_currently_colliding_with=nullptr);
         bool has_on_collision_callback(void* entity=nullptr);
         void call_on_collision_callback(void* entity=nullptr);
         virtual std::vector<AllegroFlare::Elements::StoryboardPages::Base *> create_arbitrary_storyboard_pages_by_identifier(std::string identifier="[unset-identifier]");
      };
   }
}



