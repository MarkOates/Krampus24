

#include <Krampus24/Gameplay/Entities/Turret.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <allegro5/allegro_color.h>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>


namespace Krampus24
{
namespace Gameplay
{
namespace Entities
{


Turret::Turret()
   : Krampus24::Gameplay::Entities::Base()
   , initialized(false)
   , power_bar_level(2)
   , full_power_charge_duration(2.0f)
   , body(nullptr)
   , power_bar_1(nullptr)
   , power_bar_2(nullptr)
   , power_bar_3(nullptr)
   , power_bar_4(nullptr)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
{
}


Turret::~Turret()
{
}


bool Turret::get_initialized() const
{
   return initialized;
}


uint32_t Turret::get_state() const
{
   return state;
}


void Turret::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::initialize]: error: guard \"(!initialized)\" not met");
   }
   initialized = true;
   return;
}

Krampus24::Gameplay::Entities::Turret* Turret::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::Vec3D position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::construct]: error: guard \"bitmap_bin\" not met");
   }
   // TODO: Sort out the alignment, size, of objects, their collision boxes, their repositioning and the relationship
   // to the bounding box, as well as the collision padding of the player entity
   //float width = 2;
   //float height = 2;
   //AllegroFlare::Vec3D box_start_position = position; // + AllegroFlare::Vec3D(1, 0, 0);
   //AllegroFlare::Vec3D box_end_position = position; // + AllegroFlare::Vec3D(5, 0, 0);

   // Make the manager
   auto manager = new Krampus24::Gameplay::Entities::Turret;
   manager->model = model_bin->auto_get("turret-11-legs.obj");
   //manager->texture = bitmap_bin->auto_get("turret-11-body.png");
   manager->texture = bitmap_bin->auto_get("entities_texture-01.png");
   manager->placement.position = position;
   manager->placement.rotation.y = rotation;
   manager->placement.size = { 6.0, 3.0, 6.0 };
   manager->placement.align = { 0.0, 0.0, 0.0 };
   manager->placement.scale = { 1.0, 1.0, 1.0 };

   // Make inspectable
   manager->collides_with_player = true;
   manager->player_can_inspect_or_use = true;
   manager->aabb3d.set_max(manager->placement.size); //{ 5.0, 1.5, 5.0 });
   manager->aabb3d_alignment = { 0.5, 0.0, 0.5 };


   manager->body = model_bin->auto_get("turret-11-body.obj");
   manager->power_bar_1 = model_bin->auto_get("turret-11-power_bar_1.obj");
   manager->power_bar_2 = model_bin->auto_get("turret-11-power_bar_2.obj");
   manager->power_bar_3 = model_bin->auto_get("turret-11-power_bar_3.obj");
   manager->power_bar_4 = model_bin->auto_get("turret-11-power_bar_4.obj");


   manager->initialize();

   manager->set_state(STATE_IDLE);

   // DEVELOPMENT: For now, just going to make an interactable zone to trigger the action on this entity
   //manager->set_hit_box_2d(AllegroFlare::Physics::AABB2D(0, 0, 20, 20));
   //manager->set(AllegroFlare::Prototypes::TileFPS::EntityFlags::PLAYER_CAN_INSPECT_OR_USE);
   //manager->set(AllegroFlare::Prototypes::TileFPS::EntityFlags::COLLIDES_WITH_PLAYER);

   return manager;
}

void Turret::draw()
{
   placement.start_transform();

   ALLEGRO_COLOR color = al_color_name("dodgerblue");
   AllegroFlare::Shaders::Base::set_vec3("color_lift", color.r, color.g, color.b);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.08);
   AllegroFlare::Shaders::Base::set_int("color_lift_blend_mode", 2);


   //AllegroFlare::Shaders::Base::set_float("color_lift", al_color_name("lightpink"));
   //AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.0);

   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);



   bool bar_1_on = power_bar_level >= 1;
   bool bar_2_on = power_bar_level >= 2;
   bool bar_3_on = power_bar_level >= 3;
   bool bar_4_on = power_bar_level >= 4;



   float base_bar_uv_offset_x = 0.3-0.1;
   float base_bar_uv_offset_y = 0.05;
   float on_offset = 0.2f;


   //float bar_1_uv_offset_x = 0.2f + base_bar_uv_offset_x;
   float bar_1_uv_offset_x = (bar_1_on ? on_offset : 0.0f) + base_bar_uv_offset_x;
   float bar_1_uv_offset_y = 0.0 + base_bar_uv_offset_y;

   float bar_2_uv_offset_x = (bar_2_on ? on_offset : 0.0f) + base_bar_uv_offset_x;
   float bar_2_uv_offset_y = 0.0 + base_bar_uv_offset_y;

   float bar_3_uv_offset_x = (bar_3_on ? on_offset : 0.0f) + base_bar_uv_offset_x;
   float bar_3_uv_offset_y = 0.0 + base_bar_uv_offset_y;

   float bar_4_uv_offset_x = (bar_4_on ? on_offset : 0.0f) + base_bar_uv_offset_x;
   float bar_4_uv_offset_y = 0.0 + base_bar_uv_offset_y;



   //door->placement.start_transform();
   model->set_texture(texture);
   model->draw();

   body->set_texture(texture);
   body->draw();

   //AllegroFlare::Shaders::Base::set_float("color_lift", al_color_name("red"));
   //AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.3);

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", bar_1_uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", bar_1_uv_offset_y);
   power_bar_1->set_texture(texture);
   power_bar_1->draw();

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", bar_2_uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", bar_2_uv_offset_y);
   power_bar_2->set_texture(texture);
   power_bar_2->draw();

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", bar_3_uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", bar_3_uv_offset_y);
   power_bar_3->set_texture(texture);
   power_bar_3->draw();

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", bar_4_uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", bar_4_uv_offset_y);
   power_bar_4->set_texture(texture);
   power_bar_4->draw();
   //door->placement.restore_transform();



   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);
   AllegroFlare::Shaders::Base::set_int("color_lift_blend_mode", 0);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.0);
   AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0);

   placement.restore_transform();
   return;
}

bool Turret::on_player_inspect_or_use()
{
   // TODO: Consider some interaction here
   //throw std::runtime_error("asdfasfasdf");
   if (is_state(STATE_IDLE)) set_state(STATE_POWERING_UP);
   return true;
}

void Turret::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void Turret::set_state(uint32_t state, bool override_if_busy)
{
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;
   //this->state = state;
   //state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_IDLE: {
         power_bar_level = 0;
      } break;

      case STATE_POWERING_UP: {
         power_bar_level = 0; // DEVELOPMENT
      } break;

      case STATE_BROKEN: {
         power_bar_level = 0; // DEVELOPMENT
      } break;

      default: {
         AllegroFlare::Logger::throw_error(
            "ClassName::set_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      } break;
   }

   this->state = state;
   state_changed_at = al_get_time();

   return;
}

void Turret::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Turret::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Turret::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_IDLE: {
      } break;

      case STATE_POWERING_UP: {
         //throw std::runtime_error("asdfasdf");
         power_bar_level = ((age / full_power_charge_duration) * 5);
         if (age >= full_power_charge_duration)
         {
            power_bar_level = 4;
            set_state(STATE_BROKEN);
            // TODO: Play broke sound effect
         }
      } break;

      case STATE_BROKEN: {
      } break;

      default: {
         AllegroFlare::Logger::throw_error(
            "ClassName::update_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      } break;
   }

   return;
}

bool Turret::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_IDLE,
      STATE_POWERING_UP,
      STATE_BROKEN,
   };
   return (valid_states.count(state) > 0);
}

bool Turret::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Turret::infer_current_state_age(float time_now)
{
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


