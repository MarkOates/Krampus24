

#include <Krampus24/Gameplay/Entities/SlidingDoor.hpp>

#include <AllegroFlare/Logger.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
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


SlidingDoor::SlidingDoor()
   : Krampus24::Gameplay::Entities::Base()
   , event_emitter(nullptr)
   , initial_position(AllegroFlare::Vec3D(0, 0, 0))
   , door(nullptr)
   , frame(nullptr)
   , open_position(0.0f)
   , speed(0.0195f)
   , locked(true)
   , state(STATE_UNDEF)
   , state_is_busy(false)
   , state_changed_at(0.0f)
   , style(Krampus24::Gameplay::Entities::SlidingDoor::STYLE_NORMAL)
   , uv_offset_x(0.0f)
   , uv_offset_y(0.0f)
   , initialized(false)
{
}


SlidingDoor::~SlidingDoor()
{
   //if (initialized)
   //{
      //AllegroFlare::Logger::info_from(
         //"Krampus24::Gameplay::Entities::SlidingDoor",
         //"This class will need to destroy its local copies of 
      //);
   //}
   return;
}


uint32_t SlidingDoor::get_state() const
{
   return state;
}


Krampus24::Gameplay::Entities::SlidingDoor::Style SlidingDoor::get_style() const
{
   return style;
}


float SlidingDoor::get_uv_offset_x() const
{
   return uv_offset_x;
}


float SlidingDoor::get_uv_offset_y() const
{
   return uv_offset_y;
}


std::vector<Krampus24::Gameplay::Entities::Base*> SlidingDoor::construct(AllegroFlare::ModelBin* model_bin, AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::EventEmitter* event_emitter, AllegroFlare::Vec3D initial_position, float rotation)
{
   if (!(model_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"model_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"model_bin\" not met");
   }
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"bitmap_bin\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::construct]: error: guard \"event_emitter\" not met");
   }
   // Main entity
   Krampus24::Gameplay::Entities::SlidingDoor* result = new Krampus24::Gameplay::Entities::SlidingDoor;
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
   result->door = new Krampus24::Gameplay::Entities::Base;
   result->door->model = model_bin->auto_get("sliding_door-01-door.obj");
   result->door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->door->affected_by_environmental_forces = false;
   result->door->collides_with_player = false;
   result->door->placement.position = { 0.0, 0.0, 0.0 };
   result->door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->door->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->door->visible = false;
   //result->left_door->active = false;

   result->frame = new Krampus24::Gameplay::Entities::Base;
   result->frame->model = model_bin->auto_get("sliding_door-01-frame.obj");
   result->frame->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->frame->affected_by_environmental_forces = false;
   result->frame->collides_with_player = false;
   result->frame->placement.position = { 0.0, 0.0, 0.0 };
   result->frame->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->frame->placement.size = { 0, 0, 0 };
   //result->left_door->placement.rotation.y = rotation;
   result->frame->visible = false;

   /*
   // Right door
   result->right_door = new Krampus24::Gameplay::Entities::Base;
   result->right_door->model = model_bin->auto_get("sliding_door-01-door.obj");
   result->right_door->texture = bitmap_bin->auto_get("entities_texture-01.png");
   result->right_door->affected_by_environmental_forces = false;
   result->right_door->collides_with_player = false;
   result->right_door->placement.position = { 0.0, 0.0, 0.0 };
   result->right_door->placement.align = { 0.0, 0.0, 0.0 }; // Not sure how this will make sense
   result->right_door->placement.size = { 0, 0, 0 };
   //result->right_door->placement.rotation.y = rotation;
   result->right_door->visible = false;
   //result->right_door->active = false;
   */

   // Preload the samples
   //result->sample_bin = sample_bin;
   //sample_bin->preload(DOOR_OPEN_SAMPLE_IDENTIFIER);
   result->event_emitter = event_emitter;


   result->initialized = true;
   result->set_state(STATE_CLOSED);

   return { result, result->door, result->frame }; //, result->right_door };
}

void SlidingDoor::unlock()
{
   locked = false;
   return;
}

void SlidingDoor::lock()
{
   locked = true;
   return;
}

void SlidingDoor::set_style(Krampus24::Gameplay::Entities::SlidingDoor::Style style)
{
   this->style = style;
   std::tie(uv_offset_x, uv_offset_y) = get_uv_offset_from_style(style);
   return;
}

void SlidingDoor::set_uv_offset_x(float uv_offset_x)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_x = uv_offset_x;
   return;
}

void SlidingDoor::set_uv_offset_y(float uv_offset_y)
{
   this->style = STYLE_USER_CUSTOM_DEFINED_UV;
   this->uv_offset_y = uv_offset_y;
   return;
}

std::pair<float, float> SlidingDoor::get_uv_offset_from_style(Krampus24::Gameplay::Entities::SlidingDoor::Style style)
{
   if (!((style != STYLE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::get_uv_offset_from_style]: error: guard \"(style != STYLE_UNDEF)\" not met");
   }
   switch (style)
   {
      case STYLE_NORMAL: return { 0, 0 }; break;

      case STYLE_NORMAL_DISRUPTED: return { 0.05, 0.0 }; break;

      case STYLE_BARN: return { 0.2, 0.2 }; break;

      case STYLE_FIRE: return { 0.25, 0.20 }; break; // NOTE: This one is not really practical, just for debugging

      default:
         AllegroFlare::Logger::throw_error(
            "ClassName::get_uv_offset_from_style",
            "Unable to handle case for style \"" + std::to_string(style) + "\""
         );
      break;
   }

   AllegroFlare::Logger::throw_error(
      "ClassName::get_uv_offset_from_style",
      "Unexpected code path to here (code: 678sdf678sdf678sdf)"
   );
   return { 0, 0 };
}

void SlidingDoor::draw()
{
   placement.start_transform();

   //std::pair<float, float> uv_offset = get_uv_offset_from_style(style);//STYLE_NORMAL_DISRUPTED);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset.first);
   //AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset.second);
   AllegroFlare::Shaders::Base::set_float("uv_offset_x", uv_offset_x);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", uv_offset_y);

   //right_door->placement.start_transform();
   //right_door->model->set_texture(right_door->texture);
   //right_door->model->draw();
   //right_door->placement.restore_transform();

   door->placement.start_transform();
   door->model->set_texture(door->texture);
   door->model->draw();
   door->placement.restore_transform();

   frame->placement.start_transform();
   frame->model->set_texture(frame->texture);
   frame->model->draw();
   frame->placement.restore_transform();

   AllegroFlare::Shaders::Base::set_float("uv_offset_x", 0.0);
   AllegroFlare::Shaders::Base::set_float("uv_offset_y", 0.0);

   placement.restore_transform();
   return;
}

void SlidingDoor::set_open_position(float open_position)
{
   open_position = std::max(std::min(1.0f, open_position), 0.0f);
   this->open_position = open_position;
   door->placement.position.z = open_position * 2;
   //right_door->placement.position.z = -open_position * 2;
   return;
}

void SlidingDoor::on_enter_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   set_state(STATE_OPENING);
   return;
}

void SlidingDoor::on_exit_player_bbox_collision(Krampus24::Gameplay::Entities::Base* player_entity)
{
   set_state(STATE_CLOSING);
   return;
}

void SlidingDoor::on_time_step(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::on_time_step]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::on_time_step]: error: guard \"initialized\" not met");
   }
   update_state(time_step, time_now);
   return;
}

void SlidingDoor::play_open_door_sound_effect()
{
   //sample_bin->operator[](DOOR_OPEN_SAMPLE_IDENTIFIER)->play();
   event_emitter->emit_play_sound_effect_event("open_metal_door");
   return;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> SlidingDoor::build_audio_controller_sound_effect_list()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> sound_effect_elements = {
      { "open_metal_door", { "door-01-opening.ogg", false, "restart" } },
   };
   return sound_effect_elements;
}

void SlidingDoor::set_state(uint32_t state, bool override_if_busy)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::set_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::set_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::set_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::set_state]: error: guard \"is_valid_state(state)\" not met");
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

void SlidingDoor::update_state(double time_step, double time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::update_state]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::update_state]: error: guard \"initialized\" not met");
   }
   if (!(is_valid_state(state)))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::update_state]: error: guard \"is_valid_state(state)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::update_state]: error: guard \"is_valid_state(state)\" not met");
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

bool SlidingDoor::is_valid_state(uint32_t state)
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

bool SlidingDoor::is_state(uint32_t possible_state)
{
   return (state == possible_state);
}

float SlidingDoor::infer_current_state_age(float time_now)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Krampus24::Gameplay::Entities::SlidingDoor::infer_current_state_age]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("[Krampus24::Gameplay::Entities::SlidingDoor::infer_current_state_age]: error: guard \"initialized\" not met");
   }
   return (time_now - state_changed_at);
}


} // namespace Entities
} // namespace Gameplay
} // namespace Krampus24


