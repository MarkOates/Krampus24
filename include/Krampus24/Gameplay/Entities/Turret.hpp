#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Model3D.hpp>
#include <AllegroFlare/ModelBin.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <Krampus24/Gameplay/Entities/Turret.hpp>
#include <cstdint>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace Entities
      {
         class Turret : public Krampus24::Gameplay::Entities::Base
         {
         public:
            static constexpr char* BLENDER_IDENTIFIER = (char*)"turret";

         private:
            enum State
            {
               STATE_UNDEF = 0,
               STATE_IDLE,
               STATE_POWERING_UP,
               STATE_BROKEN,
            };
            bool initialized;
            int power_bar_level;
            float full_power_charge_duration;
            AllegroFlare::Model3D* body;
            AllegroFlare::Model3D* power_bar_1;
            AllegroFlare::Model3D* power_bar_2;
            AllegroFlare::Model3D* power_bar_3;
            AllegroFlare::Model3D* power_bar_4;
            uint32_t state;
            bool state_is_busy;
            float state_changed_at;

         protected:


         public:
            Turret();
            virtual ~Turret();

            bool get_initialized() const;
            uint32_t get_state() const;
            void initialize();
            static Krampus24::Gameplay::Entities::Turret* construct(AllegroFlare::ModelBin* model_bin=nullptr, AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::Vec3D position={}, float rotation=0.0f);
            virtual void draw() override;
            virtual bool on_player_inspect_or_use() override;
            virtual void on_time_step(double time_step=0.0f, double time_now=0.0f) override;
            void set_state(uint32_t state=STATE_UNDEF, bool override_if_busy=false);
            void update_state(double time_step=0.0f, double time_now=0.0f);
            static bool is_valid_state(uint32_t state=STATE_UNDEF);
            bool is_state(uint32_t possible_state=STATE_UNDEF);
            float infer_current_state_age(float time_now=al_get_time());
         };
      }
   }
}



