#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Physics/CollisionMesh.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/ElevatorShaft.hpp>
#include <allegro5/allegro.h>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class ElevatorShaft : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"elevator_shaft";
            static constexpr char* DOOR_OPEN_SAMPLE_IDENTIFIER = (char*)"door-01-opening.ogg";

         public:

            enum Style
            {
               STYLE_UNDEF = 0,
               STYLE_NORMAL,
               STYLE_NORMAL_DISRUPTED,
               STYLE_BARN,
               STYLE_FIRE,
               STYLE_USER_CUSTOM_DEFINED_UV,
            };
         private:
            enum State
            {
               STATE_UNDEF = 0,
               STATE_GOING_UP,
               STATE_AT_TOP,
               STATE_GOING_DOWN,
               STATE_AT_BOTTOM,
            };
            AllegroFlare::EventEmitter* event_emitter;
            AllegroFlare::Physics::CollisionMesh* collision_mesh;
            AllegroFlare::Vec3D initial_position;
            AllegroFlare::Model3D* shaft_bottom_cap;
            AllegroFlare::Model3D* shaft_column;
            AllegroFlare::Model3D* car;
            std::vector<std::string> car_body_dynamic_collision_mesh_face_names;
            std::vector<std::string> car_door_dynamic_collision_mesh_face_names;
            float elevation_position;
            float num_tiers;
            float speed;
            bool locked;
            bool using_is_disabled;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;
            Krampus24::Gameplay::Entities::ElevatorShaft::Style style;
            float uv_offset_x;
            float uv_offset_y;
            bool initialized;

         protected:


         public:
            ElevatorShaft();
            virtual ~ElevatorShaft();

            uint32_t get_state() const;
            Krampus24::Gameplay::Entities::ElevatorShaft::Style get_style() const;
            float get_uv_offset_x() const;
            float get_uv_offset_y() const;
            static void transform_model(AllegroFlare::Model3D* model=nullptr, ALLEGRO_TRANSFORM* transform=nullptr);
            static std::vector<Krampus24::Gameplay::Entities::Base*> construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::Physics::CollisionMesh* collision_mesh=nullptr, std::string name_for_collision_faces="[unset-name_for_collision_faces]", AllegroFlare::Vec3D initial_position=AllegroFlare::Vec3D(0, 0, 0), float rotation=0.0f);
            void set_num_tiers(float num_tiers=4.0f);
            bool is_enabled_for_use();
            void disable_for_use();
            void enable_for_use();
            bool attempt_to_move_elevator_up();
            bool attempt_to_move_elevator_down();
            virtual bool on_player_inspect_or_use() override;
            void lock();
            void unlock();
            bool elevator_is_moving();
            void snap_to_top_if_not_moving();
            void snap_to_bottom_if_not_moving();
            void set_style(Krampus24::Gameplay::Entities::ElevatorShaft::Style style=STYLE_UNDEF);
            void set_uv_offset_x(float uv_offset_x=0.0f);
            void set_uv_offset_y(float uv_offset_y=0.0f);
            static std::pair<float, float> get_uv_offset_from_style(Krampus24::Gameplay::Entities::ElevatorShaft::Style style=STYLE_UNDEF);
            void activate_collision_mesh();
            void deactivate_collision_mesh();
            virtual void draw() override;
            float calculate_local_elevator_car_y_position();
            float calculate_global_elevator_car_y_position();
            void set_elevation_position(float elevation_position=1.0f);
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



