#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Trinket.hpp>
#include <allegro5/allegro.h>
#include <cstdint>
#include <map>
#include <string>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Trinket : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"trinket";
            static constexpr char* DOOR_OPEN_SAMPLE_IDENTIFIER = (char*)"door-01-opening.ogg";

         public:

            enum TrinketType
            {
               TRINKET_TYPE_UNDEF = 0,
               TRINKET_TYPE_TABLET,
               TRINKET_TYPE_MEDAL_OF_HONOR,
            };
         private:
            enum State
            {
               STATE_UNDEF = 0,
               STATE_IDLE,
            };
            AllegroFlare::EventEmitter* event_emitter;
            AllegroFlare::ModelBin* model_bin;
            AllegroFlare::Vec3D initial_position;
            Krampus24::Gameplay::Entities::Base* door;
            Krampus24::Gameplay::Entities::Base* frame;
            float speed;
            bool locked;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;
            Krampus24::Gameplay::Entities::Trinket::TrinketType trinket_type;
            ALLEGRO_COLOR lift_color;
            float lift_color_intensity;
            float uv_offset_x;
            float uv_offset_y;
            bool initialized;

         protected:


         public:
            Trinket();
            virtual ~Trinket();

            uint32_t get_state() const;
            Krampus24::Gameplay::Entities::Trinket::TrinketType get_trinket_type() const;
            ALLEGRO_COLOR get_lift_color() const;
            float get_lift_color_intensity() const;
            float get_uv_offset_x() const;
            float get_uv_offset_y() const;
            static void transform_model(AllegroFlare::Model3D* model=nullptr, ALLEGRO_TRANSFORM* transform=nullptr);
            static float get_random_rotation();
            static Krampus24::Gameplay::Entities::Base* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::Vec3D initial_position=AllegroFlare::Vec3D(0, 0, 0), float rotation=0.0f);
            void unlock();
            void lock();
            virtual bool on_player_inspect_or_use() override;
            void set_trinket_type(Krampus24::Gameplay::Entities::Trinket::TrinketType trinket_type=TRINKET_TYPE_UNDEF);
            virtual void draw() override;
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



