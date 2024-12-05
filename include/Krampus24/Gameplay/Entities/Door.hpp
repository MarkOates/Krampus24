#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <cstdint>
#include <map>
#include <string>
#include <vector>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Door : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"door";
            static constexpr char* DOOR_OPEN_SAMPLE_IDENTIFIER = (char*)"door-01-opening.ogg";

         private:
            enum State
            {
               STATE_UNDEF = 0,
               STATE_OPENING,
               STATE_OPEN,
               STATE_CLOSING,
               STATE_CLOSED,
            };
            AllegroFlare::EventEmitter* event_emitter;
            AllegroFlare::Vec3D initial_position;
            Krampus24::Gameplay::Entities::Base* left_door;
            Krampus24::Gameplay::Entities::Base* right_door;
            float open_position;
            float speed;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;
            bool initialized;

         protected:


         public:
            Door();
            virtual ~Door();

            uint32_t get_state() const;
            static std::vector<Krampus24::Gameplay::Entities::Base*> construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::Vec3D initial_position=AllegroFlare::Vec3D(0, 0, 0), float rotation=0.0f);
            virtual void draw() override;
            void set_open_position(float open_position=1.0f);
            virtual void on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr) override;
            virtual void on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity=nullptr) override;
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f) override;
            void play_open_door_sound_effect();
            static std::map<std::string, AllegroFlare::AudioRepositoryElement> build_audio_controller_sound_effect_list();
            void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
            void update_state(double time_step=0.0f, double time_now=0.0f);
            static bool is_valid_state(uint32_t state=STATE_UNDEF);
            bool is_state(uint32_t possible_state=STATE_UNDEF);
            float infer_current_state_age(float time_now=al_get_time());
         };
      }
   }
}



