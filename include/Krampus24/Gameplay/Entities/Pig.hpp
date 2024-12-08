#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Random.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Pig.hpp>
#include <cstdint>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Pig : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"pig";

         private:
            enum State
            {
               STATE_UNDEF = 0,
               STATE_STANDING,
               STATE_ROAMING,
               STATE_RETURNING,
               STATE_TURNING,
            };
            AllegroFlare::Vec3D initial_position;
            AllegroFlare::Model3D* legs_model;
            AllegroFlare::Model3D* head_model;
            float range;
            AllegroFlare::Vec3D movement_direction;
            float movement_velocity;
            float bobble_time;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;
            AllegroFlare::Random random;
            bool initialized;

         protected:


         public:
            Pig();
            virtual ~Pig();

            uint32_t get_state() const;
            static Krampus24::Gameplay::Entities::Pig* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::Vec3D initial_position=AllegroFlare::Vec3D(0, 0, 0), float range=3.0f);
            virtual void draw() override;
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f) override;
            void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
            void update_state(double time_step=0.0f, double time_now=0.0f);
            static bool is_valid_state(uint32_t state=STATE_UNDEF);
            bool is_state(uint32_t possible_state=STATE_UNDEF);
            float infer_current_state_age(float time_now=al_get_time());
            float infer_distance_from_initial_position();
            float manhattan_distance(AllegroFlare::Vec3D* point1=nullptr, AllegroFlare::Vec3D* point2=nullptr);
         };
      }
   }
}



