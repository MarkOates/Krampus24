#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Physics/CollisionMesh.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/LibraryComputer.hpp>
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
         class LibraryComputer : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"library_computer";
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
               STATE_OPENING,
               STATE_OPEN,
               STATE_CLOSING,
               STATE_CLOSED,
            };
            AllegroFlare::EventEmitter* event_emitter;
            AllegroFlare::Vec3D initial_position;
            AllegroFlare::Physics::CollisionMesh* collision_mesh;
            std::vector<std::string> dynamic_collision_mesh_face_names;
            Krampus24::Gameplay::Entities::Base* door;
            Krampus24::Gameplay::Entities::Base* frame;
            float open_position;
            float speed;
            bool locked;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;
            Krampus24::Gameplay::Entities::LibraryComputer::Style style;
            float uv_offset_x;
            float uv_offset_y;
            bool initialized;

         protected:


         public:
            LibraryComputer();
            virtual ~LibraryComputer();

            uint32_t get_state() const;
            Krampus24::Gameplay::Entities::LibraryComputer::Style get_style() const;
            float get_uv_offset_x() const;
            float get_uv_offset_y() const;
            static void transform_model(AllegroFlare::Model3D* model=nullptr, ALLEGRO_TRANSFORM* transform=nullptr);
            static float get_random_rotation();
            static std::vector<Krampus24::Gameplay::Entities::Base*> construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, AllegroFlare::Physics::CollisionMesh* collision_mesh=nullptr, std::string name_for_collision_faces="[unset-name_for_collision_faces]", AllegroFlare::Vec3D initial_position=AllegroFlare::Vec3D(0, 0, 0), float rotation=0.0f);
            void make_non_inspectable();
            void unlock();
            void lock();
            void attempt_to_open();
            void attempt_to_close();
            virtual bool on_player_inspect_or_use() override;
            void set_style(Krampus24::Gameplay::Entities::LibraryComputer::Style style=STYLE_UNDEF);
            void set_uv_offset_x(float uv_offset_x=0.0f);
            void set_uv_offset_y(float uv_offset_y=0.0f);
            static std::pair<float, float> get_uv_offset_from_style(Krampus24::Gameplay::Entities::LibraryComputer::Style style=STYLE_UNDEF);
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



