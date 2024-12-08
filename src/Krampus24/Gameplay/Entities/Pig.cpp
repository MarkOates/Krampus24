

#include <Krampus24/Gameplay/Entities/Pig.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <allegro5/allegro_color.h>
#include <cmath>
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


Pig::Pig()
   : Krampus24::Gameplay::Entities::Base()
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , legs_model(nullptr)
   , head_model(nullptr)
   , range(3.0f)
   , movement_direction({})
   , movement_velocity(0.01f)
   , bobble_time(0.0f)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , random({})
   , initialized(false)
{
}


Pig::~Pig()
{
}


uint32_t Pig::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::Pig* Pig::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::Vec3D initial_position, float range)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::construct]: error: guard \"bitmap_bin\" not met");
   }
   Krampus24::Gameplay::Entities::Pig* result = new Krampus24::Gameplay::Entities::Pig;
   result->model = model_bin->auto_get("pig-01-body.obj");
   result->texture = bitmap_bin->auto_get("entities_texture-01.png");


   result->legs_model = model_bin->auto_get("pig-01-legs.obj");
   result->head_model = model_bin->auto_get("pig-01-head.obj");


   //Krampus24::Gameplay::Entities::Base* result = new Krampus24::Gameplay::Entities::Base();

   //float x = 0; //entity->location.x;
   //float y = 0.5; //entity->location.z; // Swapping z<->y
   //float z = 0; //entity->location.y; // Swapping z<->y

   //AllegroFlare::Vec3D position = AllegroFlare::Vec3D(x, y, z);
   //initial_position

   result->placement.position = initial_position;
   result->placement.position.y += 0.001f; // Move slightly up
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->placement.size = { 0.5, 0.5, 0.5 };
   result->aabb3d.set_max({ 6.0, 3.0, 8.0 });
   result->aabb3d_alignment = { 0.5, 0.0, 0.5 };
   //result->collides_with_player = true;
   //result->affected_by_environmental_forces = affected_by_environmental_forces;

   //result->name = entity->name;
   //std::string entity_root_name = entity->get_name_unversioned();
   //if (entity_root_name == "elevator")
   //{
      // Do elevator stuff
      //result->placement.size = { 1.0, 2.0, 1.0 };
      //result->box_color = ALLEGRO_COLOR{ 1.0, 1.0, 0.4, 1.0 };
   //}

   result->player_can_inspect_or_use = true;

   result->initial_position = initial_position;
   result->range = range;
   result->movement_direction = AllegroFlare::Vec3D(0, 0, 1);
   //result->movement_velocity = 0.01;
   result->movement_velocity = 0.01;

   result->initialized = true;
   result->random.set_seed(4371);

   //result->set_state(STATE_STANDING);
   result->set_state(STATE_ROAMING);

   return result;
}

void Pig::draw()
{
   if (!model) return;
   if (texture)
   {
      model->set_texture(texture);
      legs_model->set_texture(texture);
      head_model->set_texture(texture);
   }

   ALLEGRO_COLOR color = al_color_name("lightpink");
   AllegroFlare::Shaders::Base::set_vec3("color_lift", color.r, color.g, color.b);
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.1);

   placement.start_transform();
   legs_model->draw();
   //head_model->draw();
   placement.restore_transform();

   placement.anchor.y = std::sin(bobble_time * 0.723) * 0.04;
   placement.rotation.z = std::sin(bobble_time * 0.823) * 0.02;
   placement.start_transform();
   head_model->draw();
   placement.restore_transform();

   placement.anchor.y = std::sin(bobble_time) * 0.05;
   placement.start_transform();
   model->draw();
   placement.restore_transform();
   placement.anchor.y = 0.0f;

   //AllegroFlare::Shaders::Base::set_float("color_lift", al_color_name("lightpink"));
   AllegroFlare::Shaders::Base::set_float("color_lift_intensity", 0.0);

   return;
}

void Pig::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void Pig::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_STANDING: {
      } break;

      case STATE_ROAMING: {
      } break;

      case STATE_RETURNING: {
      } break;

      case STATE_TURNING: {
      } break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::set_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      break;
   }

   return;
}

void Pig::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_STANDING: {
         //velocity.position = movement_direction * movement_velocity;
         //float anchor_x = 0.0;
         //float anchor_y = std::sin(time_now*12) * 0.05;
         //float anchor_z = 0.0;
         //placement.anchor = AllegroFlare::Vec3D(anchor_x, anchor_y, anchor_z);
      } break;

      case STATE_RETURNING: {
         if (infer_distance_from_initial_position() <= 0.25)
         {
            //float angle_in_radians = random.get_random_float(0, ALLEGRO_PI*2);
            //movement_direction = -movement_direction;
            float angle_in_radians = random.get_random_float(0, ALLEGRO_PI*2);
            //float new_direction_unit = new_direction_radians / ALLEGRO_PI*2;
            float angle_in_degrees = angle_in_radians * (180.0 / ALLEGRO_PI);
            //float angle_in_units = angle_in_degrees / 360.0f;
            float angle_in_units = angle_in_radians / (2 * ALLEGRO_PI); 

            auto new_dir_2d = AllegroFlare::Vec2D::polar_coords(angle_in_radians, 1.0f).normalized();
            movement_direction = AllegroFlare::Vec3D(new_dir_2d.x, 0, new_dir_2d.y);
            placement.rotation.y = -angle_in_units + 0.25;
            set_state(STATE_ROAMING);
            //velocity.position = movement_direction * movement_velocity;
            //movement_direction = (initial_position - placement.position).normalized();
         }
         else
         {
            velocity.position = movement_direction * movement_velocity * 2;
            //float anchor_x = 0.0;
            //float anchor_y = std::sin(time_now*22) * 0.05;
            //float anchor_z = 0.0;

            bobble_time = time_now * 22;
             //= std::sin(time_now*22) * 0.05;

            //placement.anchor = AllegroFlare::Vec3D(anchor_x, anchor_y, anchor_z);
         }
      } break;

      case STATE_ROAMING: {
         if (infer_distance_from_initial_position() >= range)
         {
            movement_direction = -movement_direction;
            placement.rotation.y += 0.5f;
            //movement_direction = -movement_direction;
            set_state(STATE_RETURNING);
            //movement_direction = (initial_position - placement.position).normalized();
         }
         else
         {
            velocity.position = movement_direction * movement_velocity;
            //float anchor_x = 0.0;
            //float anchor_y = std::sin(time_now*12) * 0.05;
            //float anchor_z = 0.0;

            bobble_time = time_now * 12;
            //bobble_ammount = std::sin(time_now*12) * 0.05;

            //placement.anchor = AllegroFlare::Vec3D(anchor_x, anchor_y, anchor_z);
         }
      } break;

      case STATE_TURNING: {
      } break;

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::update_state",
            "Unable to handle case for state \"" + std::to_string(state) + "\""
         );
      break;
   }

   return;
}

bool Pig::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_STANDING,
      STATE_RETURNING,
      STATE_ROAMING,
      STATE_TURNING,
   };
   return (valid_states.count(state) > 0);
}

bool Pig::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Pig::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Pig::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Pig::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}

float Pig::infer_distance_from_initial_position()
{
   return manhattan_distance(&initial_position, &placement.position);
}

float Pig::manhattan_distance(AllegroFlare::Vec3D* point1, AllegroFlare::Vec3D* point2)
{
   return std::abs(point2->x - point1->x) + std::abs(point2->y - point1->y) + std::abs(point2->z - point1->z);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


