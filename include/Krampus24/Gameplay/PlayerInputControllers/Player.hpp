#pragma once


#include <AllegroFlare/Player.hpp>
#include <AllegroFlare/PlayerInputControllers/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <AllegroFlare/Vec3D.hpp>
#include <AllegroFlare/VirtualControllers/Base.hpp>
#include <Krampus24/Gameplay/Entities/Base.hpp>
#include <allegro5/allegro.h>


namespace Krampus24
{
   namespace Gameplay
   {
      namespace PlayerInputControllers
      {
         class Player : public AllegroFlare::PlayerInputControllers::Base
         {
         public:
            static constexpr char* TYPE = (char*)"Krampus24/PlayerInputControllers/Player";

         private:
            void* camera_entity;
            Krampus24::Gameplay::Entities::Base* player_entity;
            AllegroFlare::Vec2D player_control_move_velocity;
            AllegroFlare::Vec2D player_control_look_velocity;
            bool player_right_pressed;
            bool player_left_pressed;
            bool player_up_pressed;
            bool player_down_pressed;
            float move_multiplier;
            float spin_multiplier;
            float tilt_multiplier;
            float joystick_look_axis_deadzone_min_threshold;
            float joystick_move_axis_deadzone_min_threshold;
            bool initialized;
            void player_stop_moving();
            void player_spin_change(float delta=0.0f);
            void player_tilt_change(float delta=0.0f);
            AllegroFlare::Vec2D infer_player_control_move_velocity_from_keypress();
            AllegroFlare::Vec3D calculate_strafe_xy(float spin=0.0f, float displacement=0.0f);
            AllegroFlare::Vec3D calculate_forward_back_xy(float spin=0.0f, float displacement=0.0f);

         protected:


         public:
            Player(void* camera_entity=nullptr, Krampus24::Gameplay::Entities::Base* player_entity=nullptr);
            virtual ~Player();

            void set_camera_entity(void* camera_entity);
            void set_player_entity(Krampus24::Gameplay::Entities::Base* player_entity);
            void* get_camera_entity() const;
            Krampus24::Gameplay::Entities::Base* get_player_entity() const;
            float get_move_multiplier() const;
            void initialize();
            void set_move_multiplier(float move_multiplier=0.05f);
            virtual void update_time_step(double time_now=0.0, double delta_time=1.0 / 60.0) override;
            virtual void gameplay_suspend_func() override;
            virtual void gameplay_resume_func() override;
            virtual void virtual_control_button_up_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
            virtual void virtual_control_button_down_func(AllegroFlare::Player* player=nullptr, AllegroFlare::VirtualControllers::Base* virtual_controller=nullptr, int virtual_controller_button_num=0, bool is_repeat=false) override;
            virtual void key_down_func(ALLEGRO_EVENT* ev=nullptr) override;
            virtual void key_up_func(ALLEGRO_EVENT* ev=nullptr) override;
            virtual void joy_axis_func(ALLEGRO_EVENT* ev=nullptr) override;
            virtual void mouse_down_func(ALLEGRO_EVENT* ev=nullptr) override;
            virtual void mouse_axes_func(ALLEGRO_EVENT* ev=nullptr) override;
         };
      }
   }
}



