

#include <Krampus24/Gameplay/Entities/Door.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Vec2D.hpp>
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


Door::Door()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , left_door(nullptr)
   , right_door(nullptr)
   , open_position(0.0f)
   , speed(0.0165f)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , initialized(false)
{
}


Door::~Door()
{
}


uint32_t Door::get_state() const
{
   return state;
}


std::vector<Krampus24::Gameplay::Entities::Base*> Door::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::construct]: error: guard \"event_emitter\" not met");
   }
   // Main entity
   Krampus24::Gameplay::Entities::Door* result = new Krampus24::Gameplay::Entities::Door;
   //result->model = model_bin->auto_get("door-01.obj");
   //result->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->affected_by_environmental_forces = false;
   result->collides_with_player = true;
   result->placement.position = initial_position;
   result->placement.position.y += 0.001f; // Move slightly up
   result->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->placement.size = { 10.0, 10.0, 10.0 };
   result->aabb3d.set_max(result->placement.size);
   result->aabb3d_alignment = { 0.5, 0.005, 0.5 }; // Just slightly below the floor
   result->initial_position = initial_position;
   result->placement.rotation.y = rotation;

   // Left door
   result->left_door = new Krampus24::Gameplay::Entities::Base;
   result->left_door->model = model_bin->auto_get("door-01-left_door.obj");
   result->left_door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->left_door->affected_by_environmental_forces = false;
   result->left_door->collides_with_player = false;
   result->left_door->placement.position = { 0.0, 0.0, 0.0 };
   result->left_door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->left_door->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->left_door->visible = false;
   //result->left_door->active = false;

   // Right door
   result->right_door = new Krampus24::Gameplay::Entities::Base;
   result->right_door->model = model_bin->auto_get("door-01-right_door.obj");
   result->right_door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->right_door->affected_by_environmental_forces = false;
   result->right_door->collides_with_player = false;
   result->right_door->placement.position = { 0.0, 0.0, 0.0 };
   result->right_door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->right_door->placement.size = { 0, 0, 0 };
   //result->right_door->placement.rotation.y = rotation;
   result->right_door->visible = false;
   //result->right_door->active = false;

   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_CLOSED);

   return { result, result->left_door, result->right_door };
}

void Door::draw()
{
   placement.start_transform();

   right_door->placement.start_transform();
   right_door->model->set_texture(right_door->texture);
   right_door->model->draw();
   right_door->placement.restore_transform();

   left_door->placement.start_transform();
   left_door->model->set_texture(left_door->texture);
   left_door->model->draw();
   left_door->placement.restore_transform();

   placement.restore_transform();
   return;
}

void Door::set_open_position(float open_position)
{
   open_position = std::max(std::min(1.0f, open_position), 0.0f);
   this->open_position = open_position;
   left_door->placement.position.z = open_position * 2;
   right_door->placement.position.z = -open_position * 2;
   return;
}

void Door::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   set_state(STATE_OPENING);
   return;
}

void Door::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   set_state(STATE_CLOSING);
   return;
}

void Door::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void Door::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> Door::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void Door::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::set_state]: error: guard \"is_valid_state(state)\" not met");
   }
   if (this->state == state) return;
   if (!override_if_busy && state_is_busy) return;
   uint32_t previous_state = this->state;

   this->state = state;
   state_changed_at = al_get_time();

   switch (state)
   {
      case STATE_OPENING: {
         play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
         set_open_position(1.0f);
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
      } break;

      case STATE_CLOSING: {
         play_open_door_sound_effect();
         //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
         //set_state(STATE_CLOSED);
      } break;

      case STATE_CLOSED: {
         set_open_position(0.0f);
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

void Door::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::update_state]: error: guard \"is_valid_state(state)\" not met");
   }
   float age = infer_current_state_age(time_now);

   switch (state)
   {
      case STATE_OPENING: {
         set_open_position(open_position + speed);
         if (open_position >= 1.0) set_state(STATE_OPEN);
      } break;

      case STATE_OPEN: {
      } break;

      case STATE_CLOSING: {
         set_open_position(open_position - speed);
         if (open_position <= 0.0) set_state(STATE_CLOSED);
      } break;

      case STATE_CLOSED: {
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

bool Door::is_valid_state(uint32_t state)
{
   std::set<uint32_t> valid_states =
   {
      STATE_OPENING,
      STATE_OPEN,
      STATE_CLOSING,
      STATE_CLOSED
   };
   return (valid_states.count(state) > 0);
}

bool Door::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float Door::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::Door::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::Door::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


